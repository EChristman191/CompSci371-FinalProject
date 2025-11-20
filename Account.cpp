#include "Account.h"

#include <iostream>
#include <string>
#include <conio.h>   // _getch

using namespace std;

static string getPassword()
{
    string password;
    char ch;

    while (true)
    {
        ch = _getch();

        if (ch == 13)  // Enter
        {
            cout << endl;
            break;
        }
        else if (ch == 8) // Backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            password.push_back(ch);
            cout << "*";
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
    string password = getPassword();

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

    cout << "Account created!\nWelcome to our bank "
        << fName
        << " "
        << lName
        << " ("
        << username
        << ").\n\nPress Enter to continue...";

    cin.ignore();
    cin.get();

    return 0;
}

void quit()
{
    system("cls");
    cout << "\n\nThank you for using our bank!\n\n";
}
