
#ifndef SPROOF_CLIENT_DOCUMENT_H
#define SPROOF_CLIENT_DOCUMENT_H

#include <string>

#include "model/sproof-data.h"

namespace sproof {

    /**
    * Model-Klasse fuer ein Sproof Dokument
    */
    struct Document
            : public SproofData {
        Json::Value to_json() const override;

        unsigned long valid_from;
        unsigned long valid_until;
        std::string document_hash;
        std::string data;
        std::string location_hash;
        std::string name;
    };

}

#endif //SPROOF_CLIENT_DOCUMENT_H
