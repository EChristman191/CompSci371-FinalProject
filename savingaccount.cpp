#include "SavingAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "user.h"   // or whatever header defines User

void SavingAccount::deposit(User* user, double amount)
{
    // Savings-specific deposit logic (interest, limits, etc.)
    Deposit::makeDeposit(user, amount);
}

void SavingAccount::withdraw(User* user, double amount)
{
    // Savings-specific rules (e.g. limits, penalties, etc.)
    Withdraw::makeWithdraw(user, amount);
}

std::string SavingAccount::getAccountType() const
{
    return "Savings";
}
