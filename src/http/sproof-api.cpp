#include <sstream>
#include <utility>

#include <sproof-utils/sproof-utils.h>

#include "http/sproof-api.h"


namespace {

    Json::Value to_json_array(const std::vector<std::unique_ptr<sproof::SproofEvent>>& events)
    {
        Json::Value array{Json::arrayValue};
        for(const auto& ev : events)
            array.append(ev->to_json());
        return array;
    }

    Json::Value to_json(const sproof::Signature& signature)
    {
        Json::Value root{};
        root["r"] = signature.r;
        root["s"] = signature.s;
        root["v"] = signature.v;
        return root;
    }

    Json::Value get_auth_data(const sproof::Credentials& credentials)
    {
        sproof::Signature signature = sproof::utils::sign(credentials.get_address(), credentials);
        Json::Value root{};
        root["address"] = credentials.get_address();
        root["r"] = signature.r;
        root["s"] = signature.s;
        root["v"] = signature.v;
        return root;
    }

    Json::Value get_auth_data_encapsulated(const sproof::Credentials& credentials)
    {
        sproof::Signature signature = sproof::utils::sign(credentials.get_address(), credentials);
        Json::Value root{};
        root["address"] = credentials.get_address();
        root["signature"] = to_json(signature);
        return root;
    }

}


namespace sproof {

    SproofApi::SproofApi(sproof::Config config)
        : m_config{std::move(config)}, m_http_client{}
    {
    }

    std::string SproofApi::get_final_url(const std::string &path) {

        std::stringstream urlbuilder;

        urlbuilder << m_config.url;

        if(m_config.url[m_config.url.length() - 1] != '/')
            urlbuilder << '/';

        urlbuilder << API_URL;

        if(path[0] == '/')
            urlbuilder << path.substr(1);
        else
            urlbuilder << path;

        return urlbuilder.str();
    }

    sproof::Signature SproofApi::sign_credentials(const sproof::Credentials& credentials) {
        return sproof::utils::sign(credentials.get_address(), credentials);
    }

    Json::Value SproofApi::get(const std::string &path) {
        std::unordered_map<std::string,std::string> params{};
        return get(path, params);
    }

    Json::Value SproofApi::get(const std::string &path, const std::unordered_map<std::string, std::string> &params) {
        return m_http_client.get(get_final_url(path), params);
    }

    std::thread
    SproofApi::get(const std::string &path, SproofApi::ResultCb success_callback, SproofApi::ErrorCb error_callback) {
        std::unordered_map<std::string,std::string> params{};
        return get(path, params, std::move(success_callback), std::move(error_callback));
    }

    std::thread SproofApi::get(const std::string &path, const std::unordered_map<std::string, std::string> &params,
                               std::function<void(Json::Value)> success_callback,
                               std::function<void(HttpException)> error_callback) {
        return m_http_client.get(get_final_url(path), params, std::move(success_callback), std::move(error_callback));
    }

    Json::Value SproofApi::post(const std::string &path, const Json::Value &data) {
        return m_http_client.post(get_final_url(path), data);
    }

    std::thread
    SproofApi::post(const std::string &path, const Json::Value &data, std::function<void(Json::Value)> result_callback,
                    std::function<void(HttpException)> error_callback) {
        return m_http_client.post(get_final_url(path), data, std::move(result_callback), std::move(error_callback));
    }

    Json::Value
    SproofApi::post_file(const std::string &url_path, const std::string &file_path, const Json::Value &credentials) {
        return m_http_client.post_file(get_final_url(url_path), file_path, credentials);
    }

    Json::Value
    SproofApi::post_file(const std::string &url_path, std::ifstream &file_stream, const std::string &file_name,
                         const Json::Value &credentials) {
        return m_http_client.post_file(get_final_url(url_path), file_stream, file_name, credentials);
    }

    std::thread
    SproofApi::post_file(const std::string &url_path, const std::string &file_path, const Json::Value &credentials,
                         std::function<void(Json::Value)> result_callback,
                         std::function<void(HttpException)> error_callback) {
        return m_http_client.post_file(get_final_url(url_path), file_path, credentials, std::move(result_callback),
                                       std::move(error_callback));
    }


