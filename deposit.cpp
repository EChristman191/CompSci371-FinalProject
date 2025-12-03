#include "deposit.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

static std::string getTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
    localtime_s(&tm, &t);  // Windows-safe localtime

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");  
    return oss.str();
}

void Deposit::makeDeposit(User* user, double amount)
{
    if (!user) return;

    if (amount <= 0) {
        std::cout << "Amount must be greater than 0.\n";
        return;
    }

    double oldBalance = user->getBalance();
    double newBalance = oldBalance + amount;

    user->setBalance(newBalance);

    // Build EXACT required format:
    // DEPOSIT:<amount> TIME:<YYYY/MM/DD HH:MM:SS> NEW_AMOUNT:<updated_balance>|
    std::ostringstream entry;
    entry << "DEPOSIT:" << amount
          << " TIME:" << getTimestamp()
          << " NEW_AMOUNT:" << newBalance
          << "|";

    // Append to existing history
    std::string tx = user->getTransactions();
    tx += entry.str();
    user->setTransactions(tx);

    // Persist changes to user file
    user->saveToFile();

    std::cout << "\n--- Deposit Successful ---\n";
    std::cout << "Deposited: " << amount << "\n";
    std::cout << "New Balance: " << newBalance << "\n\n";
}
