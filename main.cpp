#include <iostream>
#include <string>
#include <conio.h>
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

string getPassword() {
    std::string password;
    char ch;

    while (true) {
        ch = _getch();  

        if (ch == 13) { 
            std::cout << std::endl;
            break;
        }
        else if (ch == 8) {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; 
            }
        }
        else {
            password.push_back(ch);
            std::cout << "*"; 
        }
    }

    return password;
}

int CreateAccount()
{
    string fName, lName, username;

    double balance = 0.0;

    system("cls");


    cout << "Please enter First Name: ";
    cin >> fName;

    cout << "Please enter Last Name: ";
    cin >> lName;

    cout << "Please enter a Username: ";
    cin >> username;

    cout << "Please create a Password: ";
    string password =  getPassword();

    cout << "Please confirm your new Password: ";
    string confPassword = getPassword();

    while (password != confPassword)
    {
        cout << "Your passwords do not match." << endl;

        cout << "Please create a Password: ";
        password = getPassword();

        cout << "Please confirm your new Password: ";
        confPassword = getPassword();
    }


    // TODO: Save Logic ------------------

    system("cls");
    cout << "Account created!\nWelcome to our bank " << fName << " " << lName << " (" << username << ")." << endl;
    cout << "\n\nPress Enter to continue...";
    cin.ignore();
    cin.get();

    return 0;
}

int main()
{
    list<string> mainMenuOptions = { "User Login", "Create Account","Manager Login", "Exit" };
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
