#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "PhoneNumber.h"

class Contact
{
private:
    int m_id;
    std::string m_firstName;
    std::string m_lastName;
    std::string m_middleName;
    std::string m_address;
    std::string m_birthDate;
    std::string m_email;
    std::vector<PhoneNumber> m_phoneNumbers;

public:
    Contact(int id,
            const std::string &firstName,
            const std::string &lastName,
            const std::string &email,
            const std::vector<PhoneNumber> &phoneNumbers);
    int getId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getMiddleName() const;
    std::string getAddress() const;
    std::string getBirthDate() const;
    std::string getEmail() const;
    const std::vector<PhoneNumber> &getPhoneNumbers() const;

    void setFirstName(const std::string &firstName);
    void setLastName(const std::string &lastName);
    void setMiddleName(const std::string &middleName);
    void setAddress(const std::string &address);
    void setBirthDate(const std::string &birthDate);
    void setEmail(const std::string &email);

    // методы работы с номерами
    void addPhoneNumber(const PhoneNumber &phoneNumber);
    bool removePhoneNumber(std::size_t index);
    bool editPhoneNumber(std::size_t index, const PhoneNumber &phoneNumber);
};