
#ifndef SPROOF_CLIENT_PROFILE_CONFIRM_H
#define SPROOF_CLIENT_PROFILE_CONFIRM_H

#include "model/events/sproof-event.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "PROFILE_CONFIRM" Event
    */
    class ProfileConfirm
            : public SproofEvent {
    public:
        ProfileConfirm(const std::string &profile_id, bool value);

        virtual ~ProfileConfirm() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        std::string m_profile_id;
        bool m_value;
    };

}

#endif //SPROOF_CLIENT_PROFILE_CONFIRM_H
