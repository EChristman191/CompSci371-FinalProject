#include "transactions.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Get timestamp in same format as deposit/withdraw.
// This ensures consistent time formatting across the system.
static std::string getCurrentTimestamp()
{
    using namespace std::chrono;

    auto now = system_clock::now();                  // current time
    std::time_t t = system_clock::to_time_t(now);    // convert to time_t

    std::tm tm{};
    localtime_s(&tm, &t); // Windows-safe localtime conversion

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");  // format the timestamp
    return oss.str();
}

namespace Transactions {

    // Parse a single transaction entry string such as:
    //   DEPOSIT:100 TIME:2025/12/10 17:17:10 NEW_AMOUNT:200|
    //   WITHDRAW:50 TIME:2025/12/11 12:45:01 NEW_AMOUNT:150|
    //
    // Converts the raw entry string into a structured Transaction object.
    static bool parseEntry(const std::string& entryStr, Transaction& out)
    {
        if (entryStr.empty())
            return false;

        std::string s = entryStr;

        // 1. Extract transaction type (string before first colon)
        size_t colonPos = s.find(':');
        if (colonPos == std::string::npos)
            return false;

        std::string typeToken = s.substr(0, colonPos);

        // Map raw tokens to readable type names
        if (typeToken == "DEPOSIT") {
            out.type = "Deposit";
        } else if (typeToken == "WITHDRAW") {
            out.type = "Withdraw";
        } else if (typeToken == "INITIAL_DEPOSIT") {
            out.type = "Initial Deposit";
        } else {
            return false; // unknown transaction type
        }

        // 2. Parse amount immediately after the type
        size_t amountStart = colonPos + 1;
        size_t amountEnd = s.find(' ', amountStart);
        if (amountEnd == std::string::npos)
            amountEnd = s.size();

        std::string amountStr = s.substr(amountStart, amountEnd - amountStart);
        try {
            out.amount = std::stod(amountStr); // convert amount string to double
        } catch (...) {
            return false;
        }

        // 3. Locate the TIME: field
        const std::string timeKey = "TIME:";
        size_t timePos = s.find(timeKey, amountEnd);
        if (timePos == std::string::npos)
            return false;

        size_t timeStart = timePos + timeKey.size();
        if (timeStart < s.size() && s[timeStart] == ' ')
            ++timeStart; // skip space if present

        // 4. Locate NEW_AMOUNT: which appears after the time
        const std::string newKey = "NEW_AMOUNT:";
        size_t newPos = s.find(newKey, timeStart);
        if (newPos == std::string::npos)
            return false;

        // Extract substring containing timestamp
        size_t timeEnd = newPos;
        while (timeEnd > timeStart && s[timeEnd - 1] == ' ')
            --timeEnd; // trim trailing spaces

        out.time = s.substr(timeStart, timeEnd - timeStart);

        // 5. Parse the NEW_AMOUNT field (updated account balance)
        size_t newAmountStart = newPos + newKey.size();
        if (newAmountStart < s.size() && s[newAmountStart] == ' ')
            ++newAmountStart;

        std::string newAmountStr = s.substr(newAmountStart);
        try {
            out.newAmount = std::stod(newAmountStr);
        } catch (...) {
            return false;
        }

        return true; // successful parse
    }

    // Split the full history string on '|' and parse each entry.
    std::vector<Transaction> parse(const std::string& history)
    {
        std::vector<Transaction> result;
        if (history.empty())
            return result;

        std::stringstream ss(history);
        std::string entry;

        // Each entry ends with '|'
        while (std::getline(ss, entry, '|')) {
            if (entry.empty())
                continue;

            Transaction tx;
            if (parseEntry(entry, tx)) {
                result.push_back(tx); // append valid transaction
            } else {
                // Invalid entries are skipped silently
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

        // DEBUG — print raw transaction string for debugging purposes
        std::string raw = user->getTransactions();
        // std::cout << "[DEBUG] Raw transaction string: '" << raw << "'\n";

        auto transactions = parse(raw);
        if (transactions.empty()) {
            std::cout << "No transactions found for this account.\n";
            return;
        }

        std::cout << "\n--- Transaction History for "
                  << user->getFirstName() << " " << user->getLastName()
                  << " (" << user->getUsername() << ") ---\n\n";

        // Print table header
        std::cout << std::left
                  << std::setw(18) << "Type"
                  << std::setw(20) << "Amount"
                  << std::setw(22) << "Time"
                  << std::setw(15) << "Balance After"
                  << "\n";

        std::cout << std::string(75, '-') << "\n";

        // Print each parsed transaction entry
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

    // Adds the initial deposit entry when a new account is created.
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
        user->setTransactions(history); // store updated history
    }

} // namespace Transactions
