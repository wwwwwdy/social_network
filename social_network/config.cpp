#include "config.h"

AppConfig LoadConfig() {
    AppConfig cfg;

    const char* db_host = std::getenv("DB_HOST");
    const char* db_name = std::getenv("DB_NAME");
    const char* db_user = std::getenv("DB_USER");
    const char* db_password = std::getenv("DB_PASSWORD");
    const char* port = std::getenv("APP_PORT");
    const char* jwt_secret = std::getenv("JWT_SECRET");
    const char* jwt_issuer = std::getenv("JWT_ISSUER");

    cfg.db_host = db_host ? db_host : "localhost";
    cfg.db_name = db_name ? db_name : "social_network";
    cfg.db_user = db_user ? db_user : "postgres";
    cfg.db_password = db_password ? db_password : "";
    cfg.port = port ? atoi(port) : 8001;
    cfg.jwt_secret = jwt_secret ? jwt_secret : "secret";
	cfg.jwt_issuer = jwt_issuer ? jwt_issuer : "auth_service";

    return cfg;
}


const AppConfig& GetConfig() {
    static AppConfig cfg = LoadConfig();
    return cfg;
}
