
#include <utility>
#include <tuple>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <memory>

#include <jsoncpp/json/json.h>
#include <sproof-utils/sproof-utils.h>
#include <curl/curl.h>

#include "sproof.h"
#include "json-exception.h"
#include "config.h"
#include "http/http-error.h"

namespace {

    auto config_from_inputstream(std::stringstream& in)
    {
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        JSONCPP_STRING errs;
        Json::Value result;
        if(!parseFromStream(builder, in, &result, &errs)) {
            throw JsonException{errs};
        }

        Json::Value uri_json{result["uri"]};
        if(uri_json.isNull() || !uri_json.isString())
            throw JsonException{"could not find valid 'uri' string in config file"};

        Json::Value credentials_json{result["credentials"]};
        if(credentials_json.isNull() || !credentials_json.isObject())
            throw JsonException{"could not find valid 'credentials' object in config file"};

        Json::Value sproof_code_json{credentials_json["sproofCode"]};
        if(sproof_code_json.isNull() || !sproof_code_json.isString())
            throw JsonException{"could not find valid 'sproofCode' string in 'credentials' object"};

        sproof::Credentials credentials = sproof::utils::restore_credentials(sproof_code_json.asString());
        return sproof::Config{uri_json.asString(), credentials};
    }

}

namespace sproof {

    Sproof::Sproof(const std::string &path_to_config) {
        std::stringstream in;
        std::ifstream fstream{path_to_config};
        in << fstream.rdbuf();
        fstream.close();
        //std::cout << in << std::endl;
        auto config = config_from_inputstream(in);
        m_api = std::move(std::make_unique<SproofApi>(config));
    }

    Sproof::Sproof(const std::ifstream &input_stream) {
        std::stringstream in;
        in << input_stream.rdbuf();
        auto config = config_from_inputstream(in);
        m_api = std::move(std::make_unique<SproofApi>(config));
    }

    Sproof::Sproof(const sproof::Config& config)
        : m_api{std::make_unique<SproofApi>(config)}
    {
    }

    void Sproof::add_event(std::unique_ptr<SproofEvent> event) {
        m_events.push_back(std::move(event));
    }


    void Sproof::register_profile(const Profile& profile) {
        m_events.push_back(std::make_unique<ProfileRegister>(profile));
    }

    void Sproof::revoke_profile(const std::string& reason) {
        m_events.push_back(std::make_unique<ProfileRevoke>(reason));
    }

    void Sproof::confirm_profile(const std::string& profile_id, bool value) {
        m_events.push_back(std::make_unique<ProfileConfirm>(profile_id, value));
    }

    void Sproof::update_profile(const Profile& profile) {
        m_events.push_back(std::make_unique<ProfileUpdate>(profile));
    }

    void Sproof::register_document(const Document& document) {
        m_events.push_back(std::make_unique<DocumentRegister>(document));
    }

    void Sproof::revoke_document(const std::string& document_hash, const std::string& reason) {
        m_events.push_back(std::make_unique<DocumentRevoke>(document_hash, reason));
    }

    void Sproof::add_document_receiver(const std::string& receiver_id, const std::string& document_hash) {
        m_events.push_back(std::make_unique<DocumentReceiverAdd>(receiver_id, document_hash));
    }

    void Sproof::revoke_document_receiver(const std::string& receiver_id, const std::string& reason) {
        m_events.push_back(std::make_unique<DocumentReceiverRevoke>(receiver_id, reason));
    }

    std::thread Sproof::commit(std::function<void(Json::Value)> result_callback,
                               std::function<void(HttpException)> error_callback) {

        return m_api->get_raw_transaction(m_events, [this, result_callback, error_callback](auto json){

            RawTransaction transaction = RawTransaction::from_json(json);
            auto pair = sproof::utils::sign_transaction(transaction, m_api->get_config().credentials);

            auto thread = m_api->submit(m_events, pair.first, pair.second, [this, result_callback](auto json){

                m_events.clear();
                result_callback(json);

            }, error_callback);

        }, error_callback);
    }

    std::thread Sproof::commit_premium(std::function<void(Json::Value)> result_callback,
                                       std::function<void(HttpException)> error_callback) {
        return m_api->get_hash(m_events, [this, result_callback, error_callback](Json::Value json){

            if(!json.isMember("result"))
                error_callback(HttpException{std::string{"no 'result' element in JSON data"}});

            Json::Value result = json["result"];

            if(!result.isMember("hashToRegister"))
                error_callback(HttpException{std::string{"no 'hashToRegister' element in JSON data"}});

            if(!result.isMember("hash"))
                error_callback(HttpException{std::string{"no 'hash' element in JSON data"}});

            std::string hashToRegister = result["hashToRegister"].asString();
            std::string hash = result["hash"].asString();

            Signature sig = sproof::utils::sign(hashToRegister, m_api->get_config().credentials);

            auto thread = m_api->submit_premium(m_events, m_api->get_config().credentials.get_address(),
                    hashToRegister, sig, [this, result_callback](auto json){

                        this->m_events.clear();
                        result_callback(json);

                }, error_callback);
            thread.join();

        }, error_callback);
    }

    const std::vector<std::unique_ptr<SproofEvent>> &Sproof::get_events() {
        return m_events;
    }

    std::thread Sproof::get_user(Sproof::ResultCb result_callback, Sproof::ErrorCb error_callback) {
        return m_api->get("user", std::move(result_callback), std::move(error_callback));
    }

    std::thread Sproof::get_state(Sproof::ResultCb result_callback, Sproof::ErrorCb error_callback) {
        return m_api->get("state", std::move(result_callback), std::move(error_callback));
    }

    std::thread
    Sproof::get_events(const std::unordered_map<std::string, std::string> &params, Sproof::ResultCb result_callback,
                       Sproof::ErrorCb error_callback) {
        return m_api->get("events", params, std::move(result_callback), std::move(error_callback));
    }

    std::thread Sproof::get_transactions(const std::unordered_map<std::string, std::string> &params,
                                  Sproof::ResultCb result_callback, Sproof::ErrorCb error_callback) {
        return m_api->get("transactions", params, std::move(result_callback), std::move(error_callback));
    }

    std::thread
    Sproof::get_profiles(const std::unordered_map<std::string, std::string> &params, Sproof::ResultCb result_callback,
                         Sproof::ErrorCb error_callback) {
        return m_api->get("profiles", params, std::move(result_callback), std::move(error_callback));
    }

    std::thread
    Sproof::get_receivers(const std::unordered_map<std::string, std::string> &params, Sproof::ResultCb result_callback,
                          Sproof::ErrorCb error_callback) {
        return m_api->get("receivers", params, std::move(result_callback), std::move(error_callback));
    }

    std::thread Sproof::get_registrations(const std::unordered_map<std::string, std::string> &params,
                                   Sproof::ResultCb result_callback, Sproof::ErrorCb error_callback) {
        return m_api->get("registrations", params, std::move(result_callback), std::move(error_callback));
    }

    std::thread
    Sproof::get_documents(const std::unordered_map<std::string, std::string> &params, Sproof::ResultCb result_callback,
                          Sproof::ErrorCb error_callback) {
        return m_api->get("documents", params, std::move(result_callback), std::move(error_callback));
    }

    std::thread
    Sproof::get_validation(const std::string& id, Sproof::ResultCb result_callback, Sproof::ErrorCb error_callback) {
        std::unordered_map<std::string, std::string> params;
        params["id"] = id;
        return m_api->get("verification", params, std::move(result_callback), std::move(error_callback));
    }



}