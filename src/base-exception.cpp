#include <utility>

#include "base-exception.h"

BaseException::BaseException(std::string message)
    : m_message{std::move(message)}
{

}

const char *BaseException::what() const noexcept {
    return m_message.c_str();
}
