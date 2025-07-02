#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

class WishlistLendService {
    private:
        const string filename;
    
    public:
        WishlistLendService(const string& fname) : filename(fname) {
            ofstream file(filename, ios::app | ios::binary);
            file.close();
        }

        bool wishlistLendMenu() {
            int choice;
            
            while (true) {
                clearScreen();
                cout << "~~~~~~~  Wishlist & Lend  ~~~~~~~\n";
                cout << "1. Add Book to Wishlist\n";
                cout << "2. Show Wishlist\n";
                cout << "3. Delete Book from Wishlist\n";
                cout << "4. Lend a Book\n";
                cout << "5. Show Lended Books\n";
                cout << "6. Return to main menu\n";
                cout << "7. Exit Application\n";
                cout << "Enter your choice: ";
                
                cin >> choice;
                cin.ignore(); 
                
                switch (choice) {
                    case 1:
                        addBookToWishlist();
                        break;
                    case 2:
                        showWishlist();
                        break;
                    case 3:
                        deleteBookFromWishlist();
                        break;
                    case 4:
                        lendBook();
                        break;
                    case 5:
                        showLendedBooks();
                        break;
                    case 6:
                        return false;
                    case 7:
                        return true; 
                    default:
                        clearScreen();
                        cout << "Invalid option! Please try again.\n";
                        cout << "Press Enter to continue...";
                        cin.ignore();
                        break;
                }
            }
        }

        void addBookToWishlist() 
        {
            clearScreen();
            cout << "~~~~~~~  Add Book to Wishlist  ~~~~~~~\n";
            
            Book book;
            
            // find unique id for the book
            int nextId = 1;
            ifstream file(filename, ios::binary);
            if (file.is_open()) {
                Book tempBook;
                while (file.read((char*)&tempBook, sizeof(Book))) {
                    if (tempBook.id >= nextId) {
                        nextId = tempBook.id + 1;
                    }
                }
                file.close();
            }
            book.id = nextId;

            string userInput;
            
            cout << "Enter Book Title: ";
            getline(cin, userInput);
            strcpy(book.title, userInput.c_str());

            cout << "Enter Author Name: ";
            getline(cin, userInput);
            strcpy(book.author, userInput.c_str());

            cout << "Enter Year: ";
            cin >> book.year;
            cin.ignore();
            
            bool bookExists = false;
            ifstream checkFile(filename, ios::binary);
            if (checkFile.is_open()) {
                Book existingBook;
                while (checkFile.read((char*)&existingBook, sizeof(Book))) {
                    if (strcmp(existingBook.title, book.title) == 0 && 
                        strcmp(existingBook.author, book.author) == 0) {
                        bookExists = true;
                        break;
                    }
                }
                checkFile.close();
            }
            
            if (bookExists) {
                clearScreen();
                showNotification("This book already exists in your collection!", ERROR);
                return;
            }

            book.isWishlist = true;
            book.isLent = false;
            strcpy(book.lent_to, "");
            strcpy(book.lend_to, "");
            book.isBorrowed = false;
            strcpy(book.borrowed_from, "");
            strcpy(book.category, "");
            strcpy(book.status, "Not Started");
            book.rating = 0;
            strcpy(book.review, "");

            ofstream saveFile(filename, ios::binary | ios::app);
            if (saveFile.is_open()) {
                saveFile.write((char*)&book, sizeof(Book));
                saveFile.close();
                clearScreen();
                showNotification("Book added to wishlist successfully!", SUCCESS);
            } else {
                clearScreen();
                showNotification("Error: Could not save the book!", ERROR);
            }
        }

