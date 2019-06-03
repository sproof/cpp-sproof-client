
#ifndef SPROOF_CLIENT_SPROOF_EVENT_H
#define SPROOF_CLIENT_SPROOF_EVENT_H

#include <string>

#include <jsoncpp/json/json.h>

namespace sproof {

    /**
    * Basis-Interface fuer Event-Klassen
    */
    class SproofEvent {
    public:

        Json::Value to_json() const;

        virtual Json::Value get_data() const = 0;

        virtual std::string get_event_name() const = 0;

    };

}

#endif //SPROOF_CLIENT_SPROOF_EVENT_H
