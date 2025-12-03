#ifndef USER_H
#define USER_H
#include<iostream>
#include<fstream>
#include<string>
#include<filesystem>
#include<cassert>
#include<algorithm>

class User{
    private:
        int userid;
        static int id_count;
        std::string first_name;
        std::string last_name;
        std::string password;
        double balance;
        std::string username;
        std::string account_type;
        std::string transactions;
    public:
        void saveToFile() const;
        User();
        User(std::string& first_name, std::string& last_name, double balance);

        //In the manager function, we need to create a method to use this.
        //When using this, prompt for a new username before running the Copy. Call User::isValidUsername
        User(const User& otherUser);
        
        int getUserID() const;
        std::string getPassword() const;
        std::string getFirstName() const;
        std::string getLastName() const;
        std::string getUsername() const;
        std::string getTransactions() const;
        double getBalance() const;
        std::string getAccountType() const;
        void setFirstName(const std::string& first_name);
        void setLastName(const std::string& last_name);
        void setBalance(const double balance);
        void setPassword(const std::string& password);
        void setUsername(const std::string& username);
        void setAccountType(const std::string& accountType);
        void setTransactions(const std::string& transactions);

        static void createAccount(User* newUser);
        static bool isValidUsername(std::string& username);
        static bool isUsernameTaken(std::string& username);
        static void checkAccountType(std::string& account_type);
        static void createUserDirectory();
        static User* loadUserFromFile(const std::filesystem::path& filepath);
        static void listUsers();
        static int numOfUsers();//This is to count how many users there are in order to keep the correct number for account#

        static std::filesystem::path dir_path;
};

#endif