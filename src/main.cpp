#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "services/users.cpp"
#include "services/book.cpp"

using namespace std;

//db name
const string userFile = "db/users.bin";
const string bookFile = "db/books.bin";

// color code
namespace Color {
    const string RESET = "\033[0m";
    const string BLACK = "\033[30m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string WHITE = "\033[37m";
    const string BG_RED = "\033[41m";
    const string BG_GREEN = "\033[42m";
    const string BG_YELLOW = "\033[43m";
    const string BG_BLUE = "\033[44m";
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

enum NotificationType {
    SUCCESS,
    ERROR,
    INFO,
    WARNING
};

// show notification
void showNotification(const string& message, NotificationType type, int displayTime = 3) {
    // Save cursor position
    cout << "\033[s";
    
    // Move to top of screen
    cout << "\033[1;1H";
    
    // colors and icons based on notification type
    string prefix, bgColor, textColor;
    switch (type) {
        case SUCCESS:
            bgColor = Color::BG_GREEN;
            textColor = Color::BLACK;
            prefix = "✓ SUCCESS: ";
            break;
        case ERROR:
            bgColor = Color::BG_RED;
            textColor = Color::WHITE;
            prefix = "✗ ERROR: ";
            break;
        case INFO:
            bgColor = Color::BG_BLUE;
            textColor = Color::WHITE;
            prefix = "ℹ INFO: ";
            break;
        case WARNING:
            bgColor = Color::BG_YELLOW;
            textColor = Color::BLACK;
            prefix = "⚠ WARNING: ";
            break;
    }
    
    // Calculate notification width for the border
    int width = message.length() + prefix.length() + 4;
    
    // Top border
    cout << bgColor << textColor;
    for (int i = 0; i < width; i++) cout << "═";
    cout << Color::RESET << endl;
    
    // Message
    cout << "  "<< bgColor << textColor << " " << prefix << message << " " << Color::RESET << endl;
    
    // Bottom border
    cout << bgColor << textColor;
    for (int i = 0; i < width; i++) cout << "═";
    cout << Color::RESET << endl;
    
    // display how many seconds
    cout.flush();
    this_thread::sleep_for(chrono::seconds(displayTime));
    
    // Clear notification
    cout << "\033[1;1H\033[K\n\033[K\n\033[K";
    
    cout << "\033[u";
    cout.flush();
}

void displayMenu() {
    clearScreen();
    cout << "\n===== BookSeeks: Personal Books Management System =====\n";
    cout << "1. Register new user\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Enter your choice: ";
}

void displayLoggedInMenu() {
    clearScreen();
    cout << "\n===== BookSeeks: User Menu =====\n";
    cout << "1. Add new book\n";
    cout << "2. View all books\n";
    cout << "3. Search for a book\n";
    cout << "4. Delete a book\n";
    cout << "5. Logout\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}



int main() {
    // initialize db
    UserService db(userFile);
    BookService book(bookFile);

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
                    if (db.addUser(username, password)) {
                        showNotification("Account created successfully!", SUCCESS);
                    } else {
                        showNotification("Username already exists", ERROR);
                    }
                    break;
                    
                case 2: // Login
                    clearScreen();
                    cout << "~~~~~~~   Login   ~~~~~~~\n";

                    cout << "Enter username: ";
                    getline(cin, username);

                    cout << "Enter password: ";
                    getline(cin, password);

                    if (db.authenticateUser(username, password)) {
                        showNotification("Login successful! Welcome, " + username, SUCCESS);
                        loggedIn = true;
                    } else {
                        showNotification("Invalid username or password", ERROR);
                    }
                    break;
                    
                case 3: 
                    showNotification("Thank you for using BookSeeks!", INFO, 2);
                    return 0;
                    
                default:
                    clearScreen();
                    showNotification("Invalid option selected", WARNING);
                    displayMenu();
            }
        } else {
            // User is logged in
            displayLoggedInMenu();
            cin >> choice;
            cin.ignore(); 

           
            switch (choice) {
                case 1: 
                    // Add book
                    clearScreen();
                    if(book.addBook()){
                         clearScreen();
                        showNotification("Book added successfully!", SUCCESS);
                    }else{
                         clearScreen(); 
                        showNotification("Book already exists", ERROR);
                    }

                    break;
                    
                case 2: // View all books
                    clearScreen();
                    cout << "~~~~~~~  Viewing all books  ~~~~~~~\n";
                    book.viewBooks();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    break;
                    
                case 3: // Search for a book
                    clearScreen();
                    cout << "~~~~~~~  Search for a book  ~~~~~~~\n";
                    book.searchBooks();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    break;
                    
                case 4: // Delete a book
                    clearScreen();
                    cout << "~~~~~~~  Delete a book  ~~~~~~~\n";
                    if(book.deleteBook()){
                        clearScreen();
                        showNotification("Book deleted successfully!", SUCCESS);
                        displayLoggedInMenu();
                    }else{
                        clearScreen();
                        showNotification("Book not found", ERROR);
                        displayLoggedInMenu();
                    }
                    break;
                case 5: // Logout
                 clearScreen();
                    cout << "Logging out...\n";
                    loggedIn = false;
                    currentUser = User();
                    break;
                    
                case 6: // Exit
                 clearScreen();
                    cout << "Thank you for using BookSeek. Goodbye!\n";
                    return 0;
                    
                default:
                 clearScreen();
                showNotification("Invalid option selected", WARNING);
                displayMenu();
            }
        }
    }

    return 0;
} 