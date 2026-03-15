#ifndef _social_network_types_h_
#define _social_network_types_h_

#include <Core/Core.h>

using namespace Upp;

struct CreateUser {
    String first_name;
    String last_name;
    Date   birthday;
    String gender;
    String interests;
    String city;
    String password;
    
    CreateUser() = delete;
    CreateUser(
    	String _first_name,
    	String _last_name,
    	Date _birthday,
    	String _gender,
    	String _interests,
    	String _city,
    	String _password
    );
};

ValueMap ToValueMap(const CreateUser& u);

#endif
