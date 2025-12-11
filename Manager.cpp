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
    std::cin >> username; // read the username to delete

    // Build the path to the user's file (Users/<username>.txt)
    std::filesystem::path filepath = User::dir_path / (username + ".txt");

    // Attempt to load the user object from file
    User* user = User::loadUserFromFile(filepath);

    if (user) // user file exists and loaded successfully
    {
        // Attempt to delete the user using the User class function
        if (user->DeleteUser())
        {
            std::cout << "User '" << username << "' deleted successfully." << std::endl;
        }
        else
        {
            std::cout << "Failed to delete user '" << username << "'." << std::endl;
        }

        delete user; // free allocated User object
    }
    else
    {
        // loadUserFromFile failed → user does not exist
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
    std::cin >> username; // get the username to look up

    // Attempt to load a User object from the stored user files
    User* tempUser = User::GetUser(username);

    if (tempUser) // if user exists
    {
        // Print table headers with padded column formatting
        std::cout << std::left
                  << std::setw(10) << "UserID"
                  << std::setw(15) << "Username"
                  << std::setw(20) << "Name"
                  << std::setw(12) << "Balance"
                  << std::setw(15) << "Account Type"
                  << std::endl;

        // Print a horizontal line separator
        std::cout << std::string(72, '-') << std::endl;

        // Combine first and last name for display
        std::string fullName = tempUser->getFirstName() + " " + tempUser->getLastName();

        // Print user information aligned under the correct columns
        std::cout << std::left
                  << std::setw(10) << tempUser->getUserID()
                  << std::setw(15) << tempUser->getUsername()
                  << std::setw(20) << fullName;

        // Display balance with currency formatting
        std::cout << "$" << std::setw(12);
        std::cout << std::fixed << std::setprecision(2) << tempUser->getBalance();

        // Print account type column
        std::cout << std::setw(15) << tempUser->getAccountType()
                  << std::endl;

        delete tempUser; // free the dynamically allocated User object
    }
    else
    {
        // The user could not be found or loaded
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
    std::cin >> username; // read username to view transactions

    // Attempt to load the user from file
    User* tempUser = User::GetUser(username);

    if (tempUser)
    {
        // Call the Transactions class to print the user's transaction history
        Transactions::print(tempUser);

        delete tempUser; // free allocated user object
    }
    else
    {
        // If user not found or file missing
        std::cout << "User '" << username << "' not found.\n";
        std::cout << "Returning to manager menu...\n";
    }
}

void DeleteAllUsers()
{
    char confirmation;
    std::cout << "Are you sure you want to delete ALL users? (y/n): ";
    std::cin >> confirmation; // ask for confirmation before deleting all user files

    if (confirmation == 'y' || confirmation == 'Y')
    {
        // Iterate through the user directory and delete each file
        for (const auto& entry : std::filesystem::directory_iterator(User::dir_path))
        {
            if (entry.is_regular_file()) // ensure it's a file, not a directory
            {
                std::filesystem::remove(entry.path()); // delete the file
            }
        }
        std::cout << "All users have been deleted." << std::endl;
    }
    else
    {
        // Abort deletion if user cancels
        std::cout << "Operation cancelled. No users were deleted." << std::endl;
    }
}

void managerMenu() {
    // Menu options visible to the manager
    std::list<std::string> managerMenuOptions = { 
        "View All Users",
        "View A User",
        "View A User's Transaction History",  // manager can view transactions of a specific user
        "Delete A User",
        "Delete All Users",
        "Logout"
    };

    // Create menu object with title and options
    Menu managerMenu("Manager Menu", managerMenuOptions);

    int userInput = 0;
    bool exitLoop = false;

    // Main manager menu loop
    while (true)
    {
        managerMenu.Display(); // print the menu

        std::cout << u8"► ";
        std::cin >> userInput; // read manager's selection

        // Handle menu choice
        switch (userInput)
        {
        case 1:
            User::listUsers(); // display all users in a formatted list
            break;
        case 2:
            ViewAUser(); // display a single user's information
            break;
        case 3:
            ViewUserHistory(); // show transaction history for a specific user
            break;
        case 4:
            DeleteUser(); // delete one user
            break;
        case 5:
            DeleteAllUsers(); // delete all users
            break;
        case 6:
            exitLoop = true; // exit manager menu
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
            break;
        }

        // End loop if logout was selected
        if (exitLoop) {
            break;
        }
    }
}

} // namespace Manager
