#include "VNBBank.h"
#include "VNBUI.h"
#include <iostream>
#include <memory>
#include <vector>

class VNBApplication {
private:
    std::unique_ptr<VNBBank> bank;
    std::shared_ptr<Account> currentAccount;
    
public:
    VNBApplication() {
        bank = std::make_unique<VNBBank>();
        VNBUI::init();
    }
    
    ~VNBApplication() {
        VNBUI::cleanup();
    }
    
    void run() {
        VNBUI::showWelcome();
        VNBUI::delay(1000);
        
        bool running = true;
        while (running) {
            int choice = showMainMenu();
            
            switch(choice) {
                case 1: login(); break;
                case 2: createAccount(); break;
                case 3: adminAccess(); break;
                case 4: aboutVNB(); break;
                case 5: 
                    VNBUI::showMessage("Thank you for banking with VNB!", "success");
                    running = false;
                    break;
                default:
                    VNBUI::showMessage("Invalid selection", "error");
                    break;
            }
            
            if (running && choice != 5) {
                VNBUI::delay(2000);
            }
        }
    }
    
private:
    int showMainMenu() {
        std::vector<std::string> options = {
            "Customer Login",
            "Open New Account",
            "Admin Access",
            "About VNB",
            "Exit"
        };
        
        return VNBUI::getMenuChoice("VNB MAIN MENU", options);
    }
    
    void login() {
        VNBUI::clear();
        VNBUI::drawBox(20, 5, 40, 8, "CUSTOMER LOGIN", VNBUI::Colors::BRIGHT_BLUE);
        
        VNBUI::setCursor(22, 7);
        std::string accNum = VNBUI::getInput("Account Number: ");
        
        VNBUI::setCursor(22, 8);
        std::string pin = VNBUI::getSecureInput("PIN: ");
        
        currentAccount = bank->getAccount(accNum);
        if (currentAccount && currentAccount->verifyPin(pin)) {
            VNBUI::showMessage("Login successful!", "success");
            showCustomerDashboard();
        } else {
            VNBUI::showMessage("Invalid account or PIN", "error");
            currentAccount.reset();
        }
    }
    
    void createAccount() {
        VNBUI::clear();
        VNBUI::drawBox(15, 3, 50, 14, "OPEN NEW ACCOUNT", VNBUI::Colors::BRIGHT_GREEN);
        
        VNBUI::setCursor(17, 5);
        std::string name = VNBUI::getInput("Full Name: ");
        
        VNBUI::setCursor(17, 7);
        std::string pin = VNBUI::getSecureInput("Create 4-digit PIN: ");
        
        if (pin.length() != 4) {
            VNBUI::showMessage("PIN must be 4 digits", "error");
            return;
        }
        
        // Account type selection
        VNBUI::setCursor(17, 9);
        int typeChoice = VNBUI::getMenuChoice("Select Account Type", {
            "Standard Savings",
            "Premium Checking",
            "Business Account"
        });
        
        std::string accountType;
        switch(typeChoice) {
            case 1: accountType = "Savings"; break;
            case 2: accountType = "Checking"; break;
            case 3: accountType = "Business"; break;
            default: accountType = "Savings";
        }
        
        VNBUI::setCursor(17, 15);
        std::string depositStr = VNBUI::getInput("Initial Deposit $: ");
        
        double initialDeposit = 0.0;
        try {
            initialDeposit = std::stod(depositStr);
            if (initialDeposit < 10) {
                VNBUI::showMessage("Minimum deposit is $10", "error");
                return;
            }
        } catch (...) {
            VNBUI::showMessage("Invalid amount", "error");
            return;
        }
        
        VNBUI::showLoading(2);
        
        auto newAccount = bank->createAccount(name, pin, accountType, initialDeposit);
        if (newAccount) {
            VNBUI::clear();
            VNBUI::drawBox(15, 5, 50, 12, "ACCOUNT CREATED", VNBUI::Colors::BRIGHT_GREEN);
            
            VNBUI::setCursor(17, 7);
            std::cout << VNBUI::Colors::GREEN << "✓ Account Number: " 
                      << VNBUI::Colors::BRIGHT_WHITE << newAccount->getAccountNumber() 
                      << VNBUI::Colors::RESET << std::endl;
            
            VNBUI::setCursor(17, 9);
            std::cout << VNBUI::Colors::GREEN << "✓ Account Holder: " 
                      << VNBUI::Colors::BRIGHT_WHITE << name 
                      << VNBUI::Colors::RESET << std::endl;
            
            VNBUI::setCursor(17, 11);
            std::cout << VNBUI::Colors::GREEN << "✓ Initial Balance: " 
                      << VNBUI::Colors::BRIGHT_GREEN << VNBUI::formatCurrency(initialDeposit) 
                      << VNBUI::Colors::RESET << std::endl;
            
            VNBUI::setCursor(17, 13);
            std::cout << VNBUI::Colors::YELLOW << "⚠ Keep your account details secure!" 
                      << VNBUI::Colors::RESET << std::endl;
            
            VNBUI::setCursor(17, 15);
            VNBUI::getInput("Press Enter to continue...");
        } else {
            VNBUI::showMessage("Failed to create account", "error");
        }
    }
    
