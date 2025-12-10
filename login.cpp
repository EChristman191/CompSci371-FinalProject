#include "login.h"
#include "user.h"
#include "bankaccount.h"
#include "savingaccount.h"
#include "checkingaccount.h"
#include <filesystem>

void UserLogin::login(User* user){
    std::string username, password;
    std::cout << "Please enter your username and password: ";
    std::cin >> username >> password;

    std::filesystem::path target = User::dir_path / (username + ".txt");
    if(!std::filesystem::exists(target)){
        std::cout << "Error! \"" << username << "\" does not exist!" << std::endl;
        return;
    }

    std::string line;
    std::ifstream inputFile(target);
    if(inputFile.is_open()){
        while(std::getline(inputFile, line)){
            if(line.rfind("Password: ", 0) == 0){//Current line is 'Password: '
                if(line.substr(10) == password){
                    std::cout << std::endl <<  "--- Successfully Logged In ---" << std::endl << std::endl;

                    user = User::loadUserFromFile(target);
                    BankAccount* account = nullptr;
                    if (user->getAccountType() == "Checking")
                        account = new CheckingAccount();
                    else if (user->getAccountType() == "Saving")
                        account = new SavingAccount();

                    if (account)
                    {
                        account->AccountMenu(user);
                        delete account;
                    }
                    break;
                }else{
                    std::cout << "Error! Passwords do not match!" << std::endl;
                }
            }
        }
        inputFile.close();
    }else{
        std::cout << "ERROR! Unable to open file" << std::endl;
        return;
    }
    return;
}