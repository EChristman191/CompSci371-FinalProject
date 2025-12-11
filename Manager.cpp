#include <iostream>
#include <list>
#include <string>
#include <iomanip>
#include <filesystem>
#include <windows.h>
#include "user.h"
#include "Menu.h"
#include "Manager.h"
#include "transactions.h"

namespace Manager {

void ManagerLogin() {
    std::string managerUsername;
    std::string managerPassword;

    std::cout << "Enter Manager Username: ";
    std::cin >> managerUsername;
    std::cout << "Enter Manager Password: ";
    std::cin >> managerPassword;

    std::string line, nextLine;
    std::ifstream inputFile("Managers/managers.txt");

    if (inputFile.is_open()) {
        while (std::getline(inputFile, line)) {
        // Check if this line starts with "Username: "
            if (line.rfind("Username: ", 0) == 0) {
                // Compare username after the prefix
                    if (line.substr(10) == managerUsername) {

                    // Read the next line, "Password: "
                    if (std::getline(inputFile, nextLine)) {

                    // Check if next line starts with "Password: "
                            if (nextLine.rfind("Password: ", 0) == 0) {
                            // Compare password after the prefix
                            if (nextLine.substr(10) == managerPassword) {
                                std::cout << "\n--- Successfully Logged In ---\n\n";
                                managerMenu();
                                break; // stop searching after successful login
                            } else {
                                std::cout << "Error! Passwords do not match!" << std::endl;
                            }
                        }
                    }
                }else{
                    std::cout << "Error! Account does not exist!" << std::endl;
                }
            }
        }
        inputFile.close();
    } else {
        std::cout << "ERROR! Unable to open file" << std::endl;
        return;
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


/*      EXAMPLE OF 'ViewAUser()'

UserID    Username       Name                Balance     Account Type   
------------------------------------------------------------------------
1007      desidj27       David Desing        $52.32       Checking
*/
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

        std::cout << "$" << std::setw(12);
        std::cout << std::fixed << std::setprecision(2) << tempUser->getBalance();

        std::cout << std::setw(15) << tempUser->getAccountType()
                  << std::endl;

        delete tempUser;
    }
    else
    {
        std::cout << "User '" << username << "' not found.\n";
        std::cout << "Returning to manager menu...\n";
    }
}

/*      Example of 'ViewUserHistory()'

--- Transaction History for Jack London (londj) ---

Type              Amount              Time                  Balance After
---------------------------------------------------------------------------
Initial Deposit   $96.32              2025/12/10 20:55:46   $96.32
Deposit           $23.12              2025/12/10 21:10:15   $119.44
Withdraw          $6.70               2025/12/10 21:10:20   $112.74
*/
void ViewUserHistory()
{
    std::string username;
    std::cout << "Enter the username of the user to view transaction history: ";
    std::cin >> username;

    User* tempUser = User::GetUser(username);

    if (tempUser)
    {
        Transactions::print(tempUser);//prints from Transactions

        delete tempUser;
    }
    else
    {
        std::cout << "User '" << username << "' not found.\n";
        std::cout << "Returning to manager menu...\n";
    }
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
        "View A User's Transaction History",  // NEW OPTION
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
            ViewUserHistory();
            break;
        case 4:
            DeleteUser();
            break;
        case 5:
            DeleteAllUsers();
            break;
        case 6:
            exitLoop = true;
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
        }

        if (exitLoop) {
            break;
        }
    }
}

} // namespace Manager
