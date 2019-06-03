

#include "model/events/profile-register.h"

namespace sproof {

    ProfileRegister::ProfileRegister(Profile profile)
            : m_profile{std::move(profile)} {
    }

    Json::Value ProfileRegister::get_data() const {
        return m_profile.to_json();
    }

    std::string ProfileRegister::get_event_name() const {
        return "PROFILE_REGISTER";
    }

}