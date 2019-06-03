#include <memory>
#include <string>
#include <thread>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include <sproof-utils/sproof-utils.h>

#include "http/http-client.h"
#include "http/http-error.h"

namespace {

    struct PostWriteData {
        const char* data;
        size_t left;
    };

    size_t writefunc(void *ptr, size_t size, size_t nmemb, void *userp) {
        ((std::string *) userp)->append((char *) ptr, size * nmemb);
        return size * nmemb;
    }

    size_t readfunc(void *dest, size_t size, size_t nmemb, void *userp)
    {
        auto* wt = (PostWriteData*) userp;
        if(wt->left) {
            /* copy as much as possible from the source to the destination */
            size_t copy_this_much = wt->left;
            if(copy_this_much > size)
                copy_this_much = size;
            memcpy(dest, wt->data, copy_this_much);

            wt->data += copy_this_much;
            wt->left -= copy_this_much;
            return copy_this_much; /* we copied this many bytes */
        }

        return 0; /* no more data left to deliver */
    }


    Json::Value execute_request(CURL* curl) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

        std::string result_string;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result_string);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw sproof::HttpException{std::string(curl_easy_strerror(res))};
        } else {
            Json::CharReaderBuilder builder;
            builder["collectComments"] = false;
            JSONCPP_STRING errs;
            Json::Value result;
            std::stringstream stream{result_string};
            if (!parseFromStream(builder, stream, &result, &errs)) {
                curl_easy_cleanup(curl);
                throw sproof::HttpException{errs};
            }
            curl_easy_cleanup(curl);

            Json::Value error = result["error"];
            if (!error.isNull())
                throw sproof::HttpException{error};

            Json::Value res{result["result"]};
            if (res.isNull())
                throw sproof::HttpException{std::string{"result element does not exist: " + result_string}};

            return res;
        }
    }

    std::string build_multipart_formdata(std::ifstream& file, const std::string& filename,
            const Json::Value& credentials, const std::string& boundary)
    {
        std::string separator = "--" + boundary + "\r\nContent-Disposition: form-data; name=";
        std::stringstream stream;
        std::stringstream filestring;
        filestring << file.rdbuf();
        std::string file_data{filestring.str()};
        const std::string base64_filedata{sproof::utils::base64_encode(file_data)};
        Json::FastWriter writer;
        const std::string base64_credentials{sproof::utils::base64_encode(writer.write(credentials))};

        stream << separator << R"("file"; filename=")" << filename << R"("\r\n)";
        stream << "Content-Type: text/plain\r\n";
        stream << "Content-Transfer-Encoding: base64\r\n\r\n";
        stream << base64_filedata << "\r\n";

        stream << separator << R"("credentials"\r\n)";
        stream << "Content-Type: text/plain\r\n";
        stream << "Content-Transfer-Encoding: base64\r\n\r\n";
        stream << base64_credentials << "\r\n--" << boundary << "--";
        return stream.str();
    }
}


namespace sproof {


    HttpClient::HttpClient() = default;


