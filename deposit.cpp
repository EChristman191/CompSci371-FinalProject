#include "deposit.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

// Helper function to generate a timestamp in the format:
// YYYY/MM/DD HH:MM:SS
static std::string getTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();                // get current time
    std::time_t t = system_clock::to_time_t(now);  // convert to time_t

    std::tm tm{};
    localtime_s(&tm, &t);  // Windows-safe conversion to local time

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");  // format timestamp
    return oss.str();
}

void Deposit::makeDeposit(User* user, double amount)
{
    if (!user) return; // safety check

    // Ensure positive deposit amount
    if (amount <= 0) {
        std::cout << "Amount must be greater than 0.\n";
        return;
    }

    // Retrieve the current balance and compute the updated value
    double oldBalance = user->getBalance();
    double newBalance = oldBalance + amount;

    // Update the user's stored balance
    user->setBalance(newBalance);

    // Create a transaction entry in the required format:
    // DEPOSIT:<amount> TIME:<timestamp> NEW_AMOUNT:<updated_balance>|
    std::ostringstream entry;
    entry << "DEPOSIT:" << amount
          << " TIME:" << getTimestamp()
          << " NEW_AMOUNT:" << newBalance
          << "|";

    // Append entry to user's transaction history string
    std::string tx = user->getTransactions();
    tx += entry.str();
    user->setTransactions(tx);

    // Save updated data back to the user's file
    user->saveToFile();

    // User feedback for successful deposit
    std::cout << "\n--- Deposit Successful ---\n";
    std::cout << "Deposited: " << amount << "\n";
    std::cout << "New Balance: " << newBalance << "\n\n";
}
