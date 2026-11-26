#pragma once

#include <string>
#include "Contact.h"

class Validator
{
public:
    static bool isValidName(const std::string &name);
    static bool isValidPhone(const std::string &phone);
    static bool isValidEmail(const std::string &email);
    static bool isValidBirthDate(const std::string &birthDate);
    static bool isValidContact(const Contact &contact);
    static void normalizeContact(Contact &contact);
    static std::string trim(const std::string &value);
};