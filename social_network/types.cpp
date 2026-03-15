#include <Core/Core.h>
#include "types.h"

using namespace Upp;

CreateUser::CreateUser(
    String _first_name,
    String _last_name,
    Date   _birthday,
    String _gender,
    String _interests,
   	String _city,
   	String _password
) {
	first_name = _first_name;
	last_name  = _last_name;
	birthday   = _birthday;
	gender     = _gender;
	interests  = _interests;
	city       = _city;
	password   = _password;
}

ValueMap ToValueMap(const CreateUser& u)
{
    ValueMap m;
    m("first_name", u.first_name);
    m("last_name",  u.last_name);
    m("birthday",   u.birthday);
    m("gender",     u.gender);
    m("interests",  u.interests);
    m("city",       u.city);
    return m;
}
