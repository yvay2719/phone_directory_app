#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <stdexcept>
#include "../FileStorage.h"

int passed = 0;
int failed = 0;

void check(bool condition, const std::string &testName)
{
    if (condition)
    {
        std::cout << "\033[32m[PASS]\033[0m ";
        ++passed;
    }
    else
    {
        std::cout << "\033[31m[FAIL]\033[0m ";
        ++failed;
    }

    std::cout << testName << '\n';
}

int main()
{
    FileStorage storage("test_contacts.txt");
    PhoneNumber homePhone("+78121234567", PhoneNumber::Type::Home);
    PhoneNumber workPhone("+79001234567", PhoneNumber::Type::Work);
    Contact contact(1, "Ivan", "Ivanov", "ivan@mail.ru", {homePhone, workPhone});
    contact.setMiddleName("Ivanovich");
    contact.setAddress("Moscow, Central Street 15");
    contact.setBirthDate("15.05.2000");

    std::vector<Contact> contacts = {contact};
    storage.save(contacts);

    std::ifstream file("test_contacts.txt");
    check(file.is_open(), "Storage file created");

    // проверка содержимого сохраненного файла
    std::size_t contactCount = 0;
    file >> contactCount;

    check(contactCount == 1, "Saved contact count");

    int id = 0;
    std::string firstName;
    std::string lastName;
    std::string middleName;
    std::string address;
    std::string birthDate;
    std::string email;

    file >> id >> std::quoted(firstName) >> std::quoted(lastName) >> std::quoted(middleName) >> std::quoted(address) >> std::quoted(birthDate) >> std::quoted(email);

    check(!file.fail(), "Read contact fields");
    check(id == 1, "Saved ID");
    check(firstName == "Ivan", "Saved first name");
    check(lastName == "Ivanov", "Saved last name");
    check(middleName == "Ivanovich", "Saved middle name");
    check(address == "Moscow, Central Street 15", "Saved address");
    check(birthDate == "15.05.2000", "Saved birth date");
    check(email == "ivan@mail.ru", "Saved email");

    // проверка сохраненных номеров телефона
    std::size_t phoneCount = 0;
    file >> phoneCount;

    check(phoneCount == 2, "Saved phone count");

    std::string phoneNumber;
    int phoneType = -1;

    // First phone
    file >> std::quoted(phoneNumber) >> phoneType;

    check(!file.fail(), "Read first phone");
    check(phoneNumber == "+78121234567", "Saved home number");
    check(phoneType == 1, "Saved home phone type");

    // Second phone
    file >> std::quoted(phoneNumber) >> phoneType;

    check(!file.fail(), "Read second phone");
    check(phoneNumber == "+79001234567", "Saved work number");
    check(phoneType == 0, "Saved work phone type");

    file.close();
    // загруска контакта из файла
    std::vector<Contact> loadedContacts = storage.load();

    check(loadedContacts.size() == 1, "Loaded contact count");

    if (!loadedContacts.empty())
    {
        const auto &loaded = loadedContacts[0];

        check(loaded.getId() == 1, "Loaded ID");
        check(loaded.getFirstName() == "Ivan", "Loaded first name");
        check(loaded.getLastName() == "Ivanov", "Loaded last name");
        check(loaded.getMiddleName() == "Ivanovich", "Loaded middle name");
        check(loaded.getAddress() == "Moscow, Central Street 15",
              "Loaded address");
        check(loaded.getBirthDate() == "15.05.2000", "Loaded birth date");
        check(loaded.getEmail() == "ivan@mail.ru", "Loaded email");

        const auto &loadedPhones = loaded.getPhoneNumbers();

        check(loadedPhones.size() == 2, "Loaded phone count");
        // проверка загрузки телефонных номеров
        if (loadedPhones.size() == 2)
        {
            check(loadedPhones[0].getNumber() == "+78121234567", "Loaded home number");
            check(loadedPhones[0].getType() == PhoneNumber::Type::Home, "Loaded home phone type");
            check(loadedPhones[1].getNumber() == "+79001234567", "Loaded work number");
            check(loadedPhones[1].getType() == PhoneNumber::Type::Work, "Loaded work phone type");
        }
    }

    // сохранение нескольких контактов
    PhoneNumber servicePhone("+79991234567", PhoneNumber::Type::Service);
    Contact secondContact(2, "Anna", "Petrova", "anna@mail.ru", {servicePhone});
    contacts.push_back(secondContact);
    storage.save(contacts);
    loadedContacts = storage.load();
    check(loadedContacts.size() == 2, "Multiple contacts");

    if (loadedContacts.size() == 2)
    {
        check(loadedContacts[0].getId() == 1, "First contact preserved");
        check(loadedContacts[1].getId() == 2, "Second contact ID");
        check(loadedContacts[1].getFirstName() == "Anna", "Second contact name");
        check(loadedContacts[1].getMiddleName().empty(), "Empty middle name");
        check(loadedContacts[1].getAddress().empty(), "Empty address");
        check(loadedContacts[1].getBirthDate().empty(), "Empty birth date");
    }

    // пустое хранилище
    storage.save({});
    loadedContacts = storage.load();
    check(loadedContacts.empty(), "Empty storage");

    // Missing storage file
    std::remove("test_contacts.txt");
    loadedContacts = storage.load();
    check(loadedContacts.empty(), "Missing storage file");

    // сломанный файл
    //  Corrupted storage file
    {
        std::ofstream corruptedFile("test_contacts.txt");
        corruptedFile << "invalid data\n";
    }

    bool exceptionCaught = false;
    try
    {
        storage.load();
    }
    catch (const std::runtime_error &)
    {
        exceptionCaught = true;
    }
    check(exceptionCaught, "Corrupted storage file");
    std::remove("test_contacts.txt");

    // проверка неполной записи контакта
    {
        std::ofstream incompleteFile("test_contacts.txt");

        incompleteFile << "1\n";
        incompleteFile << "1\n";
        incompleteFile << "\"Ivan\"\n";
    }

    exceptionCaught = false;

    try
    {
        storage.load();
    }
    catch (const std::runtime_error &)
    {
        exceptionCaught = true;
    }
    check(exceptionCaught, "Incomplete contact");
    std::remove("test_contacts.txt");

    std::cout << "\nTests passed: " << passed << "/"
              << passed + failed << '\n';
    std::cout << "Tests failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}