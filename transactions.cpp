#include "transactions.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Get timestamp in same format as deposit/withdraw
static std::string getCurrentTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
    localtime_s(&tm, &t); // Windows-safe

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
    return oss.str();
}

namespace Transactions {

    // Parse a single entry like:
    //  INITIAL_DEPOSIT:300.12 TIME:2025/12/10 17:17:10 NEW_AMOUNT:300.12
    //  DEPOSIT:421.91 TIME:2025/12/10 17:17:44 NEW_AMOUNT:722.03
    static bool parseEntry(const std::string& entryStr, Transaction& out)
    {
        if (entryStr.empty())
            return false;

        std::string s = entryStr;

        // 1. Get type token (before first ':')
        size_t colonPos = s.find(':');
        if (colonPos == std::string::npos)
            return false;

        std::string typeToken = s.substr(0, colonPos);

        if (typeToken == "DEPOSIT") {
            out.type = "Deposit";
        } else if (typeToken == "WITHDRAW") {
            out.type = "Withdraw";
        } else if (typeToken == "INITIAL_DEPOSIT") {
            out.type = "Initial Deposit";
        } else {
            // Unknown type
            return false;
        }

        // 2. Amount (after colon, up to next space)
        size_t amountStart = colonPos + 1;
        size_t amountEnd = s.find(' ', amountStart);
        if (amountEnd == std::string::npos)
            amountEnd = s.size();

        std::string amountStr = s.substr(amountStart, amountEnd - amountStart);
        try {
            out.amount = std::stod(amountStr);
        } catch (...) {
            return false;
        }

        // 3. TIME:...
        const std::string timeKey = "TIME:";
        size_t timePos = s.find(timeKey, amountEnd);
        if (timePos == std::string::npos)
            return false;

        size_t timeStart = timePos + timeKey.size(); // after "TIME:"
        // Skip possible space
        if (timeStart < s.size() && s[timeStart] == ' ')
            ++timeStart;

        // 4. NEW_AMOUNT:...
        const std::string newKey = "NEW_AMOUNT:";
        size_t newPos = s.find(newKey, timeStart);
        if (newPos == std::string::npos)
            return false;

        // Time string is from timeStart up to the char before "NEW_AMOUNT:"
        size_t timeEnd = newPos;
        // trim trailing space before NEW_AMOUNT:
        while (timeEnd > timeStart && s[timeEnd - 1] == ' ')
            --timeEnd;

        out.time = s.substr(timeStart, timeEnd - timeStart);

        // 5. Balance after (NEW_AMOUNT)
        size_t newAmountStart = newPos + newKey.size();
        // Skip possible space after NEW_AMOUNT:
        if (newAmountStart < s.size() && s[newAmountStart] == ' ')
            ++newAmountStart;

        std::string newAmountStr = s.substr(newAmountStart);
        try {
            out.newAmount = std::stod(newAmountStr);
        } catch (...) {
            return false;
        }

        return true;
    }

    std::vector<Transaction> parse(const std::string& history)
    {
        std::vector<Transaction> result;
        if (history.empty())
            return result;

        std::stringstream ss(history);
        std::string entry;

        // Entries separated by '|'
        while (std::getline(ss, entry, '|')) {
            if (entry.empty())
                continue;

            Transaction tx;
            if (parseEntry(entry, tx)) {
                result.push_back(tx);
            } else {
                // If one entry fails, we just skip it.
                // You can std::cerr here if you want.
            }
        }

        return result;
    }

    void print(const User* user)
    {
        if (!user) {
            std::cout << "No user loaded.\n";
            return;
        }

        // DEBUG: show raw string so you can see exactly what we're parsing
        std::string raw = user->getTransactions();
        std::cout << "[DEBUG] Raw transaction string: '" << raw << "'\n";

        auto transactions = parse(raw);
        if (transactions.empty()) {
            std::cout << "No transactions found for this account.\n";
            return;
        }

        std::cout << "\n--- Transaction History for "
                  << user->getFirstName() << " " << user->getLastName()
                  << " (" << user->getUsername() << ") ---\n\n";

        std::cout << std::left
                  << std::setw(18) << "Type"
                  << std::setw(20) << "Amount"
                  << std::setw(22) << "Time"
                  << std::setw(15) << "Balance After"
                  << "\n";

        std::cout << std::string(75, '-') << "\n";

        for (const auto& tx : transactions) {
            std::ostringstream amt;
            amt << std::fixed << std::setprecision(2) << "$" << tx.amount;

            std::ostringstream bal;
            bal << std::fixed << std::setprecision(2) << "$" << tx.newAmount;

            std::cout << std::left
                      << std::setw(18) << tx.type
                      << std::setw(20) << amt.str()
                      << std::setw(22) << tx.time
                      << std::setw(15) << bal.str()
                      << "\n";
        }

        std::cout << "\n";
    }

    void addInitialDeposit(User* user, double amount)
    {
        if (!user || amount <= 0.0)
            return;

        // Format: INITIAL_DEPOSIT:<amount> TIME:<timestamp> NEW_AMOUNT:<balance>|
        std::ostringstream entry;
        entry << "INITIAL_DEPOSIT:" << amount
              << " TIME:" << getCurrentTimestamp()
              << " NEW_AMOUNT:" << user->getBalance()
              << "|";

        std::string history = user->getTransactions();
        history += entry.str();
        user->setTransactions(history);
    }

} // namespace Transactions
