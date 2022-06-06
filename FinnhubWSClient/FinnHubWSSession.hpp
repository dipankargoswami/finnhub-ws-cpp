//
//  FinnHubWSSession.hpp
//  FinnhubWSClient
//
//  Created by Dipankar Goswami on 07/10/20.
//

#ifndef FinnHubWSSession_hpp
#define FinnHubWSSession_hpp

#include <cpprest/ws_client.h>

class FinnHubWSSession {

public:
    FinnHubWSSession(std::string fileName);

    void subscribe();
    void run_loop();

private:
    pplx::task<void> RequestJSONValueAsync();

    std::shared_ptr<spdlog::logger> logger_;
    web::websockets::client::websocket_client finnhubWSClient_;
    std::string token_;
    std::vector<std::string> subscriptions_;
};



#endif /* FinnHubWSSession_hpp */
