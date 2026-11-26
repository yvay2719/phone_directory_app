#include "FileStorage.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <filesystem>

FileStorage::FileStorage(const std::string &filePath) : m_filePath(filePath) {}

void FileStorage::save(const std::vector<Contact> &contacts)
{
    std::string tempPath = m_filePath + ".tmp";
    std::ofstream file(tempPath);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + tempPath);
    }

    file << contacts.size() << '\n';
    for (const auto &contact : contacts)
    {
        file << contact.getId() << '\n';
        file << std::quoted(contact.getFirstName()) << '\n';
        file << std::quoted(contact.getLastName()) << '\n';
        file << std::quoted(contact.getMiddleName()) << '\n';
        file << std::quoted(contact.getAddress()) << '\n';
        file << std::quoted(contact.getBirthDate()) << '\n';
        file << std::quoted(contact.getEmail()) << '\n';
        const auto &phones = contact.getPhoneNumbers();

        file << phones.size() << '\n';

        for (const auto &phone : phones)
        {
            file << std::quoted(phone.getNumber()) << ' ' << static_cast<int>(phone.getType()) << '\n';
        }
    }
    file.close();

    if (file.fail())
    {
        std::filesystem::remove(tempPath);

        throw std::runtime_error("Cannot write to file: " + tempPath);
    }

    std::filesystem::rename(tempPath, m_filePath);

    if (file.fail())
    {
        throw std::runtime_error("Cannot write to file: " + m_filePath);
    }
}

std::vector<Contact> FileStorage::load()
{
    std::vector<Contact> contacts;

    std::ifstream file(m_filePath);

    if (!file.is_open())
    {
        if (!std::filesystem::exists(m_filePath))
        {
            return contacts;
        }

        throw std::runtime_error(
            "Cannot open file: " + m_filePath);
    }

    std::size_t contactCount = 0;

    if (!(file >> contactCount))
    {
        throw std::runtime_error("Cannot read contact count");
    }
    for (std::size_t i = 0; i < contactCount; ++i)
    {
        int id = 0;
        std::string firstName;
        std::string lastName;
        std::string middleName;
        std::string address;
        std::string birthDate;
        std::string email;

        if (!(file >> id >> std::quoted(firstName) >> std::quoted(lastName) >> std::quoted(middleName) >> std::quoted(address) >> std::quoted(birthDate) >> std::quoted(email)))
        {
            throw std::runtime_error("Cannot read contact fields");
        }

        std::size_t phoneCount = 0;

        if (!(file >> phoneCount))
        {
            throw std::runtime_error("Cannot read phone count");
        }

        std::vector<PhoneNumber> phones;

        for (std::size_t j = 0; j < phoneCount; ++j)
        {
            std::string number;
            int type = -1;

            if (!(file >> std::quoted(number) >> type))
            {
                throw std::runtime_error("Cannot read phone number");
            }

            if (type < 0 || type > 2)
            {
                throw std::runtime_error("Invalid phone type");
            }

            phones.emplace_back(
                number, static_cast<PhoneNumber::Type>(type));
        }
        Contact contact(id, firstName, lastName, email, phones);
        contact.setMiddleName(middleName);
        contact.setAddress(address);
        contact.setBirthDate(birthDate);
        contacts.push_back(contact);
    }
    return contacts;
}