#include "withdraw.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

static std::string getWithdrawTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
    localtime_s(&tm, &t); // Windows-safe localtime

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");  // Same format as deposit
    return oss.str();
}

void Withdraw::makeWithdraw(User* user, double amount)
{
    if (!user) {
        std::cerr << "Error: user pointer is null in Withdraw::makeWithdraw\n";
        return;
    }

    if (amount <= 0.0) {
        std::cout << "Amount must be greater than 0.\n";
        return;
    }

    double currentBalance = user->getBalance();

    // Make sure the withdrawal does NOT bring the balance below 0.00
    if (amount > currentBalance) {
        std::cout << "Insufficient funds. You cannot withdraw more than your current balance.\n";
        std::cout << "Current Balance: " << currentBalance << std::endl;
        return;
    }

    double newBalance = currentBalance - amount;
    user->setBalance(newBalance);

    // Log format:
    // WITHDRAW:<amount> TIME:<YYYY/MM/DD HH:MM:SS> NEW_AMOUNT:<updated_balance>|
    std::ostringstream entry;
    entry << "WITHDRAW:" << amount
          << " TIME:" << getWithdrawTimestamp()
          << " NEW_AMOUNT:" << newBalance
          << "|";

    // Append to user's transaction history string
    std::string tx = user->getTransactions();
    tx += entry.str();
    user->setTransactions(tx);

    // Persist changes to the user's file
    user->saveToFile();

    std::cout << "\n--- Withdrawal Successful ---\n";
    std::cout << "Withdrawn: " << amount << "\n";
    std::cout << "New Balance: " << newBalance << "\n\n";
}