    void showCustomerDashboard() {
        bool inDashboard = true;
        
        while (inDashboard && currentAccount) {
            VNBUI::clear();
            VNBUI::drawBox(5, 2, 70, 5, "VNB CUSTOMER DASHBOARD", VNBUI::Colors::BRIGHT_BLUE);
            
            // Account info
            VNBUI::setCursor(7, 4);
            std::cout << VNBUI::Colors::CYAN << "Welcome, " 
                      << VNBUI::Colors::BRIGHT_WHITE << currentAccount->getAccountHolder() 
                      << VNBUI::Colors::RESET;
            
            VNBUI::setCursor(50, 4);
            std::cout << VNBUI::Colors::CYAN << "Account: " 
                      << VNBUI::Colors::WHITE << currentAccount->getMaskedNumber() 
                      << VNBUI::Colors::RESET;
            
            // Balance display
            VNBUI::setCursor(7, 6);
            VNBUI::showBalance(currentAccount->getBalance());
            
            // Menu
            int choice = VNBUI::getMenuChoice("BANKING SERVICES", {
                "Deposit Funds",
                "Withdraw Cash",
                "Transfer Money",
                "View Transactions",
                "Account Information",
                "Change PIN",
                "Logout"
            });
            
            switch(choice) {
                case 1: depositFunds(); break;
                case 2: withdrawCash(); break;
                case 3: transferMoney(); break;
                case 4: viewTransactions(); break;
                case 5: showAccountInfo(); break;
                case 6: changePin(); break;
                case 7: 
                    VNBUI::showMessage("Logged out successfully", "success");
                    currentAccount.reset();
                    inDashboard = false;
                    break;
            }
            
            if (inDashboard && choice != 7) {
                VNBUI::delay(2000);
            }
        }
    }
    
    void depositFunds() {
        VNBUI::clear();
        VNBUI::drawBox(20, 5, 40, 8, "DEPOSIT", VNBUI::Colors::BRIGHT_GREEN);
        
        VNBUI::setCursor(22, 7);
        std::string amountStr = VNBUI::getInput("Amount $: ");
        
        try {
            double amount = std::stod(amountStr);
            if (bank->deposit(currentAccount->getAccountNumber(), amount)) {
                VNBUI::showMessage("Deposit successful", "success");
                currentAccount = bank->getAccount(currentAccount->getAccountNumber());
            } else {
                VNBUI::showMessage("Deposit failed", "error");
            }
        } catch (...) {
            VNBUI::showMessage("Invalid amount", "error");
        }
    }
    
    void withdrawCash() {
        VNBUI::clear();
        VNBUI::drawBox(20, 5, 40, 8, "WITHDRAW", VNBUI::Colors::BRIGHT_YELLOW);
        
        VNBUI::setCursor(22, 7);
        std::string pin = VNBUI::getSecureInput("Enter PIN: ");
        
        VNBUI::setCursor(22, 8);
        std::string amountStr = VNBUI::getInput("Amount $: ");
        
        try {
            double amount = std::stod(amountStr);
            if (bank->withdraw(currentAccount->getAccountNumber(), pin, amount)) {
                VNBUI::showMessage("Withdrawal successful", "success");
                currentAccount = bank->getAccount(currentAccount->getAccountNumber());
            } else {
                VNBUI::showMessage("Withdrawal failed", "error");
            }
        } catch (...) {
            VNBUI::showMessage("Invalid amount", "error");
        }
    }
    
