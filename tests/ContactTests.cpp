#include <iostream>
#include "../Contact.h"

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
    PhoneNumber phone("+78121234567", PhoneNumber::Type::Home);

    Contact contact(1, "Ivan", "Ivanov",
                    "ivan@mail.ru", {phone});

    contact.setMiddleName("Ivanovich");
    contact.setAddress("Saint-Petersburg");
    contact.setBirthDate("07.07.2007");

    check(contact.getId() == 1, "Contact ID");
    check(contact.getFirstName() == "Ivan", "First name");
    check(contact.getLastName() == "Ivanov", "Last name");
    check(contact.getMiddleName() == "Ivanovich", "Middle name");
    check(contact.getAddress() == "Saint-Petersburg", "Address");
    check(contact.getBirthDate() == "07.07.2007", "Birth date");
    check(contact.getEmail() == "ivan@mail.ru", "Email");

    check(contact.getPhoneNumbers().size() == 1 && contact.getPhoneNumbers()[0].getNumber() == "+78121234567", "Initial phone number");

    // Add phone number
    contact.addPhoneNumber(PhoneNumber("+79001234567", PhoneNumber::Type::Work));

    check(contact.getPhoneNumbers().size() == 2 && contact.getPhoneNumbers()[1].getNumber() == "+79001234567", "Add phone number");

    // Edit phone number
    check(contact.editPhoneNumber(0, PhoneNumber("+79991234567", PhoneNumber::Type::Home)), "Edit phone number");

    check(!contact.getPhoneNumbers().empty() && contact.getPhoneNumbers()[0].getNumber() == "+79991234567", "Verify edited number");

    // Remove phone number
    check(contact.removePhoneNumber(0), "Remove phone number");

    check(contact.getPhoneNumbers().size() == 1 && contact.getPhoneNumbers()[0].getNumber() == "+79001234567", "Verify remaining number");

    // Edge cases
    check(!contact.removePhoneNumber(0), "Prevent last phone removal");

    check(!contact.removePhoneNumber(10), "Remove invalid index");

    check(!contact.editPhoneNumber(10, phone), "Edit invalid index");

    check(contact.getPhoneNumbers().size() == 1, "Verify phone count");

    std::cout << "\n=================================\n";
    std::cout << "Tests passed: " << passed << "/"
              << passed + failed << '\n';
    std::cout << "Tests failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}