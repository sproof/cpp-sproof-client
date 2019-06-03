#include <utility>


#include "json-exception.h"

JsonException::JsonException(std::string message) : BaseException(std::move(message)) {
}
