
#ifndef SPROOF_CLIENT_BASE_EXCEPTION_H
#define SPROOF_CLIENT_BASE_EXCEPTION_H

#include <exception>
#include <string>

class BaseException
: std::exception
{
public:
    explicit BaseException(std::string  message);
    ~BaseException() override = default;

    const char* what() const noexcept override;

private:
    std::string m_message;
};


#endif //SPROOF_CLIENT_BASE_EXCEPTION_H
