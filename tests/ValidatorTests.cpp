#include <iostream>
#include "../HeaderFiles/Validator.h"
#include <ctime>

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
    check(Validator::isValidName("Ivan"), "Simple name");
    check(Validator::isValidName("Ivan-Petr"), "Hyphenated name");
    check(Validator::isValidName("Ivan2"), "Name with digit");
    check(Validator::isValidName("Ivan Petr"), "Name with space");
    check(Validator::isValidName("  Ivan  "), "Trimmed name");
    check(Validator::isValidName("I"), "Single letter");

    check(!Validator::isValidName("2Ivan"), "Leading digit");
    check(!Validator::isValidName("-Ivan"), "Leading hyphen");
    check(!Validator::isValidName("Ivan-"), "Trailing hyphen");
    check(!Validator::isValidName("Ivan@"), "Invalid character");
    check(!Validator::isValidName(""), "Empty name");
    check(!Validator::isValidName("   "), "Only spaces");

    // Cyrillic names
    check(Validator::isValidName("Иван"), "Cyrillic name");
    check(Validator::isValidName("Иван-Петр"), "Cyrillic hyphenated name");
    check(Validator::isValidName("Пётр"), "Name with lowercase yo");
    check(Validator::isValidName("Ёлкин"), "Name with uppercase yo");
    check(Validator::isValidName("  Иван  "), "Trimmed Cyrillic name");
    check(Validator::isValidName("Иван2"), "Cyrillic name with digit");

    check(!Validator::isValidName("2Иван"), "Cyrillic leading digit");
    check(!Validator::isValidName("-Иван"), "Cyrillic leading hyphen");
    check(!Validator::isValidName("Иван-"), "Cyrillic trailing hyphen");
    check(!Validator::isValidName("Иван@"), "Cyrillic invalid character");

    // Valid phone numbers
    check(Validator::isValidPhone("+79211234567"), "Phone format 1");
    check(Validator::isValidPhone("89211234567"), "Phone format 2");
    check(Validator::isValidPhone("+7(921)1234567"), "Phone format 3");
    check(Validator::isValidPhone("8(921)1234567"), "Phone format 4");
    check(Validator::isValidPhone("+7(921)123-45-67"), "Phone format 5");
    check(Validator::isValidPhone("8(921)123-45-67"), "Phone format 6");

    check(Validator::isValidPhone("  +79211234567  "), "Trimmed phone");

    // Invalid phone numbers
    check(!Validator::isValidPhone("+7921123456"), "Short phone");
    check(!Validator::isValidPhone("+792112345678"), "Long phone");
    check(!Validator::isValidPhone("+19211234567"), "Unsupported country code");
    check(!Validator::isValidPhone("+7921123456A"), "Phone with letter");
    check(!Validator::isValidPhone("+7(9211234567"), "Missing bracket");
    check(!Validator::isValidPhone("+7(921)123-4567"), "Invalid hyphens");
    check(!Validator::isValidPhone("+7 921 1234567"), "Internal spaces");
    check(!Validator::isValidPhone(""), "Empty phone");

    // Valid emails
    check(Validator::isValidEmail("ivan@mail.ru"), "Simple email");
    check(Validator::isValidEmail("ivan123@gmail.com"), "Email with digits");
    check(Validator::isValidEmail("Ivan@Mail.ru"), "Uppercase email");
    check(Validator::isValidEmail("ivan@mail.co.uk"), "Multiple domains");
    check(Validator::isValidEmail("  ivan@mail.ru  "), "Trimmed email");
    check(Validator::isValidEmail("ivan @ mail.ru"), "Spaces around @");

    // Invalid emails
    check(!Validator::isValidEmail("ivanmail.ru"), "Missing @");
    check(!Validator::isValidEmail("@mail.ru"), "Missing username");
    check(!Validator::isValidEmail("ivan@"), "Missing domain");
    check(!Validator::isValidEmail("ivan@mail"), "Missing domain suffix");
    check(!Validator::isValidEmail("иван@mail.ru"), "Cyrillic email");
    check(!Validator::isValidEmail("iv an@mail.ru"), "Space in username");
    check(!Validator::isValidEmail("ivan@@mail.ru"), "Double @");
    check(!Validator::isValidEmail("ivan@mail..ru"), "Double dot");
    check(!Validator::isValidEmail(""), "Empty email");

    // Valid birth dates
    check(Validator::isValidBirthDate("07.07.2007"), "Simple date");
    check(Validator::isValidBirthDate("29.02.2000"), "Leap year 2000");
    check(Validator::isValidBirthDate("29.02.2024"), "Leap year 2024");
    check(Validator::isValidBirthDate("30.04.2000"), "Last day of April");
    check(Validator::isValidBirthDate("  15.05.2000  "), "Trimmed date");

    // Invalid birth dates
    check(!Validator::isValidBirthDate("29.02.1900"), "Non-leap year 1900");
    check(!Validator::isValidBirthDate("29.02.2023"), "Non-leap year 2023");
    check(!Validator::isValidBirthDate("31.04.2000"), "Invalid April day");
    check(!Validator::isValidBirthDate("31.02.2000"), "Invalid February day");
    check(!Validator::isValidBirthDate("00.05.2000"), "Zero day");
    check(!Validator::isValidBirthDate("15.00.2000"), "Zero month");
    check(!Validator::isValidBirthDate("15.13.2000"), "Invalid month");
    check(!Validator::isValidBirthDate("15.05.0000"), "Zero year");
    check(!Validator::isValidBirthDate("01.01.9999"), "Future date");
    check(!Validator::isValidBirthDate("1.5.2000"), "Invalid date format");
    check(!Validator::isValidBirthDate("15/05/2000"), "Invalid separator");
    check(!Validator::isValidBirthDate(""), "Empty date");

    std::time_t now = std::time(nullptr);
    char today[11];

    std::strftime(today, sizeof(today), "%d.%m.%Y", std::localtime(&now));

    check(!Validator::isValidBirthDate(today), "Today's date");

    // Contact validation
    PhoneNumber validPhone("+78121234567", PhoneNumber::Type::Home);
    Contact validContact(1, "Ivan", "Ivanov", "ivan@mail.ru", {validPhone});
    check(Validator::isValidContact(validContact), "Valid contact");

    // Invalid first name
    Contact invalidContact = validContact;
    invalidContact.setFirstName("");
    check(!Validator::isValidContact(invalidContact), "Missing first name");

    // Invalid last name
    invalidContact = validContact;
    invalidContact.setLastName("");
    check(!Validator::isValidContact(invalidContact), "Missing last name");

    // Invalid email
    invalidContact = validContact;
    invalidContact.setEmail("invalid");
    check(!Validator::isValidContact(invalidContact), "Invalid contact email");

    // Missing phone numbers
    Contact noPhones(2, "Ivan", "Ivanov", "ivan@mail.ru", {});
    check(!Validator::isValidContact(noPhones), "Contact without phones");

    // Invalid phone number
    invalidContact = validContact;
    invalidContact.addPhoneNumber(PhoneNumber("123", PhoneNumber::Type::Work));
    check(!Validator::isValidContact(invalidContact), "Contact with invalid phone");

    // Optional fields
    Contact fullContact = validContact;
    fullContact.setMiddleName("Ivanovich");
    fullContact.setAddress("Moscow");
    fullContact.setBirthDate("15.05.2000");
    check(Validator::isValidContact(fullContact), "Contact with all fields");

    // Invalid middle name
    invalidContact = validContact;
    invalidContact.setMiddleName("123");
    check(!Validator::isValidContact(invalidContact), "Invalid middle name");

    // Invalid birth date
    invalidContact = validContact;
    invalidContact.setBirthDate("31.02.2000");
    check(!Validator::isValidContact(invalidContact), "Invalid contact birth date");

    // Contact normalization
    Contact normalizedContact(1, "  Ivan  ", "  Ivanov  ", "  ivan @ mail.ru  ",
                              {PhoneNumber("  +78121234567  ", PhoneNumber::Type::Home)});

    normalizedContact.setMiddleName("  Ivanovich  ");
    normalizedContact.setAddress("  Moscow  ");
    normalizedContact.setBirthDate("  15.05.2000  ");

    Validator::normalizeContact(normalizedContact);

    check(normalizedContact.getFirstName() == "Ivan", "Normalize first name");
    check(normalizedContact.getLastName() == "Ivanov", "Normalize last name");
    check(normalizedContact.getMiddleName() == "Ivanovich", "Normalize middle name");
    check(normalizedContact.getAddress() == "Moscow", "Normalize address");
    check(normalizedContact.getBirthDate() == "15.05.2000", "Normalize birth date");

    check(normalizedContact.getEmail() == "ivan@mail.ru",
          "Normalize email");

    const auto &normalizedPhones =
        normalizedContact.getPhoneNumbers();

    check(normalizedPhones.size() == 1,
          "Preserve phone count");

    if (normalizedPhones.size() == 1)
    {
        check(normalizedPhones[0].getNumber() == "+78121234567", "Normalize phone number");
        check(normalizedPhones[0].getType() == PhoneNumber::Type::Home, "Preserve phone type");
    }
    check(Validator::isValidContact(normalizedContact), "Normalized contact is valid");
    check(Validator::isValidContact(validContact), "Empty optional fields");
    std::cout << "\nTests passed: " << passed << "/" << passed + failed << '\n';
    std::cout << "Tests failed: " << failed << '\n';

    return failed == 0 ? 0 : 1;
}