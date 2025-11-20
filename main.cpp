#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>
#include <list>
using namespace std;

class Menu
{
private:
    string title;
    list<string> options;
public:
    Menu(string title, list<string> options)
    {
        this->title = title;
        this->options = options;
    }
    ~Menu() {}

    void Display()
    {
        int optionCounter = 0;

        cout << "--------------- " << this->title << " ---------------" << endl;
        for (string option : this->options)
        {
            optionCounter += 1;

            cout << optionCounter << ". " << option << endl;
        }
    }
};

int CreateAccount()
{
    string fName, lName, password, confPassword, username;
    double balance = 0.0;

    cout << "Please enter First Name: ";
    cin >> fName;

    cout << "Please enter Last Name: ";
    cin >> lName;

    cout << "Please enter a Username: ";
    cin >> username;

    cout << "Please create a Password: ";
    cin >> password;

    cout << "Please confirm your new Password: ";
    cin >> confPassword;

    while (password != confPassword)
    {
        cout << "Your passwords do not match.\nPlease confirm your new Password: ";
        cin >> confPassword;
    }

    // TODO: Save Logic ------------------

    return 0;
}

int main()
{
    list<string> mainMenuOptions = {"User Login", "Create Account","Manager Login", "Exit"};
    Menu mainMenu = Menu("Main Menu", mainMenuOptions);
    mainMenu.Display();


    int userInput;
    cin >> userInput;
    while (userInput != 4 && userInput <= 4)
    {
        switch (userInput)
        {
            case 1:
                //User Login page
                break;
            case 2:
                CreateAccount();
                break;
            case 3:
                //User Login page
                break;
            case 4:
                cout << "Have a nice day!" << endl;
                break;
        }
        
        mainMenu.Display();
        cin >> userInput;
    }

}
