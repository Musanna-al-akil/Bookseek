#ifndef USER_H
#define USER_H

#include <string>

struct User {
    int id = 0;
    std::string username;
    std::string passwordHash;
};

#endif // USER_H 