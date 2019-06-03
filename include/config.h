
#ifndef SPROOF_CLIENT_CONFIG_H
#define SPROOF_CLIENT_CONFIG_H


#include <string>

#include <sproof-utils/credentials.h>

namespace sproof {

    /**
     * Containerklasse fuer eine Sproof Konfiguration
     */
    struct Config {

        Config(std::string  sproof_url, const sproof::Credentials& sproof_credentials);

        std::string url;
        sproof::Credentials credentials;

    };

}


#endif //SPROOF_CLIENT_CONFIG_H
