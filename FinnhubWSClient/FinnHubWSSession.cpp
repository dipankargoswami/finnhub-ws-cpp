//
//  FinnHubWSSession.cpp
//  FinnhubWSClient
//
//  Created by Dipankar Goswami on 07/10/20.
//

#include <boost/program_options.hpp>
#include <json/json.h>
#include <json/reader.h>

#include "FinnHubWSSession.hpp"

namespace po = boost::program_options;

using namespace web::websockets::client;

FinnHubWSSession::FinnHubWSSession(std::string fileName) {
    po::options_description config("Configuration");
    config.add_options()
        ("API_Token", po::value<std::string>()->default_value(""), "API Key assigned by Finnhub");
    config.add_options()
        ("Symbol_List", po::value<std::vector<std::string> >()->multitoken(), "List of Sysmbols to be subscribed");
    
    boost::program_options::variables_map vm;

    std::ifstream ifs(fileName);
    if (ifs) {
        boost::program_options::store(boost::program_options::parse_config_file(ifs, config), vm);
        boost::program_options::notify(vm);
    }
    
    token_ = vm["API_Token"].as<std::string>();
    
    for(auto symbol:vm["Symbol_List"].as<std::vector<std::string> >()) {
        std::cout << "Subscribing to the symbol [" << symbol << ']' << std::endl;
        subscriptions_.push_back("{\"type\":\"subscribe\",\"symbol\":\"" + symbol +"\"}");
    }
}

void FinnHubWSSession::subscribe() {
    finnhubWSClient_.connect(U("wss://ws.finnhub.io/?token=" + token_)).wait();
    
    for(auto subscription:subscriptions_) {
        websocket_outgoing_message msg;
        msg.set_utf8_message(subscription);
        finnhubWSClient_.send(msg).wait();
    }
}

void FinnHubWSSession::run_loop() {
    auto next_update_time = 0L;
    while(true) {
        if(time(NULL) < next_update_time) {
            continue;
        }
        next_update_time = time(NULL) + 5;
        RequestJSONValueAsync().wait();
    }
}

pplx::task<void> FinnHubWSSession::RequestJSONValueAsync()
{
    std::string body_str;
    
    return finnhubWSClient_.receive().then([body_str](websocket_incoming_message ret_msg) {
        auto msg = ret_msg.extract_string().get();

        std::cout << msg << std::endl; //Logging raw message is mandated by finnhub

        JSONCPP_STRING err;
        Json::Value root;
        Json::CharReaderBuilder builder;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        const auto rawJsonLength = static_cast<int>(msg.length());

        if (reader->parse(msg.c_str(), msg.c_str() + rawJsonLength, &root, &err))
        {
          Json::Value::iterator itr = root["data"].begin();
          while(itr != root["data"].end()) {
            std::cout << "For Product [" << (*itr)["s"].asString() << "] - LTP : [" << (*itr)["p"].asFloat() << "] and VOL [" << (*itr)["v"].asInt() << ']' << std::endl;
            itr++;
          }
        }
      });
}
