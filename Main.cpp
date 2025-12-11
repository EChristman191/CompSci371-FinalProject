#include <iostream>
#include <list>
#include <limits>
#include <windows.h>
#include "user.h"
#include "Menu.h"
#include "login.h"
#include "Manager.h"

int main() {
    std::list<std::string> mainMenuOptions = { "User Login", "Create Account", "Manager Login", "Exit" };
    Menu mainMenu("Main Menu", mainMenuOptions);
    SetConsoleOutputCP(CP_UTF8);

    User::createUserDirectory();
    User* user = nullptr;

    int userInput = 0;
    bool exitLoop = false;

    while (true) {
        mainMenu.Display();

        std::cout << u8"► ";

        if (!(std::cin >> userInput)) {
            // Input was not an integer
            std::cin.clear(); // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard bad input
            std::cout << "Invalid input. Please enter a number from the menu.\n";
            continue;
        }

        switch (userInput) {
        case 1:
            UserLogin::login(user);
            break;
        case 2:
            user = new User();
            User::createAccount(user);
            delete user;
            user = nullptr;
            break;
        case 3:
            Manager::ManagerLogin();
            break;
        case 4:
            exitLoop = true;
            break;
        default:
            std::cout << "Invalid option. Please choose 1–4.\n";
            break;
        }

        if (exitLoop) {
            break;
        }
    }

    return 0;
}
