#include <iostream>
#include <string>
#include <memory>
#include "../include/json.hpp"
#include <cpr/cpr.h>

using namespace std;
using json = nlohmann::json;

class AIReviewRecommendationService {
    private:
        string api_key;
        const string filename;
        
    public:
        AIReviewRecommendationService(const string& fname):filename(fname) {
            api_key = "AIzaSyA3qReayf1FLyxH3nihpNoVSGoyA0PI31g";
        }
        
        bool aiReviewMenu() {
            int choice;
            
            while (true) {
                clearScreen();
                cout << "~~~~~~~  AI Book Review & Recommendation  ~~~~~~~\n";
                cout << "1. Get Book Review by AI\n";
                cout << "2. Get Book Recommendations based on your last 5 books by AI\n";
                cout << "3. Return to main menu\n";
                cout << "4. Exit Application\n";
                cout << "Enter your choice: ";
                
                cin >> choice;
                cin.ignore(); 
                
                switch (choice) {
                    case 1:
                        getBookReview();
                        break;
                    case 2:
                        AiRecommendation();
                        break;
                    case 3:
                        return false; // main menu
                    case 4:
                        return true;  // Exit application
                    default:
                        clearScreen();
                        cout << "Invalid option! Please try again.\n";
                        cout << "Press Enter to continue...";
                        cin.ignore();
                        break;
                }
            }
        }
        
        void getBookReview() {
            clearScreen();
            cout << "~~~~~~~  AI Book Review  ~~~~~~~\n";
            
            string bookName, authorName;
            cout << "Enter Book Title: ";
            getline(cin, bookName);
            
            cout << "Enter Author Name (optional): ";
            getline(cin, authorName);
            
            cout << "\nBook review processing...\n";
            
            string prompt = "Please provide a detailed book review for the book '" + bookName + "'";
            if (!authorName.empty()) {
                prompt += " by " + authorName;
            }
            prompt += ". Include information about the plot, writing style, themes, and your overall opinion. Keep the review between 100-200 words with in simple language. and also give a rating out of 10.";
            
            string review = callGeminiAPI(prompt);
            
            if (!review.empty()) {
                clearScreen();
                cout << "~~~~~~~  AI Book Review  ~~~~~~~\n";
                cout << "Book: " << bookName;
                if (!authorName.empty()) {
                    cout << " by " << authorName;
                }
                cout << "\n\n";
                cout << "Review:\n";
                cout << "========================================\n";
                cout << review << "\n";
                cout << "========================================\n";
            } else {
                clearScreen();
                showNotification("Failed to get book review. Please try again.", ERROR);
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
         void AiRecommendation() {
            clearScreen();
            cout << "~~~~~~~  AI Book Recommendation  ~~~~~~~\n";

            vector<Book> recentBooks;
            ifstream inFile(filename, ios::binary);
            if (!inFile) {
                showNotification("No books found in your collection. Add some books first.", WARNING);
                return;
            }

            Book book;
            while (inFile.read((char*)&book, sizeof(Book))) {
                if (book.userId == userId) {
                    recentBooks.push_back(book);
                }
            }
            
            if (recentBooks.empty()) {
                clearScreen();
                showNotification("No books found in your collection. Add some books first.", WARNING);
                cout << "\nPress Enter to continue...";
                cin.ignore();
                return;
            }
            
            string prompt = "Based on the following books I've read:\n";
            for (const auto& book : recentBooks) {
                prompt +=  book.title;
                prompt += " , ";
            }
            prompt += "\nPlease recommend 5 different books that I might enjoy. the first line will be 'your last five books are:'(last five books). based on the last five books recommend 5 different books that user might enjoy. Try to recommend books that are similar category user read before and also try to give users new category of books so user can explore new books. For each recommendation, include:\n";
            prompt += "1. Book title:\n2. Author:\n3. description: (1 sentence)\n4. ratting: (rating out of 10)\n5. category: (category of the book) \n";
            prompt += "ignore: don't include any ** in output";
            
            cout << "\nGenerating personalized book recommendations from last 5 books...\n";
            
            string recommendations = callGeminiAPI(prompt);
            
            if (!recommendations.empty()) {
                clearScreen();
                cout << "~~~~~~~  AI Book Recommendations  ~~~~~~~\n";
                cout << "Based on your last 5 books reading history, here are some books you might enjoy:\n\n";
                cout << "========================================\n";
                cout << recommendations << "\n";
                cout << "========================================\n";
            } else {
                clearScreen();
                showNotification("Failed to get book recommendations. Please try again.", ERROR);
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
        
        
    private:
        string callGeminiAPI(const string& prompt) {
            json data;
            data["contents"][0]["parts"][0]["text"] = prompt;

            string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + api_key;

            cpr::Response response = cpr::Post(cpr::Url{url},
                                               cpr::Body{data.dump()},
                                               cpr::Header{{"Content-Type", "application/json"}});

            if (response.status_code == 200) {
                return parseResponse(response.text);
            } else {
                string msg = "Failed to call Gemini API.";
                msg += " Status code: " + to_string(response.status_code);
                msg += " Body: " + response.text;
                if (response.error) {
                     msg += " Error: " + response.error.message;
                }
                cerr << msg << endl;
                return "";
            }
        }
        
        string parseResponse(const string& jsonText) {
            try {
                json j = json::parse(jsonText);
                if (j.contains("candidates") && j["candidates"].is_array() && !j["candidates"].empty()) {
                    auto candidate = j["candidates"][0];
                    if (candidate.contains("content") 
                    && candidate["content"].contains("parts") 
                    && candidate["content"]["parts"].is_array() 
                    && !candidate["content"]["parts"].empty()) 
                    {
                        return candidate["content"]["parts"][0]["text"].get<string>();
                    }
                }
                 if (j.contains("promptFeedback")) {
                    return "Error: " + j.dump(4);
                }
                return "Error: Could not extract review text from API response.";
            } catch (json::parse_error& e) {
                cerr << "JSON parse error: " << e.what() << '\n';
                return "Error: Failed to parse API response.";
            }
        }
};
