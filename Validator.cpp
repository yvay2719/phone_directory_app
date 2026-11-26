#include "Validator.h"
#include <regex>
#include <ctime>

std::string Validator::trim(const std::string &value)
{
    std::size_t start = value.find_first_not_of(" \t\n\r");

    if (start == std::string::npos)
    {
        return "";
    }

    std::size_t end = value.find_last_not_of(" \t\n\r");

    return value.substr(start, end - start + 1);
}

bool Validator::isValidName(const std::string &name)
{
    std::string cleanedName = trim(name);

    static const std::regex pattern(
        R"((?:[A-Za-z]|\xD0[\x81\x90-\xBF]|\xD1[\x80-\x8F\x91])(?:(?:[A-Za-z0-9 -]|\xD0[\x81\x90-\xBF]|\xD1[\x80-\x8F\x91])*(?:[A-Za-z0-9]|\xD0[\x81\x90-\xBF]|\xD1[\x80-\x8F\x91]))?)");

    return std::regex_match(cleanedName, pattern);
}

bool Validator::isValidPhone(const std::string &phone)
{
    std::string cleanedPhone = trim(phone);

    static const std::regex pattern(
        R"((\+7|8)([0-9]{10}|\([0-9]{3}\)([0-9]{7}|[0-9]{3}-[0-9]{2}-[0-9]{2})))");

    return std::regex_match(cleanedPhone, pattern);
}

bool Validator::isValidEmail(const std::string &email)
{
    std::string cleanedEmail = trim(email);

    static const std::regex pattern(
        R"([A-Za-z0-9]+[ \t]*@[ \t]*[A-Za-z0-9]+(\.[A-Za-z0-9]+)+)");

    return std::regex_match(cleanedEmail, pattern);
}

bool Validator::isValidBirthDate(const std::string &birthDate)
{
    std::string cleanedDate = trim(birthDate);

    static const std::regex pattern(
        R"([0-9]{2}\.[0-9]{2}\.[0-9]{4})");

    if (!std::regex_match(cleanedDate, pattern))
    {
        return false;
    }

    int day = std::stoi(cleanedDate.substr(0, 2));
    int month = std::stoi(cleanedDate.substr(3, 2));
    int year = std::stoi(cleanedDate.substr(6, 4));

    if (day < 1 || day > 31 ||
        month < 1 || month > 12 ||
        year < 1)
    {
        return false;
    }

    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isLeapYear)
    {
        daysInMonth[1] = 29;
    }

    if (day > daysInMonth[month - 1])
    {
        return false;
    }

    std::time_t now = std::time(nullptr);
    std::tm *today = std::localtime(&now);

    if (today == nullptr)
    {
        return false;
    }

    int currentYear = today->tm_year + 1900;
    int currentMonth = today->tm_mon + 1;
    int currentDay = today->tm_mday;

    if (year > currentYear || (year == currentYear && month > currentMonth) ||
        (year == currentYear && month == currentMonth && day >= currentDay))
    {
        return false;
    }

    return true;
}

bool Validator::isValidContact(const Contact &contact)
{
    // Required fields
    if (!isValidName(contact.getFirstName()) || !isValidName(contact.getLastName()) || !isValidEmail(contact.getEmail()))
    {
        return false;
    }

    // Optional fields
    if (!trim(contact.getMiddleName()).empty() && !isValidName(contact.getMiddleName()))
    {
        return false;
    }

    if (!trim(contact.getBirthDate()).empty() && !isValidBirthDate(contact.getBirthDate()))
    {
        return false;
    }

    // Phone numbers
    const auto &phones = contact.getPhoneNumbers();

    if (phones.empty())
    {
        return false;
    }

    for (const auto &phone : phones)
    {
        if (!isValidPhone(phone.getNumber()))
        {
            return false;
        }
    }

    return true;
}

void Validator::normalizeContact(Contact &contact)
{
    contact.setFirstName(trim(contact.getFirstName()));
    contact.setLastName(trim(contact.getLastName()));
    contact.setMiddleName(trim(contact.getMiddleName()));
    contact.setAddress(trim(contact.getAddress()));
    contact.setBirthDate(trim(contact.getBirthDate()));
    std::string email = trim(contact.getEmail());

    std::size_t atPos = email.find('@');
    if (atPos != std::string::npos)
    {
        std::string username = trim(email.substr(0, atPos));
        std::string domain = trim(email.substr(atPos + 1));
        email = username + "@" + domain;
    }
    contact.setEmail(email);
    for (std::size_t i = 0;
         i < contact.getPhoneNumbers().size();
         ++i)
    {

        PhoneNumber phone = contact.getPhoneNumbers()[i];
        phone.setNumber(trim(phone.getNumber()));
        contact.editPhoneNumber(i, phone);
    }
}