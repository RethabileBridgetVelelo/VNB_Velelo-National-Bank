#ifndef VNBUI_H
#define VNBUI_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>

class VNBUI {
public:
    // VNB Color Codes
    class Colors {
    public:
        static const std::string RESET;
        static const std::string BLACK;
        static const std::string RED;
        static const std::string GREEN;
        static const std::string YELLOW;
        static const std::string BLUE;
        static const std::string MAGENTA;
        static const std::string CYAN;
        static const std::string WHITE;
        static const std::string BRIGHT_BLACK;
        static const std::string BRIGHT_RED;
        static const std::string BRIGHT_GREEN;
        static const std::string BRIGHT_YELLOW;
        static const std::string BRIGHT_BLUE;
        static const std::string BRIGHT_MAGENTA;
        static const std::string BRIGHT_CYAN;
        static const std::string BRIGHT_WHITE;
        
        // VNB Specific Colors
        static const std::string VNB_BLUE;
        static const std::string VNB_GOLD;
        static const std::string VNB_SILVER;
    };
    
    // Initialize/Shutdown
    static void init();
    static void cleanup();
    
    // Screen Control
    static void clear();
    static void setCursor(int x, int y);
    static void hideCursor();
    static void showCursor();
    
    // Drawing Functions
    static void drawBox(int x, int y, int width, int height, 
                       const std::string& title = "", 
                       const std::string& color = Colors::BRIGHT_BLUE);
    
    static void drawLine(int length, const std::string& color = Colors::CYAN);
    static void drawCentered(const std::string& text, int width, 
                           const std::string& color = Colors::BRIGHT_WHITE);
    
    // Display Functions
    static void showLogo();
    static void showWelcome();
    static void showLoading(int seconds = 2);
    static void showMessage(const std::string& msg, const std::string& type = "info");
    
    // Input Functions
    static std::string getInput(const std::string& prompt, 
                               const std::string& color = Colors::BRIGHT_CYAN);
    static std::string getSecureInput(const std::string& prompt);
    static int getMenuChoice(const std::string& title, 
                           const std::vector<std::string>& options);
    
    // Banking UI Components
    static void showBalance(double amount);
    static void showAccountInfo(const std::string& accNum, 
                               const std::string& name, 
                               double balance,
                               const std::string& type);
    static void showTransaction(const std::string& type, 
                               double amount, 
                               const std::string& date);
    
    // Helper Functions
    static std::string formatCurrency(double amount);
    static std::string formatDate();
    static void delay(int milliseconds);
    
private:
    static void setupConsole();
    static int consoleWidth();
    static int consoleHeight();
};

#endif // VNBUI_H