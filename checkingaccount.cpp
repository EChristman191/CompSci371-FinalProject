#include "CheckingAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "user.h"

void CheckingAccount::deposit(User* user, double amount)
{
    // Delegate the deposit operation to the Deposit helper class.
    // This allows centralized deposit logic shared by all account types.
    Deposit::makeDeposit(user, amount);
}

void CheckingAccount::withdraw(User* user, double amount)
{
    // Delegate the withdrawal operation to the Withdraw helper class.
    // Checking accounts follow the same withdrawal rules unless customized.
    Withdraw::makeWithdraw(user, amount);
}

std::string CheckingAccount::getAccountType() const
{
    // Return a string identifying this account type.
    return "Checking";
}
