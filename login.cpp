#include "login.h"
#include "user.h"
#include "bankaccount.h"
#include "savingaccount.h"
#include "checkingaccount.h"
#include <filesystem>

void UserLogin::login(User* user){
    std::string username, password;
    std::cout << "Please enter your username and password: ";
    std::cin >> username >> password; // read username and password from user input

    // Build the path to the user's file: Users/<username>.txt
    std::filesystem::path target = User::dir_path / (username + ".txt");

    // Check if the file exists before opening it
    if(!std::filesystem::exists(target)){
        std::cout << "Error! \"" << username << "\" does not exist!" << std::endl;
        return;
    }

    std::string line;
    std::ifstream inputFile(target); // open the user file
    if(inputFile.is_open()){
        // Read each line until EOF
        while(std::getline(inputFile, line)){
            // Check if the line starts with "Password: "
            if(line.rfind("Password: ", 0) == 0){ // current line contains the saved password
                // Compare stored password with user-entered password
                if(line.substr(10) == password){
                    std::cout << std::endl <<  "--- Successfully Logged In ---" << std::endl << std::endl;

                    // Load user object from file
                    user = User::loadUserFromFile(target);

                    BankAccount* account = nullptr;

                    // Create the correct account type (Checking or Saving)
                    if (user->getAccountType() == "Checking"){
                        account = new CheckingAccount();
                    }else if (user->getAccountType() == "Saving"){
                        account = new SavingAccount();
                    }

                    // If a valid account object was created, open its menu
                    if (account)
                    {
                        account->AccountMenu(user);
                        delete account; // free memory after menu closes
                    }
                    break; // stop reading file after successful login
                }else{
                    std::cout << "Error! Passwords do not match!" << std::endl;
                }
            }
        }
        inputFile.close(); // close the file after reading
    }else{
        // File could not be opened for some reason
        std::cout << "ERROR! Unable to open file" << std::endl;
        return;
    }
    return;
}
