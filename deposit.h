#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <string>
#include "user.h"

class Deposit {
public:
    // Deposits `amount` into this user's account and logs the transaction.
    static void makeDeposit(User* user, double amount);
};

#endif
