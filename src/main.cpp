#include <iostream>
#include <string>
#include "user.h"
#include "database.h"

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void displayMenu() {
    clearScreen();
    cout << "\n===== BookSeeks: Personal Books Management System =====\n";
    cout << "1. Register new user\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

// Display menu for logged-in users
void displayLoggedInMenu() {
    clearScreen();
    cout << "\n===== BookSeeks: User Menu =====\n";
    cout << "1. Add new book\n";
    cout << "2. View all books\n";
    cout << "3. Search for a book\n";
    cout << "4. Logout\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}



int main() {
    Database db("bookseeks.db");
    if (!db.initialize()) {
        cerr << "Failed to initialize database.\n";
        return 1;
    }

    int choice = 0;
    string username, password, title, author;
    User currentUser;
    bool loggedIn = false;
    // Book newBook;

    clearScreen();

    cout << "~~~~~~~   Welcome to BookSeeks!   ~~~~~~~\n";

    while (true) {
        if (!loggedIn) {
            displayMenu();
            cin >> choice;
            cin.ignore(); // Clear newline from buffer

            switch (choice) {
                case 1: // Register
                    clearScreen();
                    cout << "~~~~~~~   Register   ~~~~~~~\n";
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);
                    if (db.registerUser(username, password)) {
                        cout << "Registration successful!\n";
                    } else {
                        cout << "Registration failed. Username might already exist.\n";
                    }
                    break;
                    
                case 2: // Login
                    clearScreen();
                    cout << "~~~~~~~   Login   ~~~~~~~\n";
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);
                    if (db.loginUser(username, password, currentUser)) {
                        cout << "Login successful! Welcome, " << username << "!\n";
                        loggedIn = true;
                    } else {
                        cout << "Login failed. Check your credentials.\n";
                    }
                    break;
                    
                case 3: // Exit
                    cout << "Thank you for using BookSeeks. Goodbye!\n";
                    return 0;
                    
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } else {
            // User is logged in, show logged-in menu
            displayLoggedInMenu();
            cin >> choice;
            cin.ignore(); // Clear newline from buffer

            switch (choice) {
                case 1: // Add book
                    cout << "Enter book title: ";
                    break;
                    
                case 2: // View all books
                    // db.viewAllBooks(currentUser.id);
                    cout << "Viewing all books...\n";
                    break;
                    
                case 3: // Search for a book
                    cout << "Enter search term: ";
                    break;
                    
                case 4: // Logout
                    cout << "Logging out...\n";
                    loggedIn = false;
                    currentUser = User(); // Reset user
                    break;
                    
                case 5: // Exit
                    cout << "Thank you for using BookSeeks. Goodbye!\n";
                    return 0;
                    
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    return 0;
} 