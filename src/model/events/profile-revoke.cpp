
#include "model/events/profile-revoke.h"

namespace sproof {

    ProfileRevoke::ProfileRevoke(const std::string &reason)
            : m_reason{reason} {
    }

    Json::Value ProfileRevoke::get_data() const {
        Json::Value result;
        result["reason"] = m_reason;
        return result;
    }

    std::string ProfileRevoke::get_event_name() const {
        return "PROFILE_REVOKE";
    }

}
