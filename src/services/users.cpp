#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

struct User {
    int id;
    char username[64];
    char password[64];
    time_t created_at;
};

class UserService {
    private:
        const string filename;

    public:
        UserService(const string& fname) : filename(fname) {
            // Check if file exists
            ofstream checkFile(filename, ios::app | ios::binary);
            checkFile.close();
        }
        

        bool addUser(string username, string password) {
            // Check if username already exists
            try {
                getUserByUsername(username.c_str());
                return false;  
            } catch (const runtime_error& e) {
                // Username doesn't exist
            }
            
         
            int maxId = 0;
            fstream file(filename, ios::in | ios::binary);
            if (file) {
                User tempUser;
                while (file.read(reinterpret_cast<char*>(&tempUser), sizeof(User))) {
                    if (tempUser.id > maxId) {
                        maxId = tempUser.id;
                    }
                }
                file.close();
            }
            
            User user;
            user.id = maxId + 1;
            strcpy(user.username, username.c_str());
            strcpy(user.password, password.c_str());
            user.created_at = time(nullptr);
            
            // Write to file
            ofstream outFile(filename, ios::app | ios::binary);
            if (!outFile) {
                cerr << "Failed to open file for writing: " << filename << endl;
                return false;
            }
            
            outFile.write(reinterpret_cast<const char*>(&user), sizeof(User));
            outFile.close();
            return true;
        }
        
        User getUserByUsername(const char* username) {
            ifstream file(filename, ios::binary);
            if (!file) {
                throw runtime_error("Could not open file");
            }
            
            User user;
            while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
                if (strcmp(user.username, username) == 0) {
                    file.close();
                    return user;
                }
            }
            
            file.close();
            throw runtime_error("User not found");
        }
       
        bool authenticateUser(string username, string password) {
            ifstream file(filename, ios::binary);
            if (!file) {
                return false;
            }
            
            User user;
            while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
                if (strcmp(user.username, username.c_str()) == 0 && 
                    strcmp(user.password, password.c_str()) == 0) {
                    file.close();
                    return true;
                }
            }
            
            file.close();
            return false;
        }
};