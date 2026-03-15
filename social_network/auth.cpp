#include "auth.h"
#include "config.h"
#include <chrono>

String AuthService::generate_token(int user_id) const {
	auto now = std::chrono::system_clock::now();
    auto exp = now + std::chrono::minutes{30};
	
    std::string token = jwt::create()
        .set_issuer(issuer.ToStd())
        .set_type("JWT")
        .set_subject(std::to_string(user_id))
        .set_issued_at(now)
        .set_expires_at(exp)
        .sign(jwt::algorithm::hs256{secret.ToStd()});

    return String(token.c_str());
}

bool AuthService::validate_token(const String& token, AuthUser& out_user, String& error) const {
    try {
        auto decoded = jwt::decode(token.ToStd());

        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret.ToStd()})
            .with_issuer(issuer.ToStd());

        verifier.verify(decoded);

        std::string subject = decoded.get_subject();
        if(subject.empty()) {
            error = "sub claim is missing";
            return false;
        }

        out_user.user_id = atoi(subject.c_str());
        if(out_user.user_id <= 0) {
            error = "invalid sub claim";
            return false;
        }

        return true;
    }
    catch(const std::exception& e) {
        error = e.what();
        return false;
    }
}

const AuthService& GetAuthService() {
    static AuthService auth(GetConfig().jwt_secret, GetConfig().jwt_issuer);
    return auth;
}
