#ifndef _social_network_social_network_h
#define _social_network_social_network_h

#include <Skylark/Skylark.h>


using namespace Upp;


class social_network : public SkylarkApp {
public:
	virtual void WorkThread();

	social_network();
};

#endif
