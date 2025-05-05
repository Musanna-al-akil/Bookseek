#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "user.h"

class Database {
private:
    sqlite3* db;
    std::string dbPath;

public:
    Database(const std::string& path);
    ~Database();

    bool initialize();
    
    // User management
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password, User& user);
    
};

#endif // DATABASE_H 