#include "repositories.h"

#define MODEL <social_network/Model.sch>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#include <Sql/sch_header.h>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#include <Sql/sch_source.h>

#define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>
#include <Sql/sch_schema.h>

#include "config.h"

using namespace Upp;

void OpenSQL(PostgreSQLSession& session)
{
    const AppConfig& cfg = GetConfig();

    String conn =
        "host=" + cfg.db_host +
        " dbname=" + cfg.db_name +
        " user=" + cfg.db_user +
        " password=" + cfg.db_password;

    if(!session.Open(conn)) {
        Cerr() << "DB connection failed\n";
        Cerr() << "conn: " << conn << "\n";
        Cerr() << "last error: " << session.GetLastError().ToString() << "\n";
        Exit(1);
    }
#ifdef _DEBUG
	session.LogErrors();
	session.SetTrace();
#endif
	SQL = session;
}

void InitModel()
{
#ifdef _DEBUG
	PostgreSQLSession session;
	OpenSQL(session);
	SqlSchema sch(PGSQL);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	sch.SaveNormal();
#endif
}

UserRepositoryImpl::UserRepositoryImpl(PostgreSQLSession& s) : session(s) {}

Value UserRepositoryImpl::create(const CreateUser& user) 
{
    Sql sql(session);

    sql * Insert(users)
            (first_name, user.first_name)
            (last_name,  user.last_name)
            (birthday,   user.birthday)
            (gender,     user.gender)
            (interests,  user.interests)
            (city,       user.city)
            (password,   user.password).Returning(id);

    if(sql.WasError())
        return ErrorValue("insert failed");

	if(!sql.Fetch())
        return ErrorValue("insert ok but RETURNING returned no row");
    
    Value new_id = sql[0];
    
    if(IsNull(new_id))
        return ErrorValue("cannot get inserted id");
	

    sql * Select(id, first_name, last_name, birthday, gender, interests, city)
            .From(users)
            .Where(id == new_id);

    if(!sql.Fetch() || sql.WasError())
        return ErrorValue("select after insert failed");

	Date b = (Date)sql[birthday];
	
    ValueMap out;
    out("id",         (int64)sql[id]);
    out("first_name", (String)sql[first_name]);
    out("last_name",  (String)sql[last_name]);
	out("birthday",   Format("%04d-%02d-%02d", b.year, b.month, b.day));
    out("gender",     (String)sql[gender]);
    out("interests",  (String)sql[interests]);
    out("city",       (String)sql[city]);
    return out;
}

Value UserRepositoryImpl::get(int user_id) {
	Sql sql(session);
	
	sql * Select(id, first_name, last_name, birthday, gender, interests, city)
          .From(users)
          .Where(id == user_id);
    
    if(!sql.Fetch() || sql.WasError())
        return ErrorValue("select after insert failed");

	Date b = (Date)sql[birthday];

    ValueMap out;
    out("id",         (int64)sql[id]);
    out("first_name", (String)sql[first_name]);
    out("last_name",  (String)sql[last_name]);
	out("birthday",   Format("%04d-%02d-%02d", b.year, b.month, b.day));
    out("gender",     (String)sql[gender]);
    out("interests",  (String)sql[interests]);
    out("city",       (String)sql[city]);
    return out;
}

Value UserRepositoryImpl::get_password(int user_id) {
	Sql sql(session);
	
	sql * Select(id, password)
          .From(users)
          .Where(id == user_id);
    
    if(!sql.Fetch() || sql.WasError())
        return ErrorValue("select after insert failed");

    ValueMap out;
    out("id",         (int64)sql[id]);
    out("password",   (String)sql[password]);
    
    return out;
}