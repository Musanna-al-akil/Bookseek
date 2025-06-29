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
            // Check if file exists, if not create it
            ofstream checkFile(filename, ios::app | ios::binary);
            checkFile.close();
        }

    bool addBook() {
        Book book;
        
        // Get the highest existing ID to auto-increment
        int maxId = 0;
        ifstream idFile(filename, ios::binary);
        if (idFile) {
            Book tempBook;
            while (idFile.read(reinterpret_cast<char*>(&tempBook), sizeof(Book))) {
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
        strncpy(book.title, temp.c_str(), sizeof(book.title));
        book.title[sizeof(book.title) - 1] = '\0';

        cout << "Enter Author Name: ";
        getline(cin, temp);
        strncpy(book.author, temp.c_str(), sizeof(book.author));
        book.author[sizeof(book.author) - 1] = '\0';
        
        // Check if book already exists
        string inputTitle = book.title;
        string inputAuthor = book.author;
        
        ifstream checkFile(filename, ios::binary);
        if (checkFile) {
            Book existingBook;
            while (checkFile.read(reinterpret_cast<char*>(&existingBook), sizeof(Book))) {
                if (strcmp(existingBook.title, book.title) == 0 && 
                    strcmp(existingBook.author, book.author) == 0) {
                    checkFile.close();
                    return false;
                }
            }
            checkFile.close();
        }

        cout << "Enter Year: ";
        cin >> book.year;
        cin.ignore();

        ofstream outFile(filename, ios::binary | ios::app);
        if (!outFile) {
            cout << "Error opening file for writing.\n";
            return false;
        }
        outFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
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
        while (inFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
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

    void searchBooks() {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "No books found.\n";
            return;
        }

        cout << "Enter search term (title or author): ";
        string term;
        getline(cin, term);

        Book book;
        bool found = false;
        while (inFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
            string titleStr(book.title);
            string authorStr(book.author);

            // Convert to lowercase 
            string titleLower = titleStr;
            string authorLower = authorStr;
            string termLower = term;
            for (auto & c: titleLower) c = tolower(c);
            for (auto & c: authorLower) c = tolower(c);
            for (auto & c: termLower) c = tolower(c);

            if (titleLower.find(termLower) != string::npos || authorLower.find(termLower) != string::npos) {
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

        if (!found)
            cout << "No books matched your search.\n";

        inFile.close();
    }

    bool deleteBook() {
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "No books found.\n";
            return false;
        }

        cout << "Enter Book ID to delete: ";
        int delId;
        cin >> delId;
        cin.ignore();

        string tempFilename = "temp.dat";
        ofstream tempFile(tempFilename, ios::binary);
        Book book;
        bool found = false;

        while (inFile.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
            if (book.id != delId) {
                tempFile.write(reinterpret_cast<char*>(&book), sizeof(Book));
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
