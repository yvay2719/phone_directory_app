#include "../HeaderFiles/Contact.h"
// конструкторы
Contact::Contact(int id,
                 const std::string &firstName,
                 const std::string &lastName,
                 const std::string &email,
                 const std::vector<PhoneNumber> &phoneNumbers)
    : m_id(id),
      m_firstName(firstName),
      m_lastName(lastName),
      m_email(email),
      m_phoneNumbers(phoneNumbers)
{

    // getter
}
int Contact::getId() const
{
    return m_id;
}

std::string Contact::getFirstName() const
{
    return m_firstName;
}

std::string Contact::getLastName() const
{
    return m_lastName;
}

std::string Contact::getMiddleName() const
{
    return m_middleName;
}

std::string Contact::getAddress() const
{
    return m_address;
}

std::string Contact::getBirthDate() const
{
    return m_birthDate;
}

std::string Contact::getEmail() const
{
    return m_email;
}

const std::vector<PhoneNumber> &Contact::getPhoneNumbers() const
{
    return m_phoneNumbers;
}

// functions
void Contact::setFirstName(const std::string &firstName)
{
    m_firstName = firstName;
}

void Contact::setLastName(const std::string &lastName)
{
    m_lastName = lastName;
}

void Contact::setMiddleName(const std::string &middleName)
{
    m_middleName = middleName;
}

void Contact::setAddress(const std::string &address)
{
    m_address = address;
}

void Contact::setBirthDate(const std::string &birthDate)
{
    m_birthDate = birthDate;
}

void Contact::setEmail(const std::string &email)
{
    m_email = email;
}

// методы работы с номерами
void Contact::addPhoneNumber(const PhoneNumber &phoneNumber)
{
    m_phoneNumbers.push_back(phoneNumber);
}

bool Contact::removePhoneNumber(std::size_t index)
{
    if (index >= m_phoneNumbers.size() ||
        m_phoneNumbers.size() <= 1)
    {
        return false;
    }

    m_phoneNumbers.erase(m_phoneNumbers.begin() + index);
    return true;
}

bool Contact::editPhoneNumber(std::size_t index,
                              const PhoneNumber &phoneNumber)
{
    if (index >= m_phoneNumbers.size())
    {
        return false;
    }

    m_phoneNumbers[index] = phoneNumber;
    return true;
}