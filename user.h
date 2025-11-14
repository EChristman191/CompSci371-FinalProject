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
    public:
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
        double getBalance() const;
        void setFirstName(std::string& first_name);
        void setLastName(std::string& last_name);
        void setBalance(double increment);
        void setPassword(std::string& password);
        void setUsername(std::string& username);

        static void createAccount(User* newUser);
        static bool isValidUsername(std::string& username);
        static bool isUsernameTaken(std::string& username);
        static void checkAccountType(std::string& account_type);
        static void createUserDirectory();
        static void listUsers();
        static int numOfUsers();//This is to count how many users there are in order to keep the correct number for account#

        static std::filesystem::path dir_path;
};

#endif