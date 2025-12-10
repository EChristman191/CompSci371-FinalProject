#include "user.h"
#include<string>
#include<sstream>

int User::id_count = 1000;
std::filesystem::path User::dir_path = "Users";

User::User()
{//Default constructor for when we do a new User()
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

// Getters
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

double User::getBalance() const {
    return this->balance;
}

std::string User::getUsername() const{
    return this->username;
}

std::string User::getAccountType() const{
    return this->account_type;
}

std::string User::getTransactions() const{
    return this->transactions;
}

// Setters 
void User::setFirstName(const std::string& first_name){
    this->first_name = first_name;
}

void User::setLastName(const std::string& last_name){
    this->last_name = last_name;
}

void User::setBalance(double balance){
    this->balance = balance;
}

void User::setPassword(const std::string& password){
    this->password = password;
}

void User::setUsername(const std::string& username){
    this->username = username;
}

void User::setAccountType(const std::string& account_type){
    this->account_type = account_type;
}

void User::setTransactions(const std::string& transactions){
    this->transactions = transactions;
}

bool User::DeleteUser()
{
    // Build the path to this user's file: Users/<username>.txt
    std::filesystem::path filepath = dir_path / (username + ".txt");

    try
    {
        if (!std::filesystem::exists(filepath))
        {
            std::cout << "Error: No account file found for user '" 
                      << username << "'." << std::endl;
            return false;
        }

        // Attempt to delete the file
        if (!std::filesystem::remove(filepath))
        {
            std::cout << "Error: Failed to delete account file for user '"
                      << username << "'." << std::endl;
            return false;
        }

        // Optional: wipe in-memory data now that the file is gone
        first_name.clear();
        last_name.clear();
        password.clear();
        username.clear();
        transactions.clear();
        account_type.clear();
        balance = 0.0;

        std::cout << "Account for user deleted successfully." << std::endl;
        return true;
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem error while deleting user: " 
                  << e.what() << std::endl;
        return false;
    }
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

User* User::GetUser(std::string username)
{
    return User::loadUserFromFile(User::dir_path / (username + ".txt"));
}

//Used by Managers to view all Bank Accounts
void User::listUsers()
{
    if (!std::filesystem::exists(dir_path)) 
    {
        std::cout << "No users found." << std::endl;
        return;
    }

    std::cout << std::left
              << std::setw(10) << "UserID"
              << std::setw(15) << "Username"
              << std::setw(20) << "Name"
              << std::setw(12) << "Balance"
              << std::setw(15) << "Account Type"
              << std::endl;

    std::cout << std::string(72, '-') << std::endl;

    for (const auto& file : std::filesystem::directory_iterator(dir_path))
    {
        if (file.is_regular_file())
        {
            User* user = loadUserFromFile(file.path());
            if (user)
            {
                std::string fullName = user->getFirstName() + " " + user->getLastName();

                // format "$123.45" as a single string so setw applies to the whole thing
                std::ostringstream balStream;
                balStream << std::fixed << std::setprecision(2) << "$" << user->getBalance();
                std::string balanceStr = balStream.str();

                std::cout << std::left
                          << std::setw(10) << user->getUserID()
                          << std::setw(15) << user->getUsername()
                          << std::setw(20) << fullName
                          << std::setw(12) << balanceStr
                          << std::setw(15) << user->getAccountType()
                          << std::endl;

                delete user;
            }
        }
    }
}


//Once user logs in, load their userdata from the file
User* User::loadUserFromFile(const std::filesystem::path& filepath) {
    std::ifstream file(filepath);
    // if (!file.is_open())
    // {
    //     std::ifstream inputFile(filepath);
    // };

    User* u = new User();
    std::string line;

    while (std::getline(file, line)) {//Only loads information needed by the BankAccount

    if (line.rfind("Username: ", 0) == 0) {
        u->setUsername(line.substr(10));

    } else if (line.rfind("Password: ", 0) == 0) {
        u->setPassword(line.substr(10));

    } else if (line.rfind("First: ", 0) == 0) {
        u->setFirstName(line.substr(7));

    } else if (line.rfind("Last: ", 0) == 0) {
        u->setLastName(line.substr(6));

    } else if (line.rfind("Balance: ", 0) == 0) {
        try {
            u->setBalance(std::stod(line.substr(9)));
        } catch (...) {
            u->setBalance(0);
        }

    } else if (line.rfind("Account Type: ", 0) == 0) {
        u->setAccountType(line.substr(14));

    } else if (line.rfind("Transaction History: ", 0) == 0) {
        const std::string key = "Transaction History: ";
        u->setTransactions(line.substr(key.length()));
    }
}

    return u;
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

    //Initializes the total amount of accounts created 
    id_count += User::numOfUsers();
}

bool User::isUsernameTaken(std::string& username){
    std::filesystem::path target = dir_path / (username + ".txt");//creating a path to the inputted username. EX: "Users\\user_001.txt"
    return std::filesystem::exists(target);
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
            account_type[0] = std::toupper(account_type[0]);
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
    newUser->setAccountType(account_type);
    newUser->setTransactions(""); // start with empty transaction history

    //Instead of manually writing the file here, use the centralized saveToFile() function
    newUser->saveToFile();

    std::cout << std::endl << "-- SUCCESS! -- " 
              << username << "'s " + account_type 
              << " account has been successfully created." 
              << std::endl << std::endl;
    }

void User::saveToFile() const {
    std::filesystem::path filepath = dir_path / (username + ".txt");
    std::ofstream out(filepath);

    // YOUR ERROR CHECK (RESTORED)
    if (!out.is_open()) {
        std::cout << "Unable to open the file for writing." << std::endl;
        return;
    }

    out << "Account#: " << userid << std::endl;
    out << "Username: " << username << std::endl;
    out << "Password: " << password << std::endl;
    out << "First: " << first_name << std::endl;
    out << "Last: " << last_name << std::endl;
    out << "Balance: " << balance << std::endl;
    out << "Account Type: " << account_type << std::endl;
    out << "Transaction History: " << transactions << std::endl;

    out.close();
}