    Json::Value HttpClient::get(const std::string &path,
                                const std::unordered_map<std::string, std::string> &params) {
        std::string final_url = path + "?";
        for (const auto &pair : params)
            final_url += pair.first + "=" + pair.second;

        CURL* curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_URL, final_url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

            return execute_request(curl);
        }
        throw HttpException{std::string{"could not init curl"}};

    }

    Json::Value HttpClient::post(const std::string &path, const Json::Value &data) {

        CURL* curl = curl_easy_init();

        if (curl) {
            std::string final_url = path;

            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_URL, final_url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

            struct curl_slist *chunk = nullptr;

            Json::FastWriter writer;
            const std::string json_data = writer.write(data);
            size_t json_length = strlen(json_data.c_str());
            PostWriteData to_write{json_data.c_str(), json_length};

            curl_easy_setopt(curl, CURLOPT_READFUNCTION, readfunc);
            curl_easy_setopt(curl, CURLOPT_READDATA, &to_write);

            std::stringstream content_lengh_stream{};
            content_lengh_stream << "Content-Length: " << json_length;
            std::string content_length = content_lengh_stream.str();

            chunk = curl_slist_append(chunk, "Connection: close");
            chunk = curl_slist_append(chunk, "Content-Type: application/json");
            chunk = curl_slist_append(chunk, content_length.c_str());

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

            try {
                Json::Value result = execute_request(curl);
                curl_slist_free_all(chunk);
                return result;
            }
            catch (const HttpException &e) {
                curl_slist_free_all(chunk);
                throw e;
            }

        }
        throw HttpException{std::string{"could not init curl"}};
    }

    Json::Value
    HttpClient::post_file(const std::string &url_path, const std::string &file_path, const Json::Value &credentials) {
        std::ifstream input_stream{file_path};

        std::string filename = file_path;
        const size_t last_slash_idx = filename.find_last_of("\\/");
        if (std::string::npos != last_slash_idx)
        {
            filename.erase(0, last_slash_idx + 1);
        }

        return post_file(url_path, input_stream, filename, credentials);
    }

    Json::Value
    HttpClient::post_file(const std::string &url_path, std::ifstream &file_stream, const std::string& file_name,
            const Json::Value &credentials) {

        CURL * curl = curl_easy_init();

        if (curl) {
            std::string final_url = url_path;
            std::string boundary = sproof::utils::get_salt();

            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_URL, final_url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

            struct curl_slist *chunk = nullptr;

            std::string data = build_multipart_formdata(file_stream, file_name, credentials, boundary);
            size_t length = strlen(data.c_str());

            Json::FastWriter writer;
            const std::string json_data = writer.write(data);
            PostWriteData to_write{json_data.c_str(), length};

            curl_easy_setopt(curl, CURLOPT_READFUNCTION, readfunc);
            curl_easy_setopt(curl, CURLOPT_READDATA, &to_write);

            std::stringstream content_type_stream{};
            content_type_stream << "multipart/form-data;boundary=" << boundary;
            std::string content_type = content_type_stream.str();


            chunk = curl_slist_append(chunk, "Connection: close");
            chunk = curl_slist_append(chunk, "Content-Type: application/json");
            chunk = curl_slist_append(chunk, content_type.c_str());

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

            return Json::Value();
        }
        throw HttpException{std::string{"could not init curl"}};
    }

    std::thread HttpClient::get(const std::string &path, const std::unordered_map<std::string, std::string> &params,
                         std::function<void(Json::Value)> result_callback,
                         std::function<void(HttpException)> error_callback) {

        std::thread thread{[this, path, params, result_callback, error_callback](){

            try {
                Json::Value result = get(path, params);
                result_callback(result);
            }
            catch(const HttpException& e) {
                error_callback(e);
            }
        }};

        return std::move(thread);
    }

    std::thread
    HttpClient::post(const std::string &path, const Json::Value &data, std::function<void(Json::Value)> result_callback,
                     std::function<void(HttpException)> error_callback) {

        std::thread thread{[this, path, data, result_callback, error_callback](){

            try {
                Json::Value result = post(path, data);
                result_callback(result);
            }
            catch(const HttpException& e) {
                error_callback(e);
            }
        }};

        return std::move(thread);
    }

    std::thread
    HttpClient::post_file(const std::string &url_path, const std::string &file_path, const Json::Value &credentials,
                          std::function<void(Json::Value)> result_callback,
                          std::function<void(HttpException)> error_callback) {

        std::thread thread{[this, url_path, file_path, credentials, result_callback, error_callback](){

            try {
                Json::Value result = post_file(url_path, file_path, credentials);
                result_callback(result);
            }
            catch(const HttpException& e) {
                error_callback(e);
            }
        }};

        return std::move(thread);
    }



} //namespace sproof
