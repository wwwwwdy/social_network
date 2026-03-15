#ifndef _social_network_repositories_h_
#define _social_network_repositories_h_

#include <PostgreSQL/PostgreSQL.h>

#include "types.h"

using namespace Upp;

void OpenSQL(PostgreSQLSession& session);
void InitModel();

struct UserRepository {
	virtual ~UserRepository() = default;
	
	virtual Value create(const CreateUser& user) = 0;
	virtual Value get(int user_id) = 0;
	virtual Value get_password(int user_id) = 0;
};


class UserRepositoryImpl : public UserRepository {
public:
	UserRepositoryImpl() = delete;
	UserRepositoryImpl(PostgreSQLSession& s);
	
	Value create(const CreateUser& user);
	Value get(int user_id);
	Value get_password(int user_id);

private:
	PostgreSQLSession& session;
};

#endif