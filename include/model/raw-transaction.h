
#ifndef SPROOF_CLIENT_RAW_TRANSACTION_H
#define SPROOF_CLIENT_RAW_TRANSACTION_H

#include <jsoncpp/json/json.h>


class RawTransaction {

/**
 * Container-Klasse fuer die Daten eines getRawTransaction() Requests
 */
public:
    static RawTransaction from_json(const Json::Value& transaction);
};


#endif //SPROOF_CLIENT_RAW_TRANSACTION_H