    void transferMoney() {
        VNBUI::clear();
        VNBUI::drawBox(20, 5, 40, 10, "TRANSFER", VNBUI::Colors::BRIGHT_CYAN);
        
        VNBUI::setCursor(22, 7);
        std::string toAccount = VNBUI::getInput("Destination Account: ");
        
        VNBUI::setCursor(22, 8);
        std::string pin = VNBUI::getSecureInput("Your PIN: ");
        
        VNBUI::setCursor(22, 9);
        std::string amountStr = VNBUI::getInput("Amount $: ");
        
        try {
            double amount = std::stod(amountStr);
            if (bank->transfer(currentAccount->getAccountNumber(), toAccount, pin, amount)) {
                VNBUI::showMessage("Transfer successful", "success");
                currentAccount = bank->getAccount(currentAccount->getAccountNumber());
            } else {
                VNBUI::showMessage("Transfer failed", "error");
            }
        } catch (...) {
            VNBUI::showMessage("Invalid amount", "error");
        }
    }
    
    void viewTransactions() {
        VNBUI::clear();
        VNBUI::drawBox(10, 3, 60, 20, "TRANSACTION HISTORY", VNBUI::Colors::BRIGHT_MAGENTA);
        
        auto transactions = currentAccount->getTransactionHistory();
        if (transactions.empty()) {
            VNBUI::setCursor(15, 5);
            std::cout << VNBUI::Colors::YELLOW << "No transactions yet" 
                      << VNBUI::Colors::RESET << std::endl;
        } else {
            int y = 5;
            for (const auto& txn : transactions) {
                VNBUI::setCursor(12, y);
                std::cout << VNBUI::Colors::WHITE << txn << VNBUI::Colors::RESET;
                y++;
                if (y > 20) break;
            }
        }
        
        VNBUI::setCursor(12, 22);
        VNBUI::getInput("Press Enter to continue...");
    }
    
    void showAccountInfo() {
        VNBUI::clear();
        VNBUI::showAccountInfo(
            currentAccount->getAccountNumber(),
            currentAccount->getAccountHolder(),
            currentAccount->getBalance(),
            currentAccount->getAccountType()
        );
        
        VNBUI::setCursor(17, 14);
        VNBUI::getInput("Press Enter to continue...");
    }
    
    void changePin() {
        VNBUI::clear();
        VNBUI::drawBox(20, 5, 40, 10, "CHANGE PIN", VNBUI::Colors::BRIGHT_MAGENTA);
        
        VNBUI::setCursor(22, 7);
        std::string oldPin = VNBUI::getSecureInput("Current PIN: ");
        
        VNBUI::setCursor(22, 8);
        std::string newPin = VNBUI::getSecureInput("New PIN (4 digits): ");
        
        VNBUI::setCursor(22, 9);
        std::string confirmPin = VNBUI::getSecureInput("Confirm New PIN: ");
        
        if (newPin != confirmPin) {
            VNBUI::showMessage("PINs don't match", "error");
            return;
        }
        
        if (bank->changePin(currentAccount->getAccountNumber(), oldPin, newPin)) {
            VNBUI::showMessage("PIN changed successfully", "success");
        } else {
            VNBUI::showMessage("Failed to change PIN", "error");
        }
    }
    
    void adminAccess() {
        VNBUI::showMessage("Admin access requires special authorization", "warning");
    }
    
    void aboutVNB() {
        VNBUI::clear();
        VNBUI::drawBox(10, 3, 60, 15, "ABOUT VELELO NATIONAL BANK", VNBUI::Colors::VNB_BLUE);
        
        VNBUI::setCursor(12, 5);
        std::cout << VNBUI::Colors::BRIGHT_WHITE 
                  << "Velelo National Bank - Trusted Since 1985" 
                  << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(12, 7);
        std::cout << VNBUI::Colors::CYAN 
                  << "Mission: To provide secure, innovative banking" 
                  << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(12, 9);
        std::cout << VNBUI::Colors::WHITE 
                  << "Services:" 
                  << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(14, 11);
        std::cout << VNBUI::Colors::GREEN << "✓ Personal Banking" << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(14, 12);
        std::cout << VNBUI::Colors::GREEN << "✓ Business Accounts" << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(14, 13);
        std::cout << VNBUI::Colors::GREEN << "✓ Digital Banking" << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(14, 14);
        std::cout << VNBUI::Colors::GREEN << "✓ 24/7 Customer Support" << VNBUI::Colors::RESET;
        
        VNBUI::setCursor(12, 16);
        VNBUI::getInput("Press Enter to continue...");
    }
};

int main() {
    try {
        VNBApplication app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}