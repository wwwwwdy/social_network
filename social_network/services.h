#ifndef _social_network_services_h_
#define _social_network_services_h_

#include "types.h"
#include "repositories.h"
#include <bcrypt.h>

struct UserService {
	virtual ~UserService() = default;
	
	virtual Value register_user(const CreateUser& user) = 0;
	virtual Value get_user(int id) = 0;
	virtual Value login_user(int user_id, String password) = 0;
};

class UserServiceImpl : public UserService {
public:
	UserServiceImpl() = delete;
	UserServiceImpl(UserRepositoryImpl& repo);
	
	Value register_user(const CreateUser& user) override;
	Value get_user(int id) override;
	Value login_user(int user_id, String password) override;

private:
	UserRepositoryImpl& repo;
};

#endif