    Json::Value SproofApi::get_status() {
        return get("/storage/status");
    }

    std::thread SproofApi::get_status(SproofApi::ResultCb result_callback, SproofApi::ErrorCb error_callback) {
        return get("/storage/status", std::move(result_callback), std::move(error_callback));
    }

    Json::Value SproofApi::register_premium_user(const Profile &profile) {
        return post("/user/register", profile.to_json());
    }

    std::thread SproofApi::register_premium_user(const Profile &profile, SproofApi::ResultCb result_callback,
                                                 SproofApi::ErrorCb error_callback) {
        return post("/user/register", profile.to_json(), std::move(result_callback), std::move(error_callback));
    }


    Json::Value SproofApi::get_hash_data(const EventList& events,
            const Credentials& credentials) {
        Json::Value obj;
        obj["data"] = to_json_array(events);
        obj["credentials"] = get_auth_data(credentials);
        return obj;
    }

    Json::Value SproofApi::get_hash(const EventList& events) {
        return post("/storage/hash", get_hash_data(events, m_config.credentials));
    }

    std::thread SproofApi::get_hash(const EventList& events, ResultCb result_callback, ErrorCb error_callback) {
        return post("/storage/hash", get_hash_data(events, m_config.credentials),
                std::move(result_callback), std::move(error_callback));
    }


    Json::Value SproofApi::get_raw_transaction_data(const EventList& events, const std::string& address) {
        Json::Value root{};
        root["events"] = to_json_array(events);
        root["address"] = address;
        return root;
    }

    Json::Value SproofApi::get_raw_transaction(const EventList &events) {
        return post("/storage/transaction", get_raw_transaction_data(events, m_config.credentials.get_address()));
    }

    std::thread SproofApi::get_raw_transaction(const EventList& events,
                                               ResultCb result_callback, ErrorCb error_callback) {
        return post("/storage/transaction", get_raw_transaction_data(events, m_config.credentials.get_address()),
                std::move(result_callback), std::move(error_callback));
    }


    Json::Value SproofApi::get_submit_data(const EventList &events, const std::string& tx_hex, const std::string& tx_hash) {
        // TODO: funktioniert noch nicht, brauche echtes tx_sign_result
        Json::Value root{};
        root["signedTx"] = tx_hex;
        root["transactionHash"] = tx_hash;
        root["events"] = to_json_array(events);
        return root;
    }

    Json::Value SproofApi::submit(const EventList &events, const std::string& tx_hex, const std::string& tx_hash) {
        return post("/storage/submit", get_submit_data(events, tx_hex, tx_hash));
    }

    std::thread
    SproofApi::submit(const EventList &events, const std::string& tx_hex, const std::string& tx_hash,
                      ResultCb result_callback, ErrorCb error_callback) {
        return post("/storage/submit", get_submit_data(events, tx_hex, tx_hash), std::move(result_callback),
                std::move(error_callback));
    }


    Json::Value SproofApi::get_submit_premium_data(const EventList &events, const std::string &from,
                                                   const std::string &hash, const Signature &signature) {
        Json::Value root{};
        root["from"] = from;
        root["hash"] = hash;
        root["signature"] = to_json(signature);
        root["events"] = to_json_array(events);
        root["credentials"] = get_auth_data_encapsulated(m_config.credentials);
        return root;
    }

    Json::Value
    SproofApi::submit_premium(const EventList &events, const std::string &from,
                              const std::string &hash, const Signature &signature) {
        return post("/storage/premium/submit", get_submit_premium_data(events, from, hash, signature));
    }

    std::thread
    SproofApi::submit_premium(const std::vector<std::unique_ptr<SproofEvent>> &events, const std::string &from,
                              const std::string &hash, const Signature &signature, SproofApi::ResultCb result_callback,
                              SproofApi::ErrorCb error_callback) {
        return post("/storage/premium/submit", get_submit_premium_data(events, from, hash, signature),
                std::move(result_callback), std::move(error_callback));
    }

    const sproof::Config &SproofApi::get_config() const {
        return m_config;
    }


}