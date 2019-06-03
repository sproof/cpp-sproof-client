cmake_minimum_required(VERSION 3.13)
project(sproof_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} " )

include( CTest )
add_subdirectory( tests )

include_directories("${PROJECT_SOURCE_DIR}/include")

set(sources ${sources}
        src/sproof.cpp
        src/http/http-client.cpp src/http/http-error.cpp src/model/raw-transaction.cpp include/model/raw-transaction.h include/model/sproof-data.h include/model/profile.h src/model/profile.cpp include/model/document.h src/model/document.cpp include/model/events/sproof-event.h src/model/events/sproof-event.cpp include/model/events/document-receiver-add.h src/model/events/document-receiver-add.cpp include/model/events/document-receiver-revoke.h src/model/events/document-receiver-revoke.cpp include/model/events/document-register.h src/model/events/document-register.cpp include/model/events/document-revoke.h src/model/events/document-revoke.cpp include/model/events/profile-confirm.h src/model/events/profile-confirm.cpp include/model/events/profile-register.h src/model/events/profile-register.cpp include/model/events/profile-revoke.h src/model/events/profile-revoke.cpp include/model/events/profile-update.h src/model/events/profile-update.cpp src/base-exception.cpp include/base-exception.h src/json-exception.cpp include/json-exception.h include/http/sproof-api.h src/config.cpp include/config.h src/http/sproof-api.cpp)

set(libraries
        boost_system
        pthread
        ssl
        crypto
        curl
        jsoncpp
        sproof-utils
        cryptopp
        bitcoin-system
        sodium)

add_executable(sproof_client src/main.cpp ${sources})

target_link_libraries (sproof_client ${libraries})