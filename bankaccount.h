#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#include "user.h"

class BankAccount{
public:
    virtual ~BankAccount() = default;

    void AccountMenu(User* user);

    virtual void deposit(User* user, double amount) = 0;
    virtual void withdraw(User* user, double amount) = 0;
    virtual std::string getAccountType() const = 0;
};

#endif