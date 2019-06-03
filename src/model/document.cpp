
#include "model/document.h"

namespace sproof {

    Json::Value Document::to_json() const {
        Json::Value result;
        result["validFrom"] = Json::Value::UInt64{valid_from};
        result["validUntil"] = Json::Value::UInt64{valid_until};
        result["documentHash"] = document_hash;
        result["data"] = data;
        result["locationHash"] = location_hash;
        result["name"] = name;
        return result;
    }

}