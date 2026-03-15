#include "social_network.h"
#include "repositories.h"

#include "config.h"

social_network::social_network()
{
	root = "";
#ifdef _DEBUG
	prefork = 0;
	use_caching = false;
#endif
    const AppConfig& cfg = GetConfig();
    port = cfg.port;
}


void social_network::WorkThread()
{
	PostgreSQLSession session;
	OpenSQL(session);
	InitModel();
	RunThread();
}

// Local server URL: 127.0.0.1:8001/social_network
CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	StdLogSetup(LOG_FILE|LOG_COUT);
	Ini::skylark_log = true;
#endif
	social_network().Run();
}
