
#ifndef SPROOF_CLIENT_DOCUMENT_REGISTER_H
#define SPROOF_CLIENT_DOCUMENT_REGISTER_H


#include "model/events/sproof-event.h"
#include "model/document.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "DOCUMENT_REGISTER" Event
    */
    class DocumentRegister
            : public SproofEvent {
    public:
        DocumentRegister(Document document);

        virtual ~DocumentRegister() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        Document m_document;
    };

}

#endif //SPROOF_CLIENT_DOCUMENT_REGISTER_H
