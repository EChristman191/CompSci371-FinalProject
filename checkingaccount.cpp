#include "CheckingAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "user.h"   // or whatever header defines User

void CheckingAccount::deposit(User* user, double amount)
{
    // You can keep using your existing logic here
    Deposit::makeDeposit(user, amount);
}

void CheckingAccount::withdraw(User* user, double amount)
{
    Withdraw::makeWithdraw(user, amount);
}

std::string CheckingAccount::getAccountType() const
{
    return "Checking";
}
