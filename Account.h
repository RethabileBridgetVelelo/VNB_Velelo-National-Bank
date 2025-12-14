#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <chrono>
#include <ctime>

class Account {
private:
    std::string accountNumber;
    std::string accountHolder;
    std::string pin;
    double balance;
    std::string accountType;
    std::string creationDate;
    bool isActive;
    std::vector<std::string> transactionHistory;

public:
    Account(const std::string& holder, const std::string& pinCode, 
           const std::string& type = "Savings", double initialBalance = 0.0);
    
    // Getters
    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountHolder() const { return accountHolder; }
    std::string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    std::string getCreationDate() const { return creationDate; }
    bool getIsActive() const { return isActive; }
    
    // Account Operations
    bool deposit(double amount);
    bool withdraw(double amount, const std::string& inputPin);
    bool verifyPin(const std::string& inputPin) const;
    bool changePin(const std::string& oldPin, const std::string& newPin);
    
    // Account Management
    void deactivate() { isActive = false; }
    void activate() { isActive = true; }
    void addTransaction(const std::string& transaction);
    std::vector<std::string> getTransactionHistory() const { return transactionHistory; }
    
    // Display
    std::string getMaskedNumber() const;
    std::string getFormattedBalance() const;
    
private:
    std::string generateAccountNumber();
    std::string getCurrentDateTime();
};

#endif // ACCOUNT_H