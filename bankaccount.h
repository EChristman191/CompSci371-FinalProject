#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "user.h"

// Abstract base class representing a generic bank account.
// This class provides the interface (pure virtual functions) for any
// derived account type such as CheckingAccount or SavingAccount.
class BankAccount {
public:

    // Virtual destructor to ensure proper cleanup of derived classes.
    virtual ~BankAccount() = default;

    // Displays the account menu and handles user actions such as
    // deposit, withdrawal, viewing activity, etc.
    void AccountMenu(User* user);

    // Pure virtual function: must be implemented in derived classes.
    // Handles depositing money into the user's account.
    virtual void deposit(User* user, double amount) = 0;

    // Pure virtual function: must be implemented in derived classes.
    // Handles withdrawing money from the user's account.
    virtual void withdraw(User* user, double amount) = 0;

    // Pure virtual function that returns the type of the account
    // (e.g. "Checking" or "Saving"). Implemented by derived classes.
    virtual std::string getAccountType() const = 0;
};

#endif
