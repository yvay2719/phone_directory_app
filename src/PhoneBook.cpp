#include "../HeaderFiles/PhoneBook.h"
#include "../HeaderFiles/Validator.h"
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iterator>

PhoneBook::PhoneBook(std::unique_ptr<IContactStorage> storage) : m_storage(std::move(storage)), m_nextId(1)
{
    if (!m_storage)
    {
        throw std::invalid_argument("Storage is null");
    }

    m_contacts = m_storage->load();
    for (const auto &contact : m_contacts)
    {
        if (contact.getId() >= m_nextId)
        {
            m_nextId = contact.getId() + 1;
        }
    }
}
const std::vector<Contact> &PhoneBook::getContacts() const
{
    return m_contacts;
}

bool PhoneBook::addContact(
    const std::string &firstName,
    const std::string &lastName,
    const std::string &email,
    const std::vector<PhoneNumber> &phoneNumbers,
    const std::string &middleName,
    const std::string &address,
    const std::string &birthDate)
{
    Contact contact(
        m_nextId,
        firstName,
        lastName,
        email,
        phoneNumbers);

    contact.setMiddleName(middleName);
    contact.setAddress(address);
    contact.setBirthDate(birthDate);
    Validator::normalizeContact(contact);

    if (!Validator::isValidContact(contact))
    {
        return false;
    }

    std::vector<Contact> updatedContacts = m_contacts;
    updatedContacts.push_back(contact);
    m_storage->save(updatedContacts);
    m_contacts = std::move(updatedContacts);
    ++m_nextId;
    return true;
}

bool PhoneBook::removeContact(int id)
{
    auto it = std::find_if(
        m_contacts.begin(),
        m_contacts.end(),
        [id](const Contact &contact)
        {
            return contact.getId() == id;
        });
    if (it == m_contacts.end())
    {
        return false;
    }
    std::vector<Contact> updatedContacts = m_contacts;
    auto index = std::distance(m_contacts.begin(), it);
    updatedContacts.erase(updatedContacts.begin() + index);
    m_storage->save(updatedContacts);
    m_contacts = std::move(updatedContacts);
    return true;
}

bool PhoneBook::editContact(
    int id,
    const std::string &firstName,
    const std::string &lastName,
    const std::string &email,
    const std::vector<PhoneNumber> &phoneNumbers,
    const std::string &middleName,
    const std::string &address,
    const std::string &birthDate)
{
    auto it = std::find_if(
        m_contacts.begin(),
        m_contacts.end(),
        [id](const Contact &contact)
        {
            return contact.getId() == id;
        });
    if (it == m_contacts.end())
    {
        return false;
    }

    Contact updatedContact(
        id,
        firstName,
        lastName,
        email,
        phoneNumbers);

    updatedContact.setMiddleName(middleName);
    updatedContact.setAddress(address);
    updatedContact.setBirthDate(birthDate);
    Validator::normalizeContact(updatedContact);
    if (!Validator::isValidContact(updatedContact))
    {
        return false;
    }

    std::vector<Contact> updatedContacts = m_contacts;
    auto index = std::distance(m_contacts.begin(), it);
    updatedContacts[index] = updatedContact;
    m_storage->save(updatedContacts);
    m_contacts = std::move(updatedContacts);
    return true;
}
std::vector<Contact> PhoneBook::searchContacts(
    const std::string &query) const
{
    std::vector<Contact> results;

    std::string cleanedQuery = Validator::trim(query);

    for (const auto &contact : m_contacts)
    {
        if (
            contact.getFirstName().find(cleanedQuery) != std::string::npos ||
            contact.getLastName().find(cleanedQuery) != std::string::npos ||
            contact.getMiddleName().find(cleanedQuery) != std::string::npos ||
            contact.getAddress().find(cleanedQuery) != std::string::npos ||
            contact.getBirthDate().find(cleanedQuery) != std::string::npos ||
            contact.getEmail().find(cleanedQuery) != std::string::npos)
        {
            results.push_back(contact);
            continue;
        }

        for (const auto &phone : contact.getPhoneNumbers())
        {
            if (phone.getNumber().find(cleanedQuery) != std::string::npos)
            {
                results.push_back(contact);
                break;
            }
        }
    }

    return results;
}

void PhoneBook::sortContacts(SortField field, bool ascending)
{
    std::sort(
        m_contacts.begin(),
        m_contacts.end(),
        [field, ascending](const Contact &a, const Contact &b)
        {
            switch (field)
            {
            case SortField::Id:
                return ascending
                           ? a.getId() < b.getId()
                           : a.getId() > b.getId();
            case SortField::FirstName:
                return ascending
                           ? a.getFirstName() < b.getFirstName()
                           : a.getFirstName() > b.getFirstName();

            case SortField::LastName:
                return ascending
                           ? a.getLastName() < b.getLastName()
                           : a.getLastName() > b.getLastName();

            case SortField::MiddleName:
                return ascending
                           ? a.getMiddleName() < b.getMiddleName()
                           : a.getMiddleName() > b.getMiddleName();

            case SortField::Address:
                return ascending
                           ? a.getAddress() < b.getAddress()
                           : a.getAddress() > b.getAddress();

            case SortField::Email:
                return ascending
                           ? a.getEmail() < b.getEmail()
                           : a.getEmail() > b.getEmail();
            case SortField::BirthDate:
            {
                auto dateKey = [](const std::string &date)
                {
                    if (date.size() != 10)
                    {
                        return std::string("");
                    }

                    return date.substr(6, 4) +
                           date.substr(3, 2) +
                           date.substr(0, 2);
                };

                return ascending
                           ? dateKey(a.getBirthDate()) < dateKey(b.getBirthDate())
                           : dateKey(a.getBirthDate()) > dateKey(b.getBirthDate());
            }
            case SortField::PhoneNumber:
            {
                auto phoneKey = [](const Contact &contact)
                {
                    if (contact.getPhoneNumbers().empty())
                    {
                        return std::string("");
                    }

                    return contact.getPhoneNumbers()[0].getNumber();
                };

                return ascending
                           ? phoneKey(a) < phoneKey(b)
                           : phoneKey(a) > phoneKey(b);
            }
            default:
                return false;
            }
        });
}