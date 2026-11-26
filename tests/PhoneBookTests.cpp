#include <iostream>
#include <string>
#include <memory>
#include <cstdio>

#include "../PhoneBook.h"
#include "../FileStorage.h"

int passed = 0;
int failed = 0;

void check(bool condition, const std::string &testName)
{
    if (condition)
    {
        ++passed;
        std::cout << "\033[32m[PASS]\033[0m ";
    }
    else
    {
        ++failed;
        std::cout << "\033[31m[FAIL]\033[0m ";
    }

    std::cout << testName << '\n';
}

int main()
{
    std::remove("test_phonebook.txt");

    PhoneBook phoneBook(
        std::make_unique<FileStorage>("test_phonebook.txt"));

    check(phoneBook.getContacts().empty(),
          "Initially empty phone book");

    // Add first contact
    bool added = phoneBook.addContact(
        "Ivan",
        "Ivanov",
        "ivan@mail.ru",
        {PhoneNumber("+78121234567", PhoneNumber::Type::Home)});

    check(added, "Add valid contact");

    check(phoneBook.getContacts().size() == 1,
          "Contact count after adding");

    if (phoneBook.getContacts().size() == 1)
    {
        const auto &contact = phoneBook.getContacts()[0];
        check(contact.getId() == 1, "First contact ID");
        check(contact.getFirstName() == "Ivan", "Added contact first name");
        check(contact.getLastName() == "Ivanov", "Added contact last name");
        check(contact.getEmail() == "ivan@mail.ru", "Added contact email");
    }

    // Reload phone book
    PhoneBook reloadedPhoneBook(
        std::make_unique<FileStorage>("test_phonebook.txt"));

    check(reloadedPhoneBook.getContacts().size() == 1,
          "Contact persisted after reload");

    if (reloadedPhoneBook.getContacts().size() == 1)
    {
        const auto &contact = reloadedPhoneBook.getContacts()[0];
        check(contact.getId() == 1, "Reloaded contact ID");
        check(contact.getFirstName() == "Ivan", "Reloaded contact first name");
        check(contact.getLastName() == "Ivanov", "Reloaded contact last name");
        check(contact.getEmail() == "ivan@mail.ru", "Reloaded contact email");
    }

    // Add contact after reload
    added = reloadedPhoneBook.addContact(
        "Anna",
        "Petrova",
        "anna@mail.ru",
        {PhoneNumber("+79991234567", PhoneNumber::Type::Work)});

    check(added, "Add second contact");

    check(reloadedPhoneBook.getContacts().size() == 2,
          "Two contacts in phone book");

    if (reloadedPhoneBook.getContacts().size() == 2)
    {
        check(reloadedPhoneBook.getContacts()[1].getId() == 2,
              "Next ID after reload");
    }

    // Invalid contact
    added = reloadedPhoneBook.addContact(
        "",
        "Petrov",
        "invalid-email",
        {});

    check(!added, "Reject invalid contact");

    check(reloadedPhoneBook.getContacts().size() == 2,
          "Contact count unchanged after rejection");

    // Normalization
    added = reloadedPhoneBook.addContact(
        "  Petr  ",
        "Petrov",
        "petr @ mail.ru",
        {PhoneNumber("  +79001234567  ", PhoneNumber::Type::Home)});

    check(added, "Add contact with extra spaces");

    if (reloadedPhoneBook.getContacts().size() == 3)
    {
        const auto &contact = reloadedPhoneBook.getContacts()[2];
        check(contact.getId() == 3, "ID unchanged after rejection");
        check(contact.getFirstName() == "Petr", "Added contact normalized");
        check(contact.getEmail() == "petr@mail.ru", "Added email normalized");
    }

    // Remove contact
    bool removed = reloadedPhoneBook.removeContact(2);

    check(removed, "Remove existing contact");

    check(reloadedPhoneBook.getContacts().size() == 2,
          "Contact count after removal");

    if (reloadedPhoneBook.getContacts().size() == 2)
    {
        check(reloadedPhoneBook.getContacts()[0].getId() == 1,
              "First contact preserved");

        check(reloadedPhoneBook.getContacts()[1].getId() == 3,
              "Third contact preserved");
    }

    // Remove nonexistent contact
    removed = reloadedPhoneBook.removeContact(999);

    check(!removed, "Reject nonexistent contact removal");

    check(reloadedPhoneBook.getContacts().size() == 2,
          "Contact count unchanged");

    // Check persistence
    PhoneBook afterRemoval(std::make_unique<FileStorage>("test_phonebook.txt"));
    check(afterRemoval.getContacts().size() == 2, "Removal persisted after reload");

    // Edit existing contact
    bool edited = reloadedPhoneBook.editContact(
        1,
        "Alex",
        "Ivanov",
        "alex@mail.ru",
        {PhoneNumber("+79005554433", PhoneNumber::Type::Work)},
        "Petrovich",
        "Moscow",
        "10.10.2000");

    check(edited, "Edit existing contact");

    check(reloadedPhoneBook.getContacts().size() == 2,
          "Contact count unchanged after editing");

    if (reloadedPhoneBook.getContacts().size() == 2)
    {
        const auto &contact = reloadedPhoneBook.getContacts()[0];

        check(contact.getId() == 1, "Preserve contact ID");

        check(contact.getFirstName() == "Alex",
              "Updated first name");

        check(contact.getEmail() == "alex@mail.ru",
              "Updated email");

        check(contact.getMiddleName() == "Petrovich",
              "Updated middle name");

        check(contact.getAddress() == "Moscow",
              "Updated address");

        check(contact.getBirthDate() == "10.10.2000",
              "Updated birth date");

        check(contact.getPhoneNumbers().size() == 1 &&
                  contact.getPhoneNumbers()[0].getNumber() == "+79005554433",
              "Updated phone number");
    }

    // Invalid data
    edited = reloadedPhoneBook.editContact(
        1, "", "Ivanov", "invalid", {}, "", "", "");

    check(!edited, "Reject invalid contact editing");

    check(!reloadedPhoneBook.getContacts().empty() &&
              reloadedPhoneBook.getContacts()[0].getFirstName() == "Alex",
          "Preserve contact after invalid editing");

    // Nonexistent contact
    edited = reloadedPhoneBook.editContact(
        999, "Alex", "Ivanov", "alex@mail.ru",
        {PhoneNumber("+79005554433", PhoneNumber::Type::Work)},
        "", "", "");

    check(!edited, "Reject nonexistent contact editing");

    // Check persistence
    PhoneBook afterEditing(std::make_unique<FileStorage>("test_phonebook.txt"));
    check(!afterEditing.getContacts().empty() && afterEditing.getContacts()[0].getFirstName() == "Alex", "Editing persisted after reload");

    // Search contacts
    check(reloadedPhoneBook.searchContacts("Alex").size() == 1, "Search by first name");
    check(reloadedPhoneBook.searchContacts("Ivanov").size() == 1, "Search by last name");
    check(reloadedPhoneBook.searchContacts("Petrovich").size() == 1, "Search by middle name");
    check(reloadedPhoneBook.searchContacts("Moscow").size() == 1, "Search by address");
    check(reloadedPhoneBook.searchContacts("10.10.2000").size() == 1, "Search by birth date");
    check(reloadedPhoneBook.searchContacts("alex@mail.ru").size() == 1, "Search by email");
    check(reloadedPhoneBook.searchContacts("+79005554433").size() == 1, "Search by phone number");
    check(reloadedPhoneBook.searchContacts("Ale").size() == 1, "Search by partial match");
    check(reloadedPhoneBook.searchContacts("  Alex  ").size() == 1, "Search with extra spaces");
    check(reloadedPhoneBook.searchContacts("mail.ru").size() == 2, "Search multiple contacts");
    check(reloadedPhoneBook.searchContacts("Unknown").empty(), "Search without matches");
    check(reloadedPhoneBook.searchContacts("").size() == 2, "Search with empty query");
    check(reloadedPhoneBook.getContacts().size() == 2, "Search does not modify contacts");

    // Sort contacts
    using SortField = PhoneBook::SortField;

    reloadedPhoneBook.sortContacts(SortField::Id, false);
    check(reloadedPhoneBook.getContacts()[0].getId() == 3, "Sort by ID descending");
    reloadedPhoneBook.sortContacts(SortField::Id);
    check(reloadedPhoneBook.getContacts()[0].getId() == 1, "Sort by ID ascending");
    reloadedPhoneBook.sortContacts(SortField::FirstName);
    check(reloadedPhoneBook.getContacts()[0].getFirstName() == "Alex", "Sort by first name ascending");
    reloadedPhoneBook.sortContacts(SortField::FirstName, false);
    check(reloadedPhoneBook.getContacts()[0].getFirstName() == "Petr", "Sort by first name descending");
    reloadedPhoneBook.sortContacts(SortField::LastName);
    check(reloadedPhoneBook.getContacts()[0].getLastName() == "Ivanov", "Sort by last name");
    reloadedPhoneBook.sortContacts(SortField::MiddleName);
    check(reloadedPhoneBook.getContacts()[0].getMiddleName().empty(), "Sort by middle name");
    reloadedPhoneBook.sortContacts(SortField::Address);
    check(reloadedPhoneBook.getContacts()[0].getAddress().empty(), "Sort by address");
    reloadedPhoneBook.sortContacts(SortField::BirthDate);
    check(reloadedPhoneBook.getContacts()[0].getBirthDate().empty(), "Sort by birth date ascending");
    reloadedPhoneBook.sortContacts(SortField::BirthDate, false);
    check(reloadedPhoneBook.getContacts()[0].getBirthDate() == "10.10.2000", "Sort by birth date descending");
    reloadedPhoneBook.sortContacts(SortField::Email);
    check(reloadedPhoneBook.getContacts()[0].getEmail() == "alex@mail.ru", "Sort by email");
    reloadedPhoneBook.sortContacts(SortField::PhoneNumber);
    check(reloadedPhoneBook.getContacts()[0].getId() == 3, "Sort by phone number");

    check(reloadedPhoneBook.getContacts().size() == 2, "Contact count unchanged after sorting");

    std::remove("test_phonebook.txt");

    std::cout << "\nTests passed: "
              << passed << "/" << passed + failed << '\n';

    std::cout << "Tests failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}