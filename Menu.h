#pragma once
#include <string>
#include <list>

class Menu
{
private:
    std::string title;
    std::list<std::string> options;

public:
    Menu(std::string title, std::list<std::string> options);
    ~Menu();

    void Display();
};
