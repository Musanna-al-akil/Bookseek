#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "helper/notification.cpp"
#include "services/users.cpp"
#include "services/book.cpp"
#include "services/wishlist-lend.cpp"
#include "services/ai-review-recommendation.cpp"

using namespace std;

//db path
const string userFilePath = "db/users.bin";
const string bookFilePath = "db/books.bin";

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
    cout << "5. Wishlist & Lend\n";
    cout << "6. Filter books by category\n";
    cout << "7. Update reading status\n";
    cout << "8. Add rating and review\n";
    cout << "9. AI Book Review and Recommendation\n";
    cout << "10. Logout\n";
    cout << "11. Exit\n";
    cout << "Enter your choice: ";
}


int main() {
    // db init
    UserService user(userFilePath);
    BookService book(bookFilePath);
    WishlistLendService wishlistLend(bookFilePath);
    AIReviewRecommendationService aiService;

    
    int choice = 0;
    string username, password, title, author;
    User currentUser;
    bool loggedIn = false;

    clearScreen();

    cout << "~~~~~~~   Welcome to BookSeeks!   ~~~~~~~\n";

    while (true) {
        if (!loggedIn) {
            displayMenu();
            cin >> choice;
            cin.ignore(); 

            switch (choice) {
                case 1: // Register
                    clearScreen();
                    cout << "~~~~~~~   Register   ~~~~~~~\n";
                    cout << "Enter username: ";
                    getline(cin, username);
                    cout << "Enter password: ";
                    getline(cin, password);
                    if (user.addUser(username, password)) {
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

                    if (user.authenticateUser(username, password)) {
                        showNotification("Login successful! Welcome, " + username, SUCCESS);
                        loggedIn = true;
                    } else {
                        showNotification("Invalid username or password", ERROR);
                    }
                    break;
                    
                case 3: // Exit
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
                case 1: // Add book
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
                    
                case 3: // Search book
                    clearScreen();

                    cout << "~~~~~~~  Search for a book  ~~~~~~~\n";
                    book.searchBooks();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    break;
                    
                case 4: // Delete book
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
                case 5: // Wishlist & Lend
                    clearScreen();

                    if (wishlistLend.wishlistLendMenu()) {
                        clearScreen();
                        cout << "Thank you for using BookSeek. Goodbye!\n";
                        return 0;
                    }
                    break;
                case 6: // Filter books by category
                    clearScreen();
                    cout << "~~~~~~~  Filter books by category  ~~~~~~~\n";
                    book.filterByCategory();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    break;
                case 7: // Update reading status
                    clearScreen();
                    cout << "~~~~~~~  Update reading status  ~~~~~~~\n";
                    book.updateProgress();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    break;
                case 8: // Add rating and review
                    clearScreen();
                    cout << "~~~~~~~  Add rating and review  ~~~~~~~\n";
                    book.addRatingReview();
                    cout << "\nPress Enter to return to menu...";
                    cin.ignore();
                    break;
                case 9: // Ai recommend and review
                    clearScreen();
                    if (aiService.aiReviewMenu()) {
                        clearScreen();
                        cout << "Thank you for using BookSeek. Goodbye!\n";
                        return 0;
                    }
                    break;
                case 10: // Logout
                    clearScreen();
                 
                    cout << "Logging out...\n";
                    loggedIn = false;
                    userId = -1;
                    currentUser = User();
                    break;
                    
                case 11: // Exit
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