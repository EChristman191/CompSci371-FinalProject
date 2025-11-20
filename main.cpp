#include <iostream>
#include <list>
#include <windows.h>

#include "Menu.h"
#include "Account.h"

using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);  // so ► prints correctly

    list<string> mainMenuOptions = { "User Login", "Create Account", "Manager Login", "Exit" };
    Menu mainMenu("Main Menu", mainMenuOptions);

    int userInput = 0;

    while (true)
    {
        mainMenu.Display();

        cout << u8"► ";
        cin >> userInput;

        switch (userInput)
        {
        case 1:
            // User Login page (TODO)
            break;
        case 2:
            CreateAccount();
            break;
        case 3:
            // Manager Login page (TODO)
            break;
        case 4:
            quit();
            return 0;
        default:
            // Invalid option
            break;
        }
    }

    return 0;
}
