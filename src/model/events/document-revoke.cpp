

#include "model/events/document-revoke.h"

namespace sproof {

    DocumentRevoke::DocumentRevoke(const std::string &document_hash, const std::string &reason)
            : m_document_hash{document_hash}, m_reason{reason} {
    }

    Json::Value DocumentRevoke::get_data() const {
        Json::Value result;
        result["reason"] = m_reason;
        result["documentHash"] = m_document_hash;
        return result;
    }

    std::string DocumentRevoke::get_event_name() const {
        return "DOCUMENT_REVOKE";
    }

}
