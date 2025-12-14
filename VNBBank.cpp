#include "VNBBank.h"
#include <fstream>
#include <sstream>
#include <iostream>

VNBBank::VNBBank(const std::string& name) 
    : bankName(name), accountsCreated(0) {}

std::shared_ptr<Account> VNBBank::createAccount(const std::string& holder, 
                                               const std::string& pin,
                                               const std::string& type,
                                               double initialDeposit) {
    if (pin.length() != 4) {
        return nullptr;
    }
    
    auto account = std::make_shared<Account>(holder, pin, type, initialDeposit);
    accounts[account->getAccountNumber()] = account;
    accountsCreated++;
    
    if (initialDeposit > 0) {
        deposit(account->getAccountNumber(), initialDeposit);
    }
    
    return account;
}

std::shared_ptr<Account> VNBBank::getAccount(const std::string& accNum) {
    auto it = accounts.find(accNum);
    if (it != accounts.end()) {
        return it->second;
    }
    return nullptr;
}

bool VNBBank::deposit(const std::string& accNum, double amount) {
    auto account = getAccount(accNum);
    if (!account || amount <= 0) return false;
    
    if (account->deposit(amount)) {
        Transaction t(accNum, Transaction::DEPOSIT, amount, 
                     "Bank Deposit", account->getBalance());
        recordTransaction(t);
        return true;
    }
    return false;
}

bool VNBBank::withdraw(const std::string& accNum, const std::string& pin, double amount) {
    auto account = getAccount(accNum);
    if (!account || amount <= 0) return false;
    
    if (account->withdraw(amount, pin)) {
        Transaction t(accNum, Transaction::WITHDRAWAL, amount, 
                     "ATM Withdrawal", account->getBalance());
        recordTransaction(t);
        return true;
    }
    return false;
}

bool VNBBank::transfer(const std::string& fromAcc, const std::string& toAcc,
                      const std::string& pin, double amount) {
    auto fromAccount = getAccount(fromAcc);
    auto toAccount = getAccount(toAcc);
    
    if (!fromAccount || !toAccount || amount <= 0) {
        return false;
    }
    
    if (!fromAccount->verifyPin(pin)) {
        return false;
    }
    
    // Withdraw from source
    if (fromAccount->withdraw(amount, pin)) {
        // Deposit to destination
        toAccount->deposit(amount);
        
        // Record transactions
        Transaction t1(fromAcc, Transaction::TRANSFER_OUT, amount, 
                      "Transfer to " + toAccount->getMaskedNumber(), 
                      fromAccount->getBalance());
        recordTransaction(t1);
        
        Transaction t2(toAcc, Transaction::TRANSFER_IN, amount,
                      "Transfer from " + fromAccount->getMaskedNumber(),
                      toAccount->getBalance());
        recordTransaction(t2);
        
        return true;
    }
    
    return false;
}

bool VNBBank::changePin(const std::string& accNum, const std::string& oldPin,
                       const std::string& newPin) {
    auto account = getAccount(accNum);
    if (!account) return false;
    
    return account->changePin(oldPin, newPin);
}

double VNBBank::getTotalDeposits() const {
    double total = 0;
    for (const auto& pair : accounts) {
        total += pair.second->getBalance();
    }
    return total;
}

int VNBBank::getAccountCount() const {
    return accounts.size();
}

std::vector<std::string> VNBBank::getAllAccountNumbers() const {
    std::vector<std::string> numbers;
    for (const auto& pair : accounts) {
        numbers.push_back(pair.first);
    }
    return numbers;
}

std::vector<std::shared_ptr<Account>> VNBBank::findAccountsByName(const std::string& name) {
    std::vector<std::shared_ptr<Account>> results;
    for (const auto& pair : accounts) {
        if (pair.second->getAccountHolder().find(name) != std::string::npos) {
            results.push_back(pair.second);
        }
    }
    return results;
}

void VNBBank::recordTransaction(const Transaction& t) {
    transactions.push_back(t);
}

bool VNBBank::validateAccount(const std::string& accNum) {
    return accounts.find(accNum) != accounts.end();
}

bool VNBBank::closeAccount(const std::string& accNum, const std::string& pin) {
    auto account = getAccount(accNum);
    if (!account || !account->verifyPin(pin)) {
        return false;
    }
    
    account->deactivate();
    return true;
}

void VNBBank::saveToFile(const std::string& filename) {
    // Simplified file saving
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "VNB Bank Data File\n";
        file << "Accounts: " << accounts.size() << "\n";
        file << "Total Balance: $" << getTotalDeposits() << "\n";
        file.close();
    }
}

void VNBBank::loadFromFile(const std::string& filename) {
    // Simplified file loading
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Basic parsing would go here
        }
        file.close();
    }
}