        void showWishlist() {
            clearScreen();
            cout << "~~~~~~~  Your Wishlist  ~~~~~~~\n";
            cout << left << setw(10) << "ID"
                << setw(30) << "Title"
                << setw(20) << "Author"
                << "Year\n";
            cout << "------------------------------------------------------------------------\n";

            ifstream file(filename, ios::binary);
            if (!file) {
                // showNotification("No books found!", INFO);
                cout << "No books found!\n";
                cout << "\nPress Enter to continue...";
                cin.ignore();
                return;
            }
            
            Book book;
            bool found = false;
            while (file.read((char*)&book, sizeof(Book))) {
                if (book.isWishlist) {
                    cout << left << setw(10) << book.id
                        << setw(30) << book.title
                        << setw(20) << book.author
                        << book.year << endl;
                    found = true;
                }
            }
            
            if (!found) {
                cout << "No books in wishlist.\n";
            }

            file.close();
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }

        void deleteBookFromWishlist() {
            clearScreen();
            cout << "~~~~~~~  Remove Book from Wishlist  ~~~~~~~\n";
            cout << "Enter Book ID to remove from wishlist: ";
            
            int bookIdToDelete;
            cin >> bookIdToDelete;
            cin.ignore();

            ifstream file(filename, ios::binary);
            if (!file) {
                showNotification("No books found!", ERROR);
                return;
            }

            string tempFilename = "../db/temp.bin";
            ofstream tempFile(tempFilename, ios::binary);
            Book book;
            bool found = false;

            while (file.read((char*)&book, sizeof(Book))) {
                if (book.id == bookIdToDelete && book.isWishlist) {
                    found = true;
                    showNotification("Book deleted from wishlist!", SUCCESS);
                } else {
                    tempFile.write((char*)&book, sizeof(Book));
                }
            }

            file.close();
            tempFile.close();

            if (found) {
                remove(filename.c_str());
                rename(tempFilename.c_str(), filename.c_str());
            } else {
                remove(tempFilename.c_str());
                showNotification("Book not found in wishlist!", ERROR);
            }
        }

        void lendBook() {
            clearScreen();
            cout << "~~~~~~~  Lend a Book  ~~~~~~~\n";
            cout << "Enter Book ID to lend: ";
            int bookId;
            cin >> bookId;
            cin.ignore();

            fstream file(filename, ios::binary | ios::in | ios::out);
            if (!file) {
                showNotification("No books found!", ERROR);
                return;
            }

            Book book;
            bool found = false;

            while (file.read((char*)&book, sizeof(Book))) {
                if (book.id == bookId && !book.isLent) {
                    cout << "Enter name of person to lend to: ";
                    string lendTo;
                    getline(cin, lendTo);
                    
                    strcpy(book.lend_to, lendTo.c_str());
                    book.isLent = true;
                    found = true;
                    
                    // fall back one step back and rewrite the book data
                    file.seekp(file.tellg() - static_cast<streamoff>(sizeof(Book)));
                    file.write((char*)&book, sizeof(Book));
                    
                    showNotification("Book '" + string(book.title) + "' lent to " + string(book.lent_to) + " successfully!", SUCCESS);
                    break;
                }
            }

            file.close();

            if (!found) {
                showNotification("Book not found or already lent!", ERROR);
            }
        }

        void showLendedBooks() {
            clearScreen();
            cout << "~~~~~~~  Lended Books  ~~~~~~~\n";
            cout << left << setw(10) << "ID"
                << setw(25) << "Title"
                << setw(20) << "Author"
                << setw(20) << "Lent To"
                << "Year\n";
            cout << "------------------------------------------------------------------------\n";

            ifstream file(filename, ios::binary);
            if (!file) {
                showNotification("No books found!", INFO);
                return;
            }
            
            Book book;
            bool found = false;
            while (file.read((char*)&book, sizeof(Book))) {
                if (book.isLent) {
                    cout << left << setw(10) << book.id
                        << setw(25) << book.title
                        << setw(20) << book.author
                        << setw(20) << book.lent_to
                        << book.year << endl;
                    found = true;
                }
            }
            
            if (!found) {
                cout << "No books are currently lent.\n";
            }

            file.close();
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
};


  
