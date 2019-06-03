

#include "model/events/profile-update.h"

namespace sproof {

    ProfileUpdate::ProfileUpdate(Profile profile)
            : m_profile{std::move(profile)} {
    }

    Json::Value ProfileUpdate::get_data() const {
        return m_profile.to_json();
    }

    std::string ProfileUpdate::get_event_name() const {
        return "PROFILE_UPDATE";
    }

}
