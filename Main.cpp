#include "user.h"
using namespace std;

/*
    You must compile the project by doing 'g++ Main.cpp user.cpp -o app' in the directory cmd
*/

int main() {
    //VERY first thing that needs to be done. It creates the Directory for Users and managers, then the txt file for Managers. 
    User::createUserDirectory();

    //Next step: Menu System
    
    //For user creation, use following format
    User* user = new User();
    User::createAccount(user);

    //Will need to implement 'Users::loadUserFromFile' for user login

    //At the end of the program, we need to delete the stored user pointer
    delete user;
    return 0;
}

