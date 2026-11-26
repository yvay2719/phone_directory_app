#include "../HeaderFiles/ConsoleUI.h"
#include <iostream>
#include <string>
#include <exception>
#include <sstream>

ConsoleUI::ConsoleUI(PhoneBook &phoneBook)
    : m_phoneBook(phoneBook)
{
}

void ConsoleUI::showMenu() const
{
    std::cout << "\n===== PHONE BOOK =====\n"
              << "1. Show all contacts\n"
              << "2. Add contact\n"
              << "3. Remove contact\n"
              << "4. Edit contact\n"
              << "5. Search contacts\n"
              << "6. Sort contacts\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

void ConsoleUI::run()
{
    std::string choice;

    while (true)
    {
        showMenu();

        if (!std::getline(std::cin, choice))
        {
            return;
        }

        try
        {
            if (choice == "1")
            {
                showContacts();
            }
            else if (choice == "2")
            {
                addContact();
            }
            else if (choice == "3")
            {
                removeContact();
            }
            else if (choice == "4")
            {
                editContact();
            }
            else if (choice == "5")
            {
                searchContacts();
            }
            else if (choice == "6")
            {
                sortContacts();
            }
            else if (choice == "0")
            {
                std::cout << "Goodbye!\n";
                return;
            }
            else
            {
                std::cout << "Invalid option. Try again.\n";
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
}
void ConsoleUI::printContact(const Contact &contact) const
{
    std::cout << "\n-------------------------\n"
              << "ID: " << contact.getId() << '\n'
              << "First name: " << contact.getFirstName() << '\n'
              << "Last name: " << contact.getLastName() << '\n'
              << "Middle name: " << contact.getMiddleName() << '\n'
              << "Address: " << contact.getAddress() << '\n'
              << "Birth date: " << contact.getBirthDate() << '\n'
              << "Email: " << contact.getEmail() << '\n'
              << "Phone numbers:\n";

    for (const auto &phone : contact.getPhoneNumbers())
    {
        std::string type;

        switch (phone.getType())
        {
        case PhoneNumber::Type::Work:
            type = "Work";
            break;

        case PhoneNumber::Type::Home:
            type = "Home";
            break;

        case PhoneNumber::Type::Service:
            type = "Service";
            break;
        }

        std::cout << "  " << phone.getNumber()
                  << " (" << type << ")\n";
    }
}

void ConsoleUI::showContacts() const
{
    const auto &contacts = m_phoneBook.getContacts();

    if (contacts.empty())
    {
        std::cout << "Phone book is empty.\n";
        return;
    }

    for (const auto &contact : contacts)
    {
        printContact(contact);
    }

    std::cout << "\nTotal contacts: "
              << contacts.size() << '\n';
}

void ConsoleUI::searchContacts() const
{
    std::string query;

    std::cout << "Enter search query: ";

    if (!std::getline(std::cin, query))
    {
        return;
    }

    const auto results = m_phoneBook.searchContacts(query);

    if (results.empty())
    {
        std::cout << "No contacts found.\n";
        return;
    }

    for (const auto &contact : results)
    {
        printContact(contact);
    }

    std::cout << "\nContacts found: "
              << results.size() << '\n';
}

void ConsoleUI::removeContact()
{
    std::string input;

    std::cout << "Enter contact ID: ";

    if (!std::getline(std::cin, input))
    {
        return;
    }

    std::istringstream stream(input);

    int id;
    char extra;

    if (!(stream >> id) || id <= 0 || (stream >> extra))
    {
        std::cout << "Invalid ID.\n";
        return;
    }

    if (m_phoneBook.removeContact(id))
    {
        std::cout << "Contact removed successfully.\n";
    }
    else
    {
        std::cout << "Contact not found.\n";
    }
}
bool ConsoleUI::readContactData(
    std::string &firstName,
    std::string &lastName,
    std::string &email,
    std::vector<PhoneNumber> &phoneNumbers,
    std::string &middleName,
    std::string &address,
    std::string &birthDate)
{
    std::cout << "First name: ";
    if (!std::getline(std::cin, firstName))
        return false;

    std::cout << "Last name: ";
    if (!std::getline(std::cin, lastName))
        return false;

    std::cout << "Middle name (optional): ";
    if (!std::getline(std::cin, middleName))
        return false;

    std::cout << "Address (optional): ";
    if (!std::getline(std::cin, address))
        return false;

    std::cout << "Birth date (DD.MM.YYYY, optional): ";
    if (!std::getline(std::cin, birthDate))
        return false;

    std::cout << "Email: ";
    if (!std::getline(std::cin, email))
        return false;

    phoneNumbers.clear();

    while (true)
    {
        std::string number;
        std::string typeInput;

        std::cout << "Phone number (empty to finish): ";

        if (!std::getline(std::cin, number))
            return false;

        if (number.empty())
        {
            if (phoneNumbers.empty())
            {
                std::cout << "At least one phone is required.\n";
                continue;
            }

            break;
        }

        PhoneNumber::Type type;

        while (true)
        {
            std::cout << "1. Work\n"
                      << "2. Home\n"
                      << "3. Service\n"
                      << "Choose phone type: ";

            if (!std::getline(std::cin, typeInput))
                return false;

            if (typeInput == "1")
            {
                type = PhoneNumber::Type::Work;
                break;
            }
            else if (typeInput == "2")
            {
                type = PhoneNumber::Type::Home;
                break;
            }
            else if (typeInput == "3")
            {
                type = PhoneNumber::Type::Service;
                break;
            }

            std::cout << "Invalid phone type. Try again.\n";
        }

        phoneNumbers.emplace_back(number, type);
    }

    return true;
}

void ConsoleUI::addContact()
{
    std::string firstName;
    std::string lastName;
    std::string middleName;
    std::string address;
    std::string birthDate;
    std::string email;

    std::vector<PhoneNumber> phoneNumbers;

    std::cout << "\n===== ADD CONTACT =====\n";

    if (!readContactData(
            firstName,
            lastName,
            email,
            phoneNumbers,
            middleName,
            address,
            birthDate))
    {
        return;
    }

    if (m_phoneBook.addContact(
            firstName,
            lastName,
            email,
            phoneNumbers,
            middleName,
            address,
            birthDate))
    {
        std::cout << "Contact added successfully.\n";
    }
    else
    {
        std::cout << "Invalid contact data.\n";
    }
}

void ConsoleUI::editContact()
{
    std::string input;

    std::cout << "Enter contact ID: ";

    if (!std::getline(std::cin, input))
        return;

    std::istringstream stream(input);

    int id;
    char extra;

    if (!(stream >> id) || id <= 0 || (stream >> extra))
    {
        std::cout << "Invalid ID.\n";
        return;
    }

    bool found = false;

    for (const auto &contact : m_phoneBook.getContacts())
    {
        if (contact.getId() == id)
        {
            printContact(contact);
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "Contact not found.\n";
        return;
    }

    std::string firstName;
    std::string lastName;
    std::string middleName;
    std::string address;
    std::string birthDate;
    std::string email;

    std::vector<PhoneNumber> phoneNumbers;

    std::cout << "\n===== ENTER NEW CONTACT DATA =====\n";

    if (!readContactData(
            firstName,
            lastName,
            email,
            phoneNumbers,
            middleName,
            address,
            birthDate))
    {
        return;
    }

    if (m_phoneBook.editContact(
            id,
            firstName,
            lastName,
            email,
            phoneNumbers,
            middleName,
            address,
            birthDate))
    {
        std::cout << "Contact updated successfully.\n";
    }
    else
    {
        std::cout << "Invalid contact data.\n";
    }
}

void ConsoleUI::sortContacts()
{
    std::cout << "\n===== SORT CONTACTS =====\n"
              << "1. ID\n"
              << "2. First name\n"
              << "3. Last name\n"
              << "4. Middle name\n"
              << "5. Address\n"
              << "6. Birth date\n"
              << "7. Email\n"
              << "8. Phone number\n"
              << "0. Cancel\n"
              << "Choose sort field: ";

    std::string choice;

    if (!std::getline(std::cin, choice))
        return;

    using SortField = PhoneBook::SortField;

    SortField field;

    if (choice == "1")
        field = SortField::Id;
    else if (choice == "2")
        field = SortField::FirstName;
    else if (choice == "3")
        field = SortField::LastName;
    else if (choice == "4")
        field = SortField::MiddleName;
    else if (choice == "5")
        field = SortField::Address;
    else if (choice == "6")
        field = SortField::BirthDate;
    else if (choice == "7")
        field = SortField::Email;
    else if (choice == "8")
        field = SortField::PhoneNumber;
    else if (choice == "0")
        return;
    else
    {
        std::cout << "Invalid sort field.\n";
        return;
    }

    std::cout << "1. Ascending\n"
              << "2. Descending\n"
              << "Choose sort direction: ";

    std::string direction;

    if (!std::getline(std::cin, direction))
        return;

    if (direction != "1" && direction != "2")
    {
        std::cout << "Invalid sort direction.\n";
        return;
    }

    m_phoneBook.sortContacts(field, direction == "1");

    std::cout << "Contacts sorted successfully.\n";

    showContacts();
}