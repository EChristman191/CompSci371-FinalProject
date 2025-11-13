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
                //User Login page
                break;
            case 3:
                //User Login page
                break;
            case 4:
                cout << "Have a nice day!" << endl;
                break;
        }
    }

}
