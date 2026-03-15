#include <social_network/social_network.h>
#include <Core/Core.h>
#include "types.h"
#include "repositories.h"
#include "services.h"
#include "auth.h"
#include "helpers.h"


void OpenSQL(PostgreSQLSession& session);


template <class Handler>
void ProtectedEndpoint(Http& http, const AuthService& auth, Handler handler) {
    AuthUser auth_user;

    if(!AuthorizeRequest(http, auth, auth_user))
        return;

    handler(auth_user);
}


SKYLARK(Register, "user/register:POST") {
    PostgreSQLSession session;
    OpenSQL(session);

    UserRepositoryImpl repo(session);
    UserServiceImpl service(repo);

    Value body = ParseJSON(http.GetRequestContent());

    String first_name        = body["first_name"];
    String last_name         = body["last_name"];
    String string_birthday   = body["birthday"];
    String gender            = body["gender"];
    String interests         = body["interests"];
    String city              = body["city"];
    String password          = body["password"];

    if(first_name.IsEmpty() || last_name.IsEmpty()) {
        http.Response(400, "first name or last name is empty");
        http.ContentType("application/json; charset=utf-8");
        ValueMap m = {
            {"message", "first name or last name is empty"},
            {"code", "first_name_or_last_name_is_empty"}
        };
        http << AsJSON(m);
        return;
    }

	
    Vector<String> p = Split(string_birthday, '-');
    if(p.GetCount() != 3) {
        http.Response(400, "invalid birthday format");
        http.ContentType("application/json; charset=utf-8");
        ValueMap m = {
            {"message", "invalid birthday format, expected YYYY-MM-DD"},
            {"code", "invalid_birthday_format"}
        };
        http << AsJSON(m);
        return;
    }

    Date birthday(ScanInt(p[0]), ScanInt(p[1]), ScanInt(p[2]));

    CreateUser user{
        first_name,
        last_name,
        birthday,
        gender,
        interests,
        city,
        password
    };

    Value created_user = service.register_user(user);

    http.ContentType("application/json; charset=utf-8");
    http << AsJSON(created_user);
}


SKYLARK (GetUser, "user/get/*") {
	PostgreSQLSession session;
	OpenSQL(session);
	
	UserRepositoryImpl repo(session);
	UserServiceImpl service(repo);
	
	const AuthService& auth = GetAuthService();

	ProtectedEndpoint(http, auth, [&](const AuthUser& auth_user) {
		
		int user_id = ScanInt64(http[0]);
		Value user = service.get_user(user_id);
		
		http << AsJSON(user);
	});
}


SKYLARK(Login, "login:POST") {
    PostgreSQLSession session;
    OpenSQL(session);

    UserRepositoryImpl repo(session);
    UserServiceImpl service(repo);
	
	const AuthService& auth = GetAuthService();
    
    Value body = ParseJSON(http.GetRequestContent());
    
    String user_id_str = body["user_id"];
	
	int user_id = ScanInt64(user_id_str);
    String password = body["password"];

    Value login_result = service.login_user(user_id, password);
    if(IsError(login_result)) {
        http.Response(401, "invalid credentials");
        http.ContentType("application/json; charset=utf-8");
        
        ValueMap m = {
            {"message", "invalid credentials"},
            {"code", "invalid_credentials"}
        };
        
        http << AsJSON(login_result);
        return;
    }

    String token = auth.generate_token(user_id);

    ValueMap result;
    result.Add("token", token);
    result.Add("user_id", user_id);

    http.ContentType("application/json; charset=utf-8");
    http << AsJSON(result);
}