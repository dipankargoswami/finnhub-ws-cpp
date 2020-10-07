//
//  main.cpp
//  FinnhubWSClient
//
//  Created by Dipankar Goswami on 07/10/20.
//

#include <iostream>
#include "FinnHubWSSession.hpp"

int main(int argc, const char * argv[]) {
    
    FinnHubWSSession finnhubSession(argv[1]);
    finnhubSession.subscribe();
    
    finnhubSession.run_loop();
    
    return 0;
}
