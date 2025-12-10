#pragma once

#include "bankaccount.h"

class CheckingAccount : public BankAccount
{
public:
    virtual ~CheckingAccount() = default;

    void deposit(User* user, double amount) override;
    void withdraw(User* user, double amount) override;
    std::string getAccountType() const override;
};
