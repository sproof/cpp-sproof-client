
#include <sstream>
#include <iostream>
#include <jsoncpp/json/json.h>

#include "http/http-error.h"

namespace {

    std::string to_string(const Json::Value &error) {
        using namespace Json;

        Value name = error["name"];
        Value message = error["message"];
        Value status_code = error["statusCode"];
        Value error_code = error["errorCode"];

        std::string n;
        std::string m;
        int status{0};
        int e{0};

        if (name.isString())
            n = name.asString();
        if (message.isString())
            m = message.asString();
        if (status_code.isInt())
            status = status_code.asInt();
        if (error_code.isInt())
            e = error_code.asInt();

        std::stringstream out;
        out << "HttpException['" << n;
        out << "',message='" << m;
        out << "',status=" << status;
        out << ",code=" << e << "]";
        return out.str();
    }

}

namespace sproof {

    HttpException::HttpException(const std::string &msg, bool is_json) {
        if (!is_json) {
            m_message = msg;
            return;
        }

        Json::Value json_root{msg};
        m_message = to_string(json_root);

    }

    HttpException::HttpException(const Json::Value &error) {
        m_message = to_string(error);
    }

    const char *HttpException::what() const noexcept {
        return m_message.c_str();
    }

}

