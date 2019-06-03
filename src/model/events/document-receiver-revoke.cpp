
#include "model/events/document-receiver-revoke.h"

namespace sproof {

    DocumentReceiverRevoke::DocumentReceiverRevoke(const std::string &receiver_id, const std::string &reason)
            : m_receiver_id{receiver_id}, m_reason{reason} {
    }

    Json::Value DocumentReceiverRevoke::get_data() const {
        Json::Value result;
        result["reason"] = m_reason;
        result["receiverId"] = m_receiver_id;
        return result;
    }

    std::string DocumentReceiverRevoke::get_event_name() const {
        return "DOCUMENT_RECEIVER_REVOKE";
    }

}
