#include <utility>



#include "model/events/document-register.h"


namespace sproof {

    DocumentRegister::DocumentRegister(Document document)
            : m_document{std::move(document)} {
    }

    Json::Value DocumentRegister::get_data() const {
        return m_document.to_json();
    }

    std::string DocumentRegister::get_event_name() const {
        return "DOCUMENT_REGISTER";
    }

}
