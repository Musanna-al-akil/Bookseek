#include <iostream>
#include <string>
#include <curl/curl.h>
#include <memory>

using namespace std;

size_t WriteCallback(void *contents, size_t size, size_t memory_size, string *s) {
    size_t newLength = size * memory_size;
    try {
        s->append((char*)contents, newLength);
    } catch(bad_alloc &e) {
        return 0;
    }
    return newLength;
}

class AIReviewRecommendationService {
    private:
        string api_key;
        
    public:
        AIReviewRecommendationService() {
            api_key = "AIzaSyA3qReayf1FLyxH3nihpNoVSGoyA0PI31g";
        }
        
        bool aiReviewMenu() {
            int choice;
            
            while (true) {
                clearScreen();
                cout << "~~~~~~~  AI Book Review & Recommendation  ~~~~~~~\n";
                cout << "1. Get Book Review by AI\n";
                cout << "2. Return to main menu\n";
                cout << "3. Exit Application\n";
                cout << "Enter your choice: ";
                
                cin >> choice;
                cin.ignore(); 
                
                switch (choice) {
                    case 1:
                        getBookReview();
                        break;
                    case 2:
                        return false; // main menu
                    case 3:
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
        
    private:
        string callGeminiAPI(const string& prompt) {
            if (api_key == "YOUR_API_KEY") {
                return "Error: API key not configured properly.";
            }
            
            string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.5-flash:generateContent?key=" + api_key;
            
            CURL *curl;
            CURLcode res;
            string response_string;
            
            string json_data = "{\n"
                "    \"contents\": [{\n"
                "        \"parts\":[{\n"
                "            \"text\": \"" + prompt + "\"\n"
                "        }]\n"
                "    }],\n"
                "    \"generationConfig\": {\n"
                "        \"thinkingConfig\": {\n"
                "            \"thinkingBudget\": 0\n"
                "        }\n"
                "    }\n"
                "}";
            
            curl_global_init(CURL_GLOBAL_ALL);
            curl = curl_easy_init();
            
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                
                struct curl_slist *headers = NULL;
                headers = curl_slist_append(headers, "Content-Type: application/json");
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
                
                res = curl_easy_perform(curl);
                
                if (res != CURLE_OK) {
                    curl_easy_cleanup(curl);
                    curl_slist_free_all(headers);
                    curl_global_cleanup();
                    return "";
                }
                
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
            }
            
            curl_global_cleanup();
            
            return parseGeminiResponse(response_string);
        }
        
        string parseGeminiResponse(const string& jsonResponse) {

            size_t textPos = jsonResponse.find("\"text\": \"");
            
            textPos += 9; 
            size_t endPos = textPos;
            
            while (endPos < jsonResponse.length()) {
                if (jsonResponse[endPos] == '"' && 
                    (endPos == 0 || jsonResponse[endPos-1] != '\\')) {
                    break;
                }
                endPos++;
            }
            
            if (endPos >= jsonResponse.length()) {
                return "Error: Could not extract review text.";
            }
            
            string result = jsonResponse.substr(textPos, endPos - textPos);
            
            size_t pos = 0;
            while ((pos = result.find("\\n", pos)) != string::npos) {
                result.replace(pos, 2, "\n");
                pos += 1;
            }
            
            pos = 0;
            while ((pos = result.find("\\\"", pos)) != string::npos) {
                result.replace(pos, 2, "\"");
                pos += 1;
            }
            
            return result;
        }
};
