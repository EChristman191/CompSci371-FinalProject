#include "withdraw.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

// Generate a timestamp for withdrawal entries.
// Uses the same formatting as the deposit timestamp for consistency.
static std::string getWithdrawTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();               // get current system time
    std::time_t t = system_clock::to_time_t(now); // convert to time_t

    std::tm tm{};
    localtime_s(&tm, &t); // Windows-safe version of localtime()

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S"); // YYYY/MM/DD HH:MM:SS
    return oss.str();
}

void Withdraw::makeWithdraw(User* user, double amount)
{
    // Safety check: ensure pointer is valid
    if (!user) {
        std::cerr << "Error: user pointer is null in Withdraw::makeWithdraw\n";
        return;
    }

    // Withdrawal amount must be positive
    if (amount <= 0.0) {
        std::cout << "Amount must be greater than 0.\n";
        return;
    }

    double currentBalance = user->getBalance();

    // Ensure the user cannot withdraw more than they have
    if (amount > currentBalance) {
        std::cout << "Insufficient funds. You cannot withdraw more than your current balance.\n";
        std::cout << "Current Balance: " << currentBalance << std::endl;
        return;
    }

    // Calculate updated account balance
    double newBalance = currentBalance - amount;
    user->setBalance(newBalance);

    // Build transaction log entry using required format:
    // WITHDRAW:<amount> TIME:<timestamp> NEW_AMOUNT:<newBalance>|
    std::ostringstream entry;
    entry << "WITHDRAW:" << amount
          << " TIME:" << getWithdrawTimestamp()
          << " NEW_AMOUNT:" << newBalance
          << "|";

    // Append to user's existing transaction history string
    std::string tx = user->getTransactions();
    tx += entry.str();
    user->setTransactions(tx);

    // Save updated user data (balance + history) to the file
    user->saveToFile();

    // Inform the user of success
    std::cout << "\n--- Withdrawal Successful ---\n";
    std::cout << "Withdrawn: " << amount << "\n";
    std::cout << "New Balance: " << newBalance << "\n\n";
}
