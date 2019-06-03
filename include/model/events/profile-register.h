

#ifndef SPROOF_CLIENT_PROFILE_REGISTER_H
#define SPROOF_CLIENT_PROFILE_REGISTER_H


#include "model/events/sproof-event.h"
#include "model/profile.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "PROFILE_REGISTER" Event
    */
    class ProfileRegister
            : public SproofEvent {
    public:
        ProfileRegister(Profile document);

        virtual ~ProfileRegister() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        Profile m_profile;
    };

}

#endif //SPROOF_CLIENT_PROFILE_REGISTER_H
