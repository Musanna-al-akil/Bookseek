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
        book.title[sizeof(book.title) - 1] = '\0';

        cout << "Enter Author Name: ";
        getline(cin, temp);
        strcpy(book.author, temp.c_str());
        book.author[sizeof(book.author) - 1] = '\0';
        
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
        cout << left << setw(10) << "ID"
            << setw(30) << "Title"
            << setw(20) << "Author"
            << "Year\n";
        cout << "------------------------------------------------------------------------\n";

        ifstream inFile(filename, ios::binary);  
        if (!inFile) {
            cout << "No books found.\n";
            return;
        }
        
        bool found = false;
        while (inFile.read((char*)&book, sizeof(Book))) {
            cout << left << setw(10) << book.id
                << setw(30) << book.title
                << setw(20) << book.author
                << book.year << endl;
            found = true;
        }
        
        if (!found)
            cout << "No books found.\n";

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

        cout << "Enter search term (title or author): ";
        string searchTerm;
        getline(cin, searchTerm);
        string searchTermLower = toLowercase(searchTerm);

        Book book;
        bool found = false;
        
        while (inFile.read((char*)&book, sizeof(Book))) {
            string bookTitle = book.title;
            string bookAuthor = book.author;

            string bookTitleLower = toLowercase(bookTitle);
            string bookAuthorLower = toLowercase(bookAuthor);

            bool matchFound = bookTitleLower.find(searchTermLower) != string::npos || 
                              bookAuthorLower.find(searchTermLower) != string::npos;

            if (matchFound) {
                if (!found) {
                    cout << left << setw(10) << "ID"
                        << setw(30) << "Title"
                        << setw(20) << "Author"
                        << "Year\n";
                    cout << "------------------------------------------------------------------------\n";
                }
                
                cout << left << setw(10) << book.id
                    << setw(30) << book.title
                    << setw(20) << book.author
                    << book.year << endl;
                    
                found = true;
            }
        }

        if (!found) {
            cout << "No books matched your search.\n";
        }

        inFile.close();
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

        string tempFilename = "temp.dat";
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
