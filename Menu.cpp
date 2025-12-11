#include "Menu.h"
#include <iostream>
// #include <cstdlib> // for system("cls") — optional include if clearing console is needed

using namespace std;

Menu::Menu(string title, list<string> options)
{
    // Store the provided title and list of menu options
    this->title = title;
    this->options = options;
}

Menu::~Menu() {}

void Menu::Display()
{
    int optionCounter = 0; // used for numbering menu options

    // system("cls");  // optional: clear the console on Windows

    // Print menu title header
    cout << "--------------- " << this->title << " ---------------" << endl;

    // Loop through and display each menu option with sequential numbering
    for (const string& option : this->options)
    {
        optionCounter += 1;
        cout << optionCounter << ". " << option << endl;
    }
}
