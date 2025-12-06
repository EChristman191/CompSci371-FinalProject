#include <iostream>
#include <list>
#include <string>
#include <windows.h>
#include "user.h"
#include "Menu.h"
#include "Manager.h"
/*
    You must compile the project by doing 'g++ Manager.cpp user.cpp -o managerApp' in the directory cmd
*/

namespace Manager {

void ManagerLogin() {
    std::string managerUsername;
    std::string managerPassword;

    std::cout << "Enter Manager Username: ";
    std::cin >> managerUsername;
    std::cout << "Enter Manager Password: ";
    std::cin >> managerPassword;

    // For simplicity, hardcoding manager credentials
    if (managerUsername == "admin" && managerPassword == "password") {
        std::cout << "Manager login successful!" << std::endl;
        managerMenu();
    } else {
        std::cout << "Invalid manager credentials!" << std::endl;
    }
}

void DeleteUser()
{
    std::string username;
    std::cout << "Enter the username of the user to delete: ";
    std::cin >> username;

    // Load user from file
    std::filesystem::path filepath = User::dir_path / (username + ".txt");
    User* user = User::loadUserFromFile(filepath);

    if (user) 
    {

        if (user->DeleteUser()) 
        {
            std::cout << "User '" << username << "' deleted successfully." << std::endl;
        } 
        else 
        {
            std::cout << "Failed to delete user '" << username << "'." << std::endl;
        }

        delete user;
    
    } 
    else 
    {
        std::cout << "User '" << username << "' not found." << std::endl;
    }
}

void ViewAUser()
{
    std::string username;
    std::cout << "Enter the username of the user to view: ";
    std::cin >> username;

    User* tempUser = User::GetUser(username);

    if (tempUser)
    {
        std::cout << std::left
                  << std::setw(10) << "UserID"
                  << std::setw(15) << "Username"
                  << std::setw(20) << "Name"
                  << std::setw(12) << "Balance"
                  << std::setw(15) << "Account Type"
                  << std::endl;

        std::cout << std::string(72, '-') << std::endl;

        std::string fullName = tempUser->getFirstName() + " " + tempUser->getLastName();

        std::cout << std::left
                  << std::setw(10) << tempUser->getUserID()
                  << std::setw(15) << tempUser->getUsername()
                  << std::setw(20) << fullName;

        // balance formatting without affecting setw
        std::cout << std::setw(12);
        std::cout << std::fixed << std::setprecision(2) << ("$" + std::to_string(tempUser->getBalance()));

        std::cout << std::setw(15) << tempUser->getAccountType()
                  << std::endl;

        delete tempUser;
    }
    else
    {
        std::cout << "User '" << username << "' not found.\n";
        std::cout << "Returning to manager menu...\n";
    }

    return;
}

void DeleteAllUsers()
{
    char confirmation;
    std::cout << "Are you sure you want to delete ALL users? (y/n): ";
    std::cin >> confirmation;

    if (confirmation == 'y' || confirmation == 'Y') 
    {
        for (const auto& entry : std::filesystem::directory_iterator(User::dir_path)) 
        {
            if (entry.is_regular_file()) 
            {
                std::filesystem::remove(entry.path());
            }
        }
        std::cout << "All users have been deleted." << std::endl;
    } 
    else 
    {
        std::cout << "Operation cancelled. No users were deleted." << std::endl;
    }
}


void managerMenu() {
    std::list<std::string> managerMenuOptions = { 
        "View All Users",
        "View A User", 
        "Delete A User",
        "Delete All Users", 
        "Logout"
    };
    Menu managerMenu("Manager Menu", managerMenuOptions);

    int userInput = 0;
    bool exitLoop = false;
    while (true)
    {
        managerMenu.Display();

        std::cout << u8"► ";
        std::cin >> userInput;

        switch (userInput)
        {
        case 1:
            User::listUsers();
            break;
        case 2:
            ViewAUser();
            break;
        case 3:
            DeleteUser();
            break;
        case 4:
            DeleteAllUsers();
            break;
        case 5:
            exitLoop = true;
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
        }

        if(exitLoop){
            break;
        }
    }
}

} // namespace Manager