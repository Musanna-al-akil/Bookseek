#include "database.h"
#include <iostream>
#include <string>

Database::Database(const std::string& path) : dbPath(path), db(nullptr) {}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::initialize() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    // The actual schema creation is done separately in the schema.sql file
    // This function just checks if we can connect to the database
    return true;
}

bool Database::registerUser(const std::string& username, const std::string& password) {
    // Simple implementation without proper hashing for demo purposes
    // In a real application, password should be properly hashed
    
    // First check if user already exists
    sqlite3_stmt* stmt;
    std::string sql = "SELECT COUNT(*) FROM users WHERE username = ?";
    
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    int count = 0;
    if (rc == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    
    if (count > 0) {
        std::cout << "Username already exists." << std::endl;
        return false;
    }
    
    // Insert new user
    sql = "INSERT INTO users (username, password) VALUES (?, ?)";
    
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool Database::loginUser(const std::string& username, const std::string& password, User& user) {
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, username, password FROM users WHERE username = ?";
    
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        std::string storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        
        if (password == storedPassword) {
            user.id = sqlite3_column_int(stmt, 0);
            user.username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            user.passwordHash = storedPassword;
            
            sqlite3_finalize(stmt);
            return true;
        }
    }
    
    sqlite3_finalize(stmt);
    return false;
}

