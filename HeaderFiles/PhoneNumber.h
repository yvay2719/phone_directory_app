#pragma once
#include <string>
class PhoneNumber
{
public:
    enum class Type
    {
        Work,
        Home,
        Service
    };

    PhoneNumber(const std::string &number, Type type);

    std::string getNumber() const;
    Type getType() const;

    void setNumber(const std::string &number);
    void setType(Type type);

private:
    std::string m_number;
    Type m_type;
};