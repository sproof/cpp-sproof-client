#include <utility>



#include "config.h"

sproof::Config::Config(std::string sproof_url, const sproof::Credentials &sproof_credentials)
    : url{std::move(sproof_url)}, credentials{sproof_credentials}
{
}
