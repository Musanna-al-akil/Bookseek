#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

struct Book {
    int id;
    char title[50];
    char author[50];
    char lend_to[50];
    bool isLent;
    char lent_to[50];
    bool isBorrowed;
    char borrowed_from[50];
    bool isWishlist;
    int year;
    char category[30];       
    char status[20];         
    int rating;             // rating 1-5
    char review[100];        
};


class BookService {
    private:
        const string filename;
    public:
        BookService(const string& fname) : filename(fname) {
            ofstream checkFile(filename, ios::app | ios::binary);
            checkFile.close();
        }

    bool addBook() {
        Book book;
        
        // next id
        int maxId = 0;
        ifstream idFile(filename, ios::binary);
        if (idFile) {
            Book tempBook;
            while (idFile.read((char*)&tempBook, sizeof(Book))) {
                if (tempBook.id > maxId) {
                    maxId = tempBook.id;
                }
            }
            idFile.close();
        }
        
        book.id = maxId + 1;

        string temp;
        cout << "Enter Book Title: ";
        getline(cin, temp);
        strcpy(book.title, temp.c_str());

        cout << "Enter Author Name: ";
        getline(cin, temp);
        strcpy(book.author, temp.c_str());
        
        ifstream checkFile(filename, ios::binary);
        if (checkFile) {
            Book existingBook;
            while (checkFile.read((char*)&existingBook, sizeof(Book))) {
                if (strcmp(existingBook.title, book.title) == 0 && 
                    strcmp(existingBook.author, book.author) == 0) {
                    checkFile.close();
                    return false;
                }
            }
            checkFile.close();
        }

        cout << "Enter Year: ";
        int year;
        cin >> year;
        
        while (cin.fail() || year < 1 || year > 9999) {
            cin.clear(); 
            cin.ignore(1000, '\n');
            cout << "Invalid year! Please enter a valid year: ";
            cin >> year;
        }
        
        book.year = year;
        cin.ignore();
        
        cout << "Enter Category/genre: ";
        getline(cin, temp);
        strcpy(book.category, temp.c_str());
        
        strcpy(book.status, "Not Started");
        
        book.rating = 0;
        strcpy(book.review, "");
        
        strcpy(book.lend_to, "");
        book.isLent = false;
        strcpy(book.lent_to, "");
        book.isBorrowed = false;
        strcpy(book.borrowed_from, "");
        book.isWishlist = false;

        ofstream outFile(filename, ios::binary | ios::app);
        if (!outFile) {
            cout << "Error opening file for writing.\n";
            return false;
        }
        outFile.write((char*)&book, sizeof(Book));
        outFile.close();
        
        return true;
    }

    void viewBooks() {
        Book book;
        cout << left << setw(5) << "ID"
            << setw(20) << "Title"
            << setw(15) << "Author"
            << setw(5) << "Year"
            << setw(15) << "Category"
            << setw(15) << "Status"
            << setw(8) << "Rating\n";
           
        cout << string(100, '-') << endl;

        ifstream inFile(filename, ios::binary);  
        if (!inFile) {
            cout << "No books found.\n";
            return;
        }
        
        bool found = false;
        while (inFile.read((char*)&book, sizeof(Book))) {
            if (book.isWishlist) {
                continue;
            }
            cout << left << setw(5) << book.id
                << setw(20) << book.title
                << setw(15) << book.author
                << setw(5) << book.year
                << setw(15) << book.category
                << setw(15) << book.status
                << setw(8) << (book.rating > 0 ? to_string(book.rating) : "-")
                << endl;
            found = true;
        }
        
        if (!found) {
            cout << "No books found.\n";
        }
        inFile.close();
    }

    string toLowercase(string& str) {
        string result = str;
        for (auto & c: result) {
            c = tolower(c);
        }
        return result;
    }

    void searchBooks() {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "No books found.\n";
            return;
        }

        cout << "Enter search term (title, author, or category): ";
        string searchTerm;
        getline(cin, searchTerm);
        string searchTermLower = toLowercase(searchTerm);

        Book book;
        bool found = false;
        
        while (inFile.read((char*)&book, sizeof(Book))) {
            string bookTitle = book.title;
            string bookAuthor = book.author;
            string bookCategory = book.category;

            string bookTitleLower = toLowercase(bookTitle);
            string bookAuthorLower = toLowercase(bookAuthor);
            string bookCategoryLower = toLowercase(bookCategory);

            bool matchFound = bookTitleLower.find(searchTermLower) != string::npos || 
                              bookAuthorLower.find(searchTermLower) != string::npos ||
                              bookCategoryLower.find(searchTermLower) != string::npos;

            if (matchFound) {
                if (!found) {
                    cout << left << setw(5) << "ID"
                        << setw(20) << "Title"
                        << setw(15) << "Author"
                        << setw(5) << "Year"
                        << setw(15) << "Category"
                        << setw(15) << "Status"
                        << setw(8) << "Rating\n";
                        
                    cout << string(100, '-') << endl;
                }
                
                cout << left << setw(5) << book.id
                    << setw(20) << book.title
                    << setw(15) << book.author
                    << setw(5) << book.year
                    << setw(15) << book.category
                    << setw(15) << book.status
                    << setw(8) << (book.rating > 0 ? to_string(book.rating) : "-")
                    << endl;
                    
                found = true;
            }
        }

