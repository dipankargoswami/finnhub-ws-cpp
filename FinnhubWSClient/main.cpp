//
//  main.cpp
//  FinnhubWSClient
//
//  Created by Dipankar Goswami on 07/10/20.
//

//System Headers
#include <iostream>

//3rd Party lib Headers
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"

//App Headers
#include "FinnHubWSSession.hpp"

int main(int argc, const char * argv[]) {
    
    auto logger = spdlog::daily_logger_mt("daily_logger", "./daily.txt", 2, 30);
    logger->info("===== Starting Application Initialization =====");

    FinnHubWSSession finnhubSession(argv[1]);
    finnhubSession.subscribe();
    
    finnhubSession.run_loop();
    
    return 0;
}
