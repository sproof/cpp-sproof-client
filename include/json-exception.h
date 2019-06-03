
#ifndef SPROOF_CLIENT_JSON_EXCEPTION_H
#define SPROOF_CLIENT_JSON_EXCEPTION_H

#include "base-exception.h"

class JsonException
: public BaseException
{
public:
    explicit JsonException(std::string message);
    ~JsonException() override = default;
};


#endif //SPROOF_CLIENT_JSON_EXCEPTION_H
