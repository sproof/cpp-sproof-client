

#ifndef SPROOF_CLIENT_PROFILE_UPDATE_H
#define SPROOF_CLIENT_PROFILE_UPDATE_H

#include "model/events/sproof-event.h"
#include "model/profile.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "PROFILE_UPDATE" Event
    */
    class ProfileUpdate
            : public SproofEvent {
    public:
        ProfileUpdate(Profile document);

        virtual ~ProfileUpdate() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        Profile m_profile;
    };

}

#endif //SPROOF_CLIENT_PROFILE_UPDATE_H
