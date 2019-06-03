

#include "model/events/sproof-event.h"

namespace sproof {


    Json::Value SproofEvent::to_json() const {
        Json::Value result;
        result["eventType"] = get_event_name();
        result["data"] = get_data();
        return result;
    }

}
