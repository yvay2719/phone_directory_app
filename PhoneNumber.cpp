#include "PhoneNumber.h"
// конструктор
PhoneNumber::PhoneNumber(const std::string &number, Type type)
    : m_number(number), m_type(type)
{
}

// геттеры
std::string PhoneNumber::getNumber() const
{
    return m_number;
}

PhoneNumber::Type PhoneNumber::getType() const
{
    return m_type;
}

void PhoneNumber::setNumber(const std::string &number)
{
    m_number = number;
}

void PhoneNumber::setType(Type type)
{
    m_type = type;
}
