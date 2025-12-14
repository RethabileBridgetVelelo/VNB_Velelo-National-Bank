#include "Transaction.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>

Transaction::Transaction(const std::string& accNum, Type t, double amt, 
                        const std::string& desc, double newBalance)
    : accountNumber(accNum), type(t), amount(amt), 
      description(desc), balanceAfter(newBalance) {
    
    id = generateId();
    
    // Get current date/time
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    dateTime = ss.str();
}

std::string Transaction::generateId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000, 999999);
    
    std::stringstream ss;
    ss << "TXN" << std::setw(6) << std::setfill('0') << dis(gen);
    return ss.str();
}

std::string Transaction::toString() const {
    std::stringstream ss;
    ss << dateTime << " | " 
       << getTypeString() << " | "
       << (type == DEPOSIT || type == TRANSFER_IN ? "+" : "-") << "$"
       << std::fixed << std::setprecision(2) << amount
       << " | Balance: $" << balanceAfter;
    
    if (!description.empty()) {
        ss << " | " << description;
    }
    
    return ss.str();
}

std::string Transaction::getTypeString() const {
    switch(type) {
        case DEPOSIT: return "DEPOSIT";
        case WITHDRAWAL: return "WITHDRAWAL";
        case TRANSFER_IN: return "TRANSFER IN";
        case TRANSFER_OUT: return "TRANSFER OUT";
        default: return "UNKNOWN";
    }
}