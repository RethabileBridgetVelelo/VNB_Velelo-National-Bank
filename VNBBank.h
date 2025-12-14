#ifndef VNBBANK_H
#define VNBBANK_H

#include "Account.h"
#include "Transaction.h"
#include <vector>
#include <memory>
#include <map>

class VNBBank {
private:
    std::map<std::string, std::shared_ptr<Account>> accounts;
    std::vector<Transaction> transactions;
    std::string bankName;
    int accountsCreated;
    
public:
    VNBBank(const std::string& name = "Velelo National Bank");
    
    // Account Management
    std::shared_ptr<Account> createAccount(const std::string& holder, 
                                          const std::string& pin,
                                          const std::string& type = "Savings",
                                          double initialDeposit = 0.0);
    
    std::shared_ptr<Account> getAccount(const std::string& accNum);
    bool closeAccount(const std::string& accNum, const std::string& pin);
    
    // Transactions
    bool deposit(const std::string& accNum, double amount);
    bool withdraw(const std::string& accNum, const std::string& pin, double amount);
    bool transfer(const std::string& fromAcc, const std::string& toAcc,
                 const std::string& pin, double amount);
    
    // Account Operations
    bool changePin(const std::string& accNum, const std::string& oldPin,
                  const std::string& newPin);
    
    // Bank Information
    double getTotalDeposits() const;
    int getAccountCount() const;
    std::vector<std::string> getAllAccountNumbers() const;
    
    // Search
    std::vector<std::shared_ptr<Account>> findAccountsByName(const std::string& name);
    
    // Data Management
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
private:
    void recordTransaction(const Transaction& t);
    bool validateAccount(const std::string& accNum);
};

#endif // VNBBANK_H