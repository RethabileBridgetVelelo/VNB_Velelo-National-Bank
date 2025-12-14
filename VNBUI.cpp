#include "VNBUI.h"
#include <iostream>
#include <windows.h>

// Initialize color constants
const std::string VNBUI::Colors::RESET = "\033[0m";
const std::string VNBUI::Colors::BLACK = "\033[30m";
const std::string VNBUI::Colors::RED = "\033[31m";
const std::string VNBUI::Colors::GREEN = "\033[32m";
const std::string VNBUI::Colors::YELLOW = "\033[33m";
const std::string VNBUI::Colors::BLUE = "\033[34m";
const std::string VNBUI::Colors::MAGENTA = "\033[35m";
const std::string VNBUI::Colors::CYAN = "\033[36m";
const std::string VNBUI::Colors::WHITE = "\033[37m";
const std::string VNBUI::Colors::BRIGHT_BLACK = "\033[90m";
const std::string VNBUI::Colors::BRIGHT_RED = "\033[91m";
const std::string VNBUI::Colors::BRIGHT_GREEN = "\033[92m";
const std::string VNBUI::Colors::BRIGHT_YELLOW = "\033[93m";
const std::string VNBUI::Colors::BRIGHT_BLUE = "\033[94m";
const std::string VNBUI::Colors::BRIGHT_MAGENTA = "\033[95m";
const std::string VNBUI::Colors::BRIGHT_CYAN = "\033[96m";
const std::string VNBUI::Colors::BRIGHT_WHITE = "\033[97m";
const std::string VNBUI::Colors::VNB_BLUE = "\033[38;5;33m";
const std::string VNBUI::Colors::VNB_GOLD = "\033[38;5;220m";
const std::string VNBUI::Colors::VNB_SILVER = "\033[38;5;248m";

void VNBUI::init() {
#ifdef _WIN32
    // Enable ANSI escape codes on Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
    setupConsole();
}

void VNBUI::cleanup() {
    showCursor();
}

void VNBUI::setupConsole() {
    clear();
    hideCursor();
}

void VNBUI::clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void VNBUI::setCursor(int x, int y) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
#else
    printf("\033[%d;%dH", y, x);
#endif
}

void VNBUI::hideCursor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    printf("\033[?25l");
#endif
}

void VNBUI::showCursor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    printf("\033[?25h");
#endif
}

void VNBUI::drawBox(int x, int y, int width, int height, const std::string& title, const std::string& color) {
    // Draw top border
    setCursor(x, y);
    std::cout << color << "┌";
    for (int i = 0; i < width - 2; i++) std::cout << "─";
    std::cout << "┐" << Colors::RESET;
    
    // Draw title if provided
    if (!title.empty()) {
        setCursor(x + 2, y);
        std::cout << color << " " << title << " " << Colors::RESET;
    }
    
    // Draw sides
    for (int row = y + 1; row < y + height - 1; row++) {
        setCursor(x, row);
        std::cout << color << "│" << Colors::RESET;
        setCursor(x + width - 1, row);
        std::cout << color << "│" << Colors::RESET;
    }
    
    // Draw bottom border
    setCursor(x, y + height - 1);
    std::cout << color << "└";
    for (int i = 0; i < width - 2; i++) std::cout << "─";
    std::cout << "┘" << Colors::RESET;
}

void VNBUI::showLogo() {
    clear();
    std::cout << Colors::VNB_BLUE << R"(
    ╔═══════════════════════════════════════════════════╗
    ║                                                   ║
    ║        ██╗   ██╗███╗   ██╗██████╗                ║
    ║        ██║   ██║████╗  ██║██╔══██╗               ║
    ║        ██║   ██║██╔██╗ ██║██████╔╝               ║
    ║        ██║   ██║██║╚██╗██║██╔══██╗               ║
    ║        ╚██████╔╝██║ ╚████║██████╔╝               ║
    ║         ╚═════╝ ╚═╝  ╚═══╝╚═════╝                ║
    ║                                                   ║
    ║           VELELO NATIONAL BANK                    ║
    ║            Secure Banking System                  ║
    ║                                                   ║
    ╚═══════════════════════════════════════════════════╝
    )" << Colors::RESET << std::endl;
}

