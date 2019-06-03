
#ifndef SPROOF_CLIENT_SPROOF_DATA_H
#define SPROOF_CLIENT_SPROOF_DATA_H

#include <jsoncpp/json/json.h>

namespace sproof {

    class SproofData {
    public:
        virtual Json::Value to_json() const = 0;
    };

}

#endif //SPROOF_CLIENT_SPROOF_DATA_H
