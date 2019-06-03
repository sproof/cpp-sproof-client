
#ifndef SPROOF_CLIENT_DOCUMENT_RECEIVER_ADD_H
#define SPROOF_CLIENT_DOCUMENT_RECEIVER_ADD_H


#include "model/events/sproof-event.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "DOCUMENT_RECEIVER_ADD" Event
    */
    class DocumentReceiverAdd
            : public SproofEvent {
    public:
        DocumentReceiverAdd(const std::string &receiver_id, const std::string &document_hash);

        virtual ~DocumentReceiverAdd() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        std::string m_receiver_id;
        std::string m_document_hash;
    };

}

#endif //SPROOF_CLIENT_DOCUMENT_RECEIVER_ADD_H
