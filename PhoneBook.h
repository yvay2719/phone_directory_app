#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Contact.h"
#include "IContactStorage.h"

class PhoneBook
{
private:
    std::vector<Contact> m_contacts;
    std::unique_ptr<IContactStorage> m_storage;
    int m_nextId;

public:
    enum class SortField
    {
        Id,
        FirstName,
        LastName,
        MiddleName,
        Address,
        BirthDate,
        Email,
        PhoneNumber
    };

    explicit PhoneBook(std::unique_ptr<IContactStorage> storage);
    const std::vector<Contact> &getContacts() const;
    bool addContact(
        const std::string &firstName,
        const std::string &lastName,
        const std::string &email,
        const std::vector<PhoneNumber> &phoneNumbers,
        const std::string &middleName = "",
        const std::string &address = "",
        const std::string &birthDate = "");
    bool removeContact(int id);
    bool editContact(
        int id,
        const std::string &firstName,
        const std::string &lastName,
        const std::string &email,
        const std::vector<PhoneNumber> &phoneNumbers,
        const std::string &middleName,
        const std::string &address,
        const std::string &birthDate);
    std::vector<Contact> searchContacts(const std::string &query) const;
    void sortContacts(SortField field, bool ascending = true);
};