#ifndef _social_network_config_h_
#define _social_network_config_h_

#include <Core/Core.h>
#include <cstdlib>

using namespace Upp;

struct AppConfig {
    String db_host;
    String db_name;
    String db_user;
    String db_password;

    int port;
    String jwt_secret;
    String jwt_issuer;    
};

AppConfig LoadConfig();
const AppConfig& GetConfig();

#endif
