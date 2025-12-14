#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
public:
    enum Type {
        DEPOSIT,
        WITHDRAWAL,
        TRANSFER_IN,
        TRANSFER_OUT
    };
    
private:
    std::string id;
    std::string accountNumber;
    Type type;
    double amount;
    std::string dateTime;
    std::string description;
    double balanceAfter;
    
public:
    Transaction(const std::string& accNum, Type t, double amt, 
               const std::string& desc = "", double newBalance = 0.0);
    
    // Getters
    std::string getId() const { return id; }
    std::string getAccountNumber() const { return accountNumber; }
    Type getType() const { return type; }
    double getAmount() const { return amount; }
    std::string getDateTime() const { return dateTime; }
    std::string getDescription() const { return description; }
    double getBalanceAfter() const { return balanceAfter; }
    
    // Display
    std::string toString() const;
    std::string getTypeString() const;
    
private:
    std::string generateId();
};

#endif // TRANSACTION_H