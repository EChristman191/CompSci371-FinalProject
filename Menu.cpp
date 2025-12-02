#include "Menu.h"
#include <iostream>
// #include <cstdlib> // for system("cls")

using namespace std;

Menu::Menu(string title, list<string> options)
{
    this->title = title;
    this->options = options;
}

Menu::~Menu() {}

void Menu::Display()
{
    int optionCounter = 0;
    // system("cls");
    cout << "--------------- " << this->title << " ---------------" << endl;
    for (const string& option : this->options)
    {
        optionCounter += 1;
        cout << optionCounter << ". " << option << endl;
    }
}
