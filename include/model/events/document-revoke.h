
#ifndef SPROOF_CLIENT_DOCUMENT_REVOKE_H
#define SPROOF_CLIENT_DOCUMENT_REVOKE_H

#include "model/events/sproof-event.h"

namespace sproof {

    /**
    * Model Klasse fuer ein "DOCUMENT_REVOKE" Event
    */
    class DocumentRevoke
            : public SproofEvent {
    public:
        DocumentRevoke(const std::string &document_hash, const std::string &reason);

        virtual ~DocumentRevoke() = default;

        Json::Value get_data() const override;

        std::string get_event_name() const override;

    private:
        std::string m_document_hash;
        std::string m_reason;
    };

}

#endif //SPROOF_CLIENT_DOCUMENT_REVOKE_H
