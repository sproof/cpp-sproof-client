
#ifndef SPROOF_CLIENT_DOCUMENT_RECEIVER_REVOKE_H
#define SPROOF_CLIENT_DOCUMENT_RECEIVER_REVOKE_H

#include "model/events/sproof-event.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "DOCUMENT_RECEIVER_REVOKE" Event
    */
    class DocumentReceiverRevoke
            : public SproofEvent {
    public:
        DocumentReceiverRevoke(const std::string &receiver_id, const std::string &reason);

        virtual ~DocumentReceiverRevoke() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        std::string m_receiver_id;
        std::string m_reason;
    };

}

#endif //SPROOF_CLIENT_DOCUMENT_RECEIVER_REVOKE_H
