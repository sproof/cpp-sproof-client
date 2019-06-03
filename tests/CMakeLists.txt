
configure_file("config.h.in" ${CMAKE_SOURCE_DIR}/tests/config.h )

set( test_executables
        test_sproof)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(test_sources
        ${PROJECT_SOURCE_DIR}/src/sproof.cpp
        ${PROJECT_SOURCE_DIR}/src/http/http-client.cpp
        ${PROJECT_SOURCE_DIR}/src/http/http-error.cpp
        ${PROJECT_SOURCE_DIR}/src/model/raw-transaction.cpp
        ${PROJECT_SOURCE_DIR}/src/model/profile.cpp
        ${PROJECT_SOURCE_DIR}/src/model/document.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/sproof-event.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/document-receiver-add.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/document-receiver-revoke.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/document-register.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/document-revoke.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/profile-confirm.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/profile-register.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/profile-revoke.cpp
        ${PROJECT_SOURCE_DIR}/src/model/events/profile-update.cpp
        ${PROJECT_SOURCE_DIR}/src/base-exception.cpp
        ${PROJECT_SOURCE_DIR}/src/json-exception.cpp
        ${PROJECT_SOURCE_DIR}/src/config.cpp
        ${PROJECT_SOURCE_DIR}/src/http/sproof-api.cpp)

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


FOREACH( test_name IN ITEMS ${test_executables} )

    add_executable( ${test_name} ${test_name}.cpp ${test_sources} )

    target_link_libraries( ${test_name} ${libraries} )

    add_test( ${test_name}_test ${test_name} )

ENDFOREACH()



