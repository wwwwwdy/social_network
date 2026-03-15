#ifndef _social_network_helpers_h_
#define _social_network_helpers_h_

#include <Core/Core.h>
#include <Skylark/Skylark.h>
#include "auth.h"


inline bool AuthorizeRequest(Http& http, const AuthService& auth, AuthUser& auth_user) {
    String header = http.GetHeader("authorization");

    if(header.IsEmpty()) {
        http.Response(401, "missing Authorization header");
        http.ContentType("application/json; charset=utf-8");
        http << "{\"message\":\"missing Authorization header\",\"code\":\"missing_authorization_header\"}";
        return false;
    }

    const String prefix = "Bearer ";
    if(!header.StartsWith(prefix)) {
        http.Response(401, "invalid Authorization header");
        http.ContentType("application/json; charset=utf-8");
        http << "{\"message\":\"invalid Authorization header\",\"code\":\"invalid_authorization_header\"}";
        return false;
    }

    String token = header.Mid(prefix.GetCount());

    String error;
    if(!auth.validate_token(token, auth_user, error)) {
        http.Response(401, "invalid token");
        http.ContentType("application/json; charset=utf-8");
        http << "{\"message\":\"invalid token\",\"code\":\"invalid_token\"}";
        return false;
    }

    return true;
}

#endif
