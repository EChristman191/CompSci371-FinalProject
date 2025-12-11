#include "SavingAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "user.h"   // ensures the User class is available

void SavingAccount::deposit(User* user, double amount)
{
    // Perform a deposit into a savings account.
    // This function could include savings-specific logic in the future
    // (interest bonuses, deposit limits, etc.)
    Deposit::makeDeposit(user, amount);
}

void SavingAccount::withdraw(User* user, double amount)
{
    // Withdraw from a savings account.
    // Savings accounts may impose withdrawal limits or penalties.
    // Currently handled by the Withdraw helper class.
    Withdraw::makeWithdraw(user, amount);
}

std::string SavingAccount::getAccountType() const
{
    // Identify this account type as "Saving"
    return "Saving";
}
