#ifndef WITHDRAW_H
#define WITHDRAW_H

#include "user.h"

// Handles withdrawal logic for a user's account.
class Withdraw {
public:
    // Attempts to withdraw `amount` from the given user's balance.
    // - Does nothing if amount <= 0
    // - Does nothing (and prints an error) if it would take balance below 0.00
    // - On success: updates balance, appends to transaction history, and saves to file.
    static void makeWithdraw(User* user, double amount);
};

#endif
