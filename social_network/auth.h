#ifndef _social_network_auth_h_
#define _social_network_auth_h_

#include <Core/Core.h>
#include <jwt-cpp/jwt.h>

using namespace Upp;

struct AuthUser {
    int user_id = 0;
};

class AuthService {
public:
    AuthService(const String& secret, const String& issuer) : secret(secret), issuer(issuer) {}

    String generate_token(int user_id) const;
    bool validate_token(const String& token, AuthUser& out_user, String& error) const;

private:
    String secret;
    String issuer;
};

const AuthService& GetAuthService();

#endif
