
#include "model/profile.h"

namespace sproof {

    Json::Value Profile::to_json() const {
        Json::Value result;
        result["name"] = name;
        result["profileText"] = profile_text;
        result["image"] = image;
        result["website"] = website;
        return result;
    }

}
