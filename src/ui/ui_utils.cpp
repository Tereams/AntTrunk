#include "ui/ui_utils.h"

#include <iostream>
#include <limits>

std::string promptString(const std::string& message){
    std::cout << message;
    std::string value;
    std::getline(std::cin, value);

    return value;
}

int promptInt(const std::string& message){
    int value;

    while (true){
        std::cout << message;

        if (std::cin >> value){
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            return value;
        }

        std::cout << "Invalid input. Please enter an integer.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }
}

double promptDouble(const std::string& message)
{
    double value;
    while (true)
    {
        std::cout << message;
        if (std::cin >> value)
        {
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            return value;
        }

        std::cout << "Invalid input. Please enter a number.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }
}

bool promptYesNo(const std::string& message)
{
    while (true)
    {
        std::string input = promptString(message + " (y/n): ");

        if (input == "y" || input == "Y")
        {
            return true;
        }

        if (input == "n" || input == "N")
        {
            return false;
        }

        std::cout << "Invalid input. Please enter y or n.\n";
    }
}