        if (!found) {
            cout << "No books matched your search.\n";
        }

        inFile.close();
    }
    
    void filterByCategory() {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "No books found.\n";
            return;
        }

        cout << "Enter category to filter by: ";
        string category;
        getline(cin, category);
        string categoryLower = toLowercase(category);

        Book book;
        bool found = false;
        
        while (inFile.read((char*)&book, sizeof(Book))) {
            string bookCategory = book.category;
            string bookCategoryLower = toLowercase(bookCategory);

            if (bookCategoryLower.find(categoryLower) != string::npos) {
                if (!found) {
                    cout << left << setw(5) << "ID"
                        << setw(20) << "Title"
                        << setw(15) << "Author"
                        << setw(5) << "Year"
                        << setw(15) << "Category"
                        << setw(15) << "Status"
                        << setw(8) << "Rating"
                        << "Review\n";
                    cout << string(100, '-') << endl;
                }
                
                cout << left << setw(5) << book.id
                    << setw(20) << book.title
                    << setw(15) << book.author
                    << setw(5) << book.year
                    << setw(15) << book.category
                    << setw(15) << book.status
                    << setw(8) << (book.rating > 0 ? to_string(book.rating) : "-")
                    << book.review << endl;
                    
                found = true;
            }
        }

        if (!found) {
            cout << "No books found in that category.\n";
        }

        inFile.close();
    }
    
    void updateProgress() {
        int bookId;
        cout << "Enter Book ID to update reading status: ";
        cin >> bookId;
        cin.ignore();
        
        fstream file(filename, ios::in | ios::out | ios::binary);
        if (!file) {
            cout << "Error opening file.\n";
            return;
        }
        
        Book book;
        bool found = false;
        
        while (file.read((char*)&book, sizeof(Book))) {
            if (book.id == bookId) {
                found = true;
                
                cout << "Current status: " << book.status << "\n";
                cout << "Available statuses:\n";
                cout << "1. Not Started\n";
                cout << "2. Reading\n";
                cout << "3. On Hold\n";
                cout << "4. Completed\n";
                cout << "Enter new status (1-4): ";
                
                int statusChoice;
                cin >> statusChoice;
                cin.ignore();
                
                while (cin.fail() || statusChoice < 1 || statusChoice > 4) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid choice! Please enter a number between 1-4: ";
                    cin >> statusChoice;
                    cin.ignore();
                }
                
                switch (statusChoice) {
                    case 1:
                        strcpy(book.status, "Not Started");
                        break;
                    case 2:
                        strcpy(book.status, "Reading");
                        break;
                    case 3:
                        strcpy(book.status, "On Hold");
                        break;
                    case 4:
                        strcpy(book.status, "Completed");
                        break;
                }
                
                file.seekp(file.tellg() - static_cast<streamoff>(sizeof(Book)));
                file.write((char*)&book, sizeof(Book));
                cout << "Reading status updated successfully.\n";
                break;
            }
        }
        
        if (!found) {
            cout << "Book not found.\n";
        }
        
        file.close();
    }
    
    void addRatingReview() {
        int bookId;
        cout << "Enter Book ID to rate and review: ";
        cin >> bookId;
        cin.ignore();
        
        fstream file(filename, ios::in | ios::out | ios::binary);
        if (!file) {
            cout << "Error opening file.\n";
            return;
        }
        
        Book book;
        bool found = false;
        
        while (file.read((char*)&book, sizeof(Book))) {
            if (book.id == bookId) {
                found = true;
                
                cout << "Enter rating (1-5): ";
                int newRating;
                cin >> newRating;
                cin.ignore();
                
                while (cin.fail() || newRating < 1 || newRating > 5) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid rating! Please enter a value between 1-5: ";
                    cin >> newRating;
                    cin.ignore();
                }
                
                book.rating = newRating;
                
                string reviewText;
                cout << "Enter review: ";
                getline(cin, reviewText);
                strcpy(book.review, reviewText.c_str());
                book.review[sizeof(book.review) - 1] = '\0';
                
                file.seekp(file.tellg() - static_cast<streamoff>(sizeof(Book)));
                file.write((char*)&book, sizeof(Book));
                cout << "Rating and review updated successfully.\n";
                break;
            }
        }
        
        if (!found) {
            cout << "Book not found.\n";
        }
        
        file.close();
    }

    bool deleteBook() {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "No books found.\n";
            return false;
        }

        cout << "Enter Book ID to delete: ";
        int bookIdToDelete;
        cin >> bookIdToDelete;
        cin.ignore();

        string tempFilename = "db/temp.bin";
        ofstream tempFile(tempFilename, ios::binary);
        Book book;
        bool found = false;

        while (inFile.read((char*)&book, sizeof(Book))) {
            if (book.id != bookIdToDelete) {
                tempFile.write((char*)&book, sizeof(Book));
            } else {
                found = true;
            }
        }

        inFile.close();
        tempFile.close();

        if (found) {
            remove(filename.c_str());
            rename(tempFilename.c_str(), filename.c_str());
            return true;
        } else {
            remove(tempFilename.c_str());
            return false;
        }
    }
};
