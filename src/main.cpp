#include "../HeaderFiles/PhoneBook.h"
#include "../HeaderFiles/FileStorage.h"
#include "../HeaderFiles/ConsoleUI.h"

#include <iostream>
#include <memory>
#include <exception>

int main()
{
    try
    {
        auto storage = std::make_unique<FileStorage>("contacts.txt");

        PhoneBook phoneBook(std::move(storage));

        ConsoleUI consoleUI(phoneBook);

        consoleUI.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Application error: "
                  << e.what() << '\n';

        return 1;
    }

    return 0;
}