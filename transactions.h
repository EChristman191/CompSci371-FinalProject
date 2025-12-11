#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <string>
#include <vector>
#include "user.h"

namespace Transactions {

    struct Transaction {
        std::string type;
        double      amount;
        std::string time;
        double      newAmount;
    };

    std::vector<Transaction> parse(const std::string& history);

    void print(const User* user);

    // Adds an "initial deposit" log entry (does not change balance).
    void addInitialDeposit(User* user, double amount);
}

#endif
