#include <iostream>
#include <unordered_map>
#include <functional>
#include <thread>

#include <jsoncpp/json/json.h>

#include "sproof.h"
#include "http/http-client.h"
#include "http/http-error.h"
#include "json-exception.h"

int main() {

    std::function<void(Json::Value)> result_callback = [](Json::Value result){
        std::cout << result.toStyledString() << std::endl;
    };

    std::function<void(sproof::HttpException)> error_callback = [](sproof::HttpException ex){
        std::cout << ex.what() << std::endl;
    };

    sproof::Sproof sproof{"/home/user/sproof/config.json"};

//    sproof.revoke_profile("somereason");
//    sproof.revoke_profile("anotherreason");
//    std::thread t = sproof.commit(result_callback, error_callback);
//    t.join();

//    auto th = sproof.commit(result_callback, error_callback);
//    th.join();

    auto th = sproof.get_user(result_callback, error_callback);
    th.join();

    th = sproof.get_state(result_callback, error_callback);
    th.join();

    std::unordered_map<std::string,std::string> params;
    th = sproof.get_events(params, result_callback, error_callback);
    th.join();

}