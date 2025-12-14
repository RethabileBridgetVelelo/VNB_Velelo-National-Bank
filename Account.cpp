#include "Account.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

Account::Account(const std::string& holder, const std::string& pinCode, 
                const std::string& type, double initialBalance)
    : accountHolder(holder), pin(pinCode), accountType(type), 
      balance(initialBalance), isActive(true) {
    
    accountNumber = generateAccountNumber();
    creationDate = getCurrentDateTime();
}

std::string Account::generateAccountNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000000, 99999999);
    
    std::stringstream ss;
    ss << "VNB" << std::setw(8) << std::setfill('0') << dis(gen);
    return ss.str();
}

bool Account::deposit(double amount) {
    if (amount <= 0) return false;
    
    balance += amount;
    
    std::stringstream ss;
    ss << getCurrentDateTime() << " | DEPOSIT | +$" 
       << std::fixed << std::setprecision(2) << amount 
       << " | Balance: $" << balance;
    transactionHistory.push_back(ss.str());
    
    return true;
}

bool Account::withdraw(double amount, const std::string& inputPin) {
    if (!verifyPin(inputPin) || amount <= 0 || amount > balance) {
        return false;
    }
    
    balance -= amount;
    
    std::stringstream ss;
    ss << getCurrentDateTime() << " | WITHDRAWAL | -$" 
       << std::fixed << std::setprecision(2) << amount 
       << " | Balance: $" << balance;
    transactionHistory.push_back(ss.str());
    
    return true;
}

bool Account::verifyPin(const std::string& inputPin) const {
    return pin == inputPin;
}

bool Account::changePin(const std::string& oldPin, const std::string& newPin) {
    if (verifyPin(oldPin) && newPin.length() == 4) {
        pin = newPin;
        return true;
    }
    return false;
}

std::string Account::getMaskedNumber() const {
    if (accountNumber.length() < 8) return accountNumber;
    return accountNumber.substr(0, 3) + "****" + accountNumber.substr(accountNumber.length() - 4);
}

std::string Account::getFormattedBalance() const {
    std::stringstream ss;
    ss << "$" << std::fixed << std::setprecision(2) << balance;
    return ss.str();
}

std::string Account::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}