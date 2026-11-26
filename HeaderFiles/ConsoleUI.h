#pragma once

#include "PhoneBook.h"

class ConsoleUI
{
public:
    explicit ConsoleUI(PhoneBook &phoneBook);

    void run();

private:
    PhoneBook &m_phoneBook;
    void showMenu() const;
    void showContacts() const;
    void addContact();
    void removeContact();
    void editContact();
    void searchContacts() const;
    void sortContacts();
    void printContact(const Contact &contact) const;
    bool readContactData(
        std::string &firstName,
        std::string &lastName,
        std::string &email,
        std::vector<PhoneNumber> &phoneNumbers,
        std::string &middleName,
        std::string &address,
        std::string &birthDate);
};