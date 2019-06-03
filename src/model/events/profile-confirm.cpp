


#include "model/events/profile-confirm.h"

namespace sproof {

    ProfileConfirm::ProfileConfirm(const std::string &profile_id, bool value)
            : m_profile_id{profile_id}, m_value{value} {
    }

    Json::Value ProfileConfirm::get_data() const {
        Json::Value result;
        result["profileId"] = m_profile_id;
        result["value"] = m_value;
        return result;
    }

    std::string ProfileConfirm::get_event_name() const {
        return "PROFILE_CONFIRM";
    }

}

