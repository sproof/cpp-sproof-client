
#ifndef SPROOF_CLIENT_HTTP_ERROR_H
#define SPROOF_CLIENT_HTTP_ERROR_H

#include <exception>

namespace sproof {

    class HttpException
            : public std::exception {
    public:
        explicit HttpException(const std::string &message, bool is_json = false);

        explicit HttpException(const Json::Value &error);

        const char *what() const noexcept override;

    private:
        std::string m_message;
    };

}


#endif //SPROOF_CLIENT_HTTP_ERROR_H
