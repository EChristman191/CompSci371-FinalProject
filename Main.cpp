// #include <iostream>
#include <list>
#include <windows.h>
#include "user.h"
#include "Menu.h"
#include "login.h"

/*
    You must compile the project by doing 'g++ Main.cpp user.cpp -o app' in the directory cmd
*/

int main() {
    std::list<std::string> mainMenuOptions = { "User Login", "Create Account", "Manager Login", "Exit" };
    Menu mainMenu("Main Menu", mainMenuOptions);
    SetConsoleOutputCP(CP_UTF8);

    //VERY first thing that needs to be done. It creates the Directory for Users and managers, then the txt file for Managers. 
    User::createUserDirectory();
    User* user;

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
            // User Login page (TODO)
            UserLogin::login(user);
            break;
        case 2:
            //For user creation, use following format
            user = new User();
            User::createAccount(user);
            delete user;//Delete the user pointer as it is no longer needed for account creation. Will be reused when user logs in
            break;
        case 3:
            // Manager Login page (TODO)
            break;
        case 4:
            exitLoop = true;
            break;
        default:
            // Invalid option
            break;
        }

        if(exitLoop){
            break;
        }
    }

    return 0;
}