void VNBUI::showWelcome() {
    clear();
    showLogo();
    
    drawLine(60);
    drawCentered("Welcome to VNB Digital Banking", 60, Colors::BRIGHT_CYAN);
    drawCentered("Your Trust, Our Commitment", 60, Colors::VNB_GOLD);
    drawLine(60);
    
    std::cout << std::endl << std::endl;
}

void VNBUI::showLoading(int seconds) {
    std::cout << Colors::CYAN << "Loading ";
    for (int i = 0; i < 3; i++) {
        std::cout << ".";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << Colors::RESET << std::endl;
}

void VNBUI::showMessage(const std::string& msg, const std::string& type) {
    std::string color = Colors::CYAN;
    std::string prefix = "[INFO] ";
    
    if (type == "success") {
        color = Colors::BRIGHT_GREEN;
        prefix = "✓ ";
    } else if (type == "error") {
        color = Colors::BRIGHT_RED;
        prefix = "✗ ";
    } else if (type == "warning") {
        color = Colors::BRIGHT_YELLOW;
        prefix = "⚠ ";
    }
    
    std::cout << color << prefix << msg << Colors::RESET << std::endl;
}

std::string VNBUI::getInput(const std::string& prompt, const std::string& color) {
    std::cout << color << "> " << prompt << Colors::RESET;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string VNBUI::getSecureInput(const std::string& prompt) {
    std::cout << Colors::CYAN << "> " << prompt << Colors::RESET;
    
#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
    
    std::string input;
    std::getline(std::cin, input);
    
    SetConsoleMode(hStdin, mode);
    std::cout << std::endl;
    return input;
#else
    // For Linux/Mac - simplified
    std::string input;
    std::getline(std::cin, input);
    return input;
#endif
}

int VNBUI::getMenuChoice(const std::string& title, const std::vector<std::string>& options) {
    clear();
    drawBox(20, 5, 40, options.size() + 6, title, Colors::BRIGHT_BLUE);
    
    for (size_t i = 0; i < options.size(); i++) {
        setCursor(22, 7 + i);
        std::cout << Colors::BRIGHT_CYAN << (i + 1) << ". " 
                  << Colors::WHITE << options[i] << Colors::RESET;
    }
    
    setCursor(22, 7 + options.size() + 1);
    std::string choiceStr = getInput("Select: ", Colors::BRIGHT_CYAN);
    
    try {
        return std::stoi(choiceStr);
    } catch (...) {
        return -1;
    }
}

void VNBUI::showBalance(double amount) {
    std::cout << Colors::VNB_GOLD << "💰 Current Balance: " 
              << Colors::BRIGHT_GREEN << formatCurrency(amount) 
              << Colors::RESET << std::endl;
}

std::string VNBUI::formatCurrency(double amount) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << "$" << amount;
    return ss.str();
}

std::string VNBUI::formatDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void VNBUI::delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void VNBUI::drawLine(int length, const std::string& color) {
    std::cout << color;
    for (int i = 0; i < length; i++) std::cout << "═";
    std::cout << Colors::RESET << std::endl;
}

void VNBUI::drawCentered(const std::string& text, int width, const std::string& color) {
    int padding = (width - text.length()) / 2;
    std::cout << color;
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << text;
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << Colors::RESET << std::endl;
}

int VNBUI::consoleWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    return 80; // Default
#endif
}

int VNBUI::consoleHeight() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    return 24; // Default
#endif
}

void VNBUI::showAccountInfo(const std::string& accNum, const std::string& name, 
                          double balance, const std::string& type) {
    drawBox(15, 5, 50, 10, "ACCOUNT INFORMATION", Colors::BRIGHT_BLUE);
    
    setCursor(17, 7);
    std::cout << Colors::CYAN << "Account: " << Colors::WHITE << accNum << Colors::RESET;
    
    setCursor(17, 8);
    std::cout << Colors::CYAN << "Holder: " << Colors::WHITE << name << Colors::RESET;
    
    setCursor(17, 9);
    std::cout << Colors::CYAN << "Type: " << Colors::WHITE << type << Colors::RESET;
    
    setCursor(17, 10);
    std::cout << Colors::CYAN << "Balance: " << Colors::BRIGHT_GREEN 
              << formatCurrency(balance) << Colors::RESET;
    
    setCursor(17, 12);
    std::cout << Colors::VNB_GOLD << "✓ Account is active and in good standing" 
              << Colors::RESET;
}