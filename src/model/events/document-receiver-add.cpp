
#include "model/events/document-receiver-add.h"

namespace sproof {

    DocumentReceiverAdd::DocumentReceiverAdd(const std::string &receiver_id, const std::string &document_hash)
            : m_receiver_id{receiver_id}, m_document_hash{document_hash} {
    }

    Json::Value DocumentReceiverAdd::get_data() const {
        Json::Value result;
        result["documentHash"] = m_document_hash;
        result["receiverId"] = m_receiver_id;
        return result;
    }

    std::string DocumentReceiverAdd::get_event_name() const {
        return "DOCUMENT_RECEIVER_ADD";
    }

}
