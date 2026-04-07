#include "services.h"
#include <jwt-cpp/jwt.h>
#include <Core/Core.h>

UserServiceImpl::UserServiceImpl(UserRepositoryImpl& repo) : repo(repo) {}

Value UserServiceImpl::register_user(const CreateUser& user) {
    if(user.password.IsEmpty())
        return ErrorValue("password is empty");

    CreateUser copy = user;

    std::string hashed = bcrypt::generateHash(~user.password);

    copy.password = hashed;

    return repo.create(copy);
}


Value UserServiceImpl::get_user(int id) {
	return repo.get(id);
}


Value UserServiceImpl::login_user(int user_id, String password) {
	Cout() << "User ID is ---->>>" << user_id;
    Value login_info = repo.get_password(user_id);

    if(IsError(login_info))
        return ErrorValue("user not found");

    String stored_hash = login_info["password"];

    bool ok = bcrypt::validatePassword(~password, ~stored_hash);
    if(!ok)
        return ErrorValue("invalid password");

    return login_info;
}

Value UserServiceImpl::search_users(const String& first_name, const String& last_name) {
    return repo.search(first_name, last_name);
}
