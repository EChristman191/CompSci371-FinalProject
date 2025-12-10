#pragma once

#include "bankaccount.h"

class SavingAccount : public BankAccount
{
public:
    virtual ~SavingAccount() = default;

    void deposit(User* user, double amount) override;
    void withdraw(User* user, double amount) override;
    std::string getAccountType() const override;
};
