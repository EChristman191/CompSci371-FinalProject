#include "bankaccount.h"
#include "Menu.h"
#include "transactions.h"
#include <windows.h>
#include <list>
#include <string>
#include <iostream>
#include <limits>

void BankAccount::AccountMenu(User* user)
{
    // Menu options for bank account actions
    std::list<std::string> mainMenuOptions = { "Deposit", "Withdraw", "Account Info", "View Activity", "Log Out" };
    Menu mainMenu("Bank Account Menu", mainMenuOptions);
    SetConsoleOutputCP(CP_UTF8); // ensures UTF-8 output in Windows console

    int userInput = 0;
    bool exitLoop = false;

    // Main account menu loop
    while (true)
    {
        mainMenu.Display(); // show menu options

        std::cout << u8"► ";
        std::cin >> userInput; // read menu choice

        switch (userInput)
        {
        case 1: // Deposit
        {
            double amount = 0.0;
            std::cout << "How much would you like to deposit? ";

            // Input validation for deposit amount
            if (!(std::cin >> amount))
            {
                std::cin.clear(); // clear error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
                std::cout << "Invalid amount.\n";
                break;
            }

            // Calls virtual deposit() method implemented in derived classes
            deposit(user, amount);
            break;
        }

        case 2: // Withdraw
        {
            double amount = 0.0;
            std::cout << "How much would you like to withdraw? ";

            // Input validation for withdrawal amount
            if (!(std::cin >> amount))
            {
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount.\n";
                break;
            }

            // Calls virtual withdraw() method from derived classes
            withdraw(user, amount);
            break;
        }

        case 3: // Account information
            std::cout << std::endl
                      << "--- " << user->getFirstName() << " " << user->getLastName() << "'s Account ---" << std::endl;
            std::cout << "Account Type: " << getAccountType() << std::endl
                      << "Balance: " << user->getBalance() << std::endl
                      << std::endl;
            break;

        case 4: // View transactions / activity
            Transactions::print(user);
            break;

        case 5: // Log out
            std::cout << std::endl
                      << "--- Successfully Logged Out ---" << std::endl
                      << std::endl;
            exitLoop = true;
            break;

        default:
            // Unrecognized menu option (ignored)
            break;
        }

        if (exitLoop)
        {
            break; // break out of loop if logout selected
        }
    }
}
