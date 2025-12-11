#include "bankaccount.h"
#include "Menu.h"
#include "transactions.h"
#include <windows.h>
#include <list>
#include <string>
#include <limits>
#include <iostream>

void BankAccount::AccountMenu(User* user)
{
    std::list<std::string> mainMenuOptions = { "Deposit", "Withdraw", "Account Info", "View Activity", "Log Out" };
    Menu mainMenu("Bank Account Menu", mainMenuOptions);
    SetConsoleOutputCP(CP_UTF8);

    int userInput = 0;
    bool exitLoop = false;
    while (true)
    {
        mainMenu.Display();

        std::cout << u8"► ";
        std::cin >> userInput;

        switch (userInput)
        {
        case 1:
        {
            double amount = 0.0;
            std::cout << "How much would you like to deposit? ";

            if (!(std::cin >> amount))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount.\n";
                break;
            }

            // Call virtual function implemented by derived class
            deposit(user, amount);
            break;
        }
        case 2:
        {
            double amount = 0.0;
            std::cout << "How much would you like to withdraw? ";

            if (!(std::cin >> amount))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount.\n";
                break;
            }

            // Call virtual function implemented by derived class
            withdraw(user, amount);
            break;
        }
        case 3:
            std::cout << std::endl
                      << "--- " << user->getFirstName() << " " << user->getLastName() << "'s Account ---" << std::endl;
            std::cout << "Account Type: " << getAccountType() << std::endl
                      << "Balance: " << user->getBalance() << std::endl
                      << std::endl;
            break;
        case 4:
            Transactions::print(user);
            break;
        case 5:
            std::cout << std::endl
                      << "--- Successfully Logged Out ---" << std::endl
                      << std::endl;
            exitLoop = true;
            break;
        default:
            // Invalid option
            break;
        }

        if (exitLoop)
        {
            break;
        }
    }
}