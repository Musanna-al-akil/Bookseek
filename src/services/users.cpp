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
            ifstream checkFile(filename, ios::binary);
            if (checkFile) {
                User existingUser;
                while (checkFile.read((char*)&existingUser, sizeof(User))) {
                    if (strcmp(existingUser.username, username.c_str()) == 0) {
                        checkFile.close();
                        return false;  
                    }
                }
                checkFile.close();
            }
            
            // next id
            int maxId = 0;
            ifstream idFile(filename, ios::binary);
            if (idFile) {
                User tempUser;
                while (idFile.read((char*)&tempUser, sizeof(User))) {
                    if (tempUser.id > maxId) {
                        maxId = tempUser.id;
                    }
                }
                idFile.close();
            }
            
            // new user
            User user;
            user.id = maxId + 1;
            strcpy(user.username, username.c_str());
            strcpy(user.password, password.c_str());
            user.created_at = time(nullptr);

            ofstream outFile(filename, ios::app | ios::binary);
            if (!outFile) {
                cout << "Failed to open file for writing: " << filename << endl;
                return false;
            }
            
            outFile.write((char*)&user, sizeof(User));
            outFile.close();
            return true;
        }
        
        User getUserByUsername(const char* username) {
            ifstream file(filename, ios::binary);
            if (!file) {
                throw runtime_error("Could not open file");
            }
            
            User user;
            bool found = false;
            
            while (file.read((char*)&user, sizeof(User))) {
                if (strcmp(user.username, username) == 0) {
                    found = true;
                    break;
                }
            }
            
            file.close();
            
            if (!found) {
                throw runtime_error("User not found");
            }
            
            return user;
        }
       
        bool authenticateUser(string username, string password) {
            ifstream file(filename, ios::binary);
            if (!file) {
                cout << "Could not open users file." << endl;
                return false;
            }
            
            User user;
            bool found = false;
            
            while (file.read((char*)&user, sizeof(User))) {
                if (strcmp(user.username, username.c_str()) == 0 && 
                    strcmp(user.password, password.c_str()) == 0) {
                    found = true;
                    break;
                }
            }
            
            file.close();
            return found;
        }
};