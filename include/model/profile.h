
#ifndef SPROOF_CLIENT_PROFILE_H
#define SPROOF_CLIENT_PROFILE_H

#include <string>

#include "model/sproof-data.h"

namespace sproof {

    /**
    * Model-Klasse fuer ein Sproof Profile
    */
    struct Profile
            : public SproofData {

        Json::Value to_json() const override;

        std::string name;
        std::string profile_text;
        std::string image;
        std::string website;
    };

}

#endif //SPROOF_CLIENT_PROFILE_H
