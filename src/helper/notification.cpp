#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

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
void showNotification(const string& message, NotificationType type, int displayTime = 2) {

    cout << "\033[s";
    
    cout << "\033[1;1H";
    
    string prefix, bgColor, textColor;
    switch (type) {
        case SUCCESS:
            bgColor = Color::BG_GREEN;
            textColor = Color::BLACK;
            prefix = "✓ SUCCESS: ";
            break;
        case ERROR:
            bgColor = Color::BG_RED;

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
    
    int width = message.length() + prefix.length() + 4;
    
    cout << bgColor << textColor;
    for (int i = 0; i < width; i++) cout << "═";
    cout << Color::RESET << endl;
    
    cout << "  "<< bgColor << textColor << " " << prefix << message << " " << Color::RESET << endl;
    
    cout << bgColor << textColor;
    for (int i = 0; i < width; i++) cout << "═";
    cout << Color::RESET << endl;
    
    cout.flush();
    this_thread::sleep_for(chrono::seconds(displayTime));
    
    cout << "\033[1;1H\033[K\n\033[K\n\033[K";
    
    cout << "\033[u";
    cout.flush();
}
