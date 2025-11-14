#include "user.h"

int User::id_count = 1000;
std::filesystem::path User::dir_path = "Users";

User::User(){//Default constructor for when we do a new User()
    id_count++;
    this->userid = id_count;
    this->first_name = "NULL";
    this->last_name = "NULL";
    this->balance = 0;
}

//Can be used by managers to create a new account by entering all information right away, will still need to run a few setters
User::User(std::string& first_name, std::string& last_name, double balance){
    id_count++;
    this->userid = id_count;
    this->first_name = first_name;
    this->last_name = last_name;
    this->balance = balance;
}

User::User(const User& otherUser){//Copy function, will need to prompt to get a username, validate it, and check if its taken
    id_count++;
    this->userid = id_count;
    this->first_name = otherUser.first_name;
    this->last_name = otherUser.last_name;
    this->balance = otherUser.balance;
}

std::string User::getFirstName() const{
    return this->first_name;
}

std::string User::getLastName() const{
    return this->last_name;
}

std::string User::getPassword() const{
    return this->password;
}

int User::getUserID() const{
    return this->userid;
}

double User::getBalance() const{
    return this->balance;
}

std::string User::getUsername() const{
    return this->username;
}

void User::setFirstName(std::string& first_name){
    this->first_name = first_name;
}

void User::setLastName(std::string& last_name){
    this->last_name = last_name;
}

void User::setBalance(double increment){
    this->balance += increment;
}

void User::setPassword(std::string& password){
    this->password = password;
}

void User::setUsername(std::string& username){
    this->username = username;
}

int User::numOfUsers(){
    int count = 0;

    if (!std::filesystem::exists(dir_path))
        return 0;

    for (const auto& file : std::filesystem::directory_iterator(dir_path)) {
        if (file.is_regular_file()) {//Counts how many txt files are inside the 'Users' directory to keep a 'real-time' update of UserIDs
            count++;
        }
    }
    return count;
}

//Used by Managers to view all Bank Accounts
void User::listUsers(){
}

//Once user logs in, load their userdata from the file
User loadUserFromFile(const std::filesystem::path& filepath) {
}


void User::createUserDirectory(){
    try {
        // Create the directory 'Users' if it doesn't exist
        if (std::filesystem::create_directories(dir_path)) {
            std::cout << "Users directory created successfully: " << dir_path << std::endl;
        }

        //Creates the Managers directory, then creates the managers.txt
        if(std::filesystem::create_directories("Managers")){
            std::ofstream outputFile("Managers/managers.txt");
            outputFile.close();
            std::cout << "Managers directory and txt created successfully: " << dir_path << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    //Correctly initializes the total amount of accounts created 
    id_count += User::numOfUsers();
}

bool User::isUsernameTaken(std::string& username){
    std::filesystem::path target = dir_path / (username + ".txt");//creating a path to the inputted username. EX: "Users\\user_001.txt"
    for(const auto& file : std::filesystem::directory_iterator(dir_path)){//automatically converts the file type depending on the file type(s) inside the 'Users' dir
        if(target == file.path()){
            return true;
        }
    }
    return false;
}

bool User::isValidUsername(std::string& username){
    bool validUsername = false;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//clears input buffer, aka next input line is actually what information we want
    do{
        std::cout << "What would you like your Username to be?" << std::endl;
        getline(std::cin, username);
        if(username.find(' ') != std::string::npos){ 
            std::cout << "Error! Username cannot have a space in it!" << std::endl;
        }else if(username.empty()){
            std::cout << "Error! Username cannot be empty!" << std::endl;
        }else{
            validUsername = true;
        }
    }while(!validUsername);
    return true;
}

void User::checkAccountType(std::string& account_type){
    while (true) {
        if (account_type == "-1") {
            return;
        }

        if (account_type == "checking" || account_type == "saving") {
            return;
        }

        std::cout << "Error! Account type is invalid! Must be either 'Checking' or 'Saving', or enter -1 to cancel account creation." << std::endl;
        std::cout << "Enter account type: ";
        std::cin >> account_type;

        transform(account_type.begin(), account_type.end(), account_type.begin(), ::tolower);
    }
}

void User::createAccount(User* newUser){//User account creation. "Returns" the pointer of the created account.
    std::string first_name, last_name, username, account_type, password;
    double balance;

    std::cout << "Enter your First and Last name. <first_name last_name>" << std::endl;
    std::cin >> first_name >> last_name;

    User::isValidUsername(username);//Checks to see if the username is valid. Aka no spaces

    transform(username.begin(), username.end(), username.begin(), ::tolower);//converts the username std::string to all lowercase

    //check if username is already in use. If so, tell the user to choose a different username. 
    while(isUsernameTaken(username)){
        std::cout << "Error! Username is already taken. Please enter a new one." << std::endl;
        std::cin >> username;
        transform(username.begin(), username.end(), username.begin(), ::tolower);
    }

    std::cout << "Please create a password:" << std::endl;//There are no password requirements as of right now. We can add some before submission
    std::cin >> password;

    std::cout << "What type of account are you creating? <Checking / Saving>" << std::endl;
    std::cin >> account_type;
    transform(account_type.begin(), account_type.end(), account_type.begin(), ::tolower);
    checkAccountType(account_type);//Account type MUST be either 'Checking' or 'Saving'. It is not case sensitive (can be ChECkIng or Checking, or checking, etc.)

    if(account_type == "-1"){
        std::cout << "Account Creation Cancelled" << std::endl;
        return;
    }

    std::cout << "How much is the initial deposit? <X.XX>" << std::endl;
    std::cin >> balance;

    //Most banks require a minimum deposite. The one I use has a min of $20
    while (std::cin.fail() || balance < 20) {//Check to see if a number was inputted, if not it's a fail()
        if (std::cin.fail()) {
            std::cin.clear(); //Clears the fail so
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//ignores the next line
            std::cout << "Error! Deposit must be a number." << std::endl;
        }
        else if (balance < 20) {
            std::cout << "Sorry, you must deposit at least $20 to open an account!" << std::endl;
        }

        std::cout << "Enter amount or -1 to cancel: ";
        std::cin >> balance;

        // If user types -1, cancel account creation
        if (!std::cin.fail() && balance == -1) {
            std::cout << "Account Creation Cancelled!" << std::endl << std::endl;
            return;
        }
    }

    //Saves the user to private data members
    newUser->setFirstName(first_name);
    newUser->setLastName(last_name);
    newUser->setPassword(password);
    newUser->setUsername(username);
    newUser->setBalance(balance);

    std::ofstream outputFile(dir_path.string() + "/" + username + ".txt");//Output to 'Users/<username>.txt'
    if (outputFile.is_open()) { 
        outputFile << "Account#: " << newUser->getUserID() << std::endl;
        outputFile << "Username: " << newUser->getUsername() << std::endl;
        outputFile << "Password: " << newUser->getPassword() << std::endl;
        outputFile << "First: " << newUser->getFirstName() << std::endl;
        outputFile << "Last: " << newUser->getLastName() << std::endl;
        outputFile << "Balance: " << newUser->getBalance() << std::endl;
        // Close the file after writing
        outputFile.close();
        std::cout << "--SUCCESS!-- " << username << "'s " + account_type + " has been successfully created." << std::endl;
    }
    else {
        std::cout << "Unable to open the file for writing." << std::endl;
    }
}