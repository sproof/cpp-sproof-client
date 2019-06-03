
#ifndef SPROOF_CLIENT_PROFILE_REVOKE_H
#define SPROOF_CLIENT_PROFILE_REVOKE_H

#include "model/events/sproof-event.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "PROFILE_Revoke" Event
    */
    class ProfileRevoke
            : public SproofEvent {
    public:
        ProfileRevoke(const std::string &reason);

        virtual ~ProfileRevoke() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        std::string m_reason;
    };

}

#endif //SPROOF_CLIENT_PROFILE_REVOKE_H
