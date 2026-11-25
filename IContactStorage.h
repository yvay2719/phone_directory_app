#pragma once

#include <vector>
#include "Contact.h"

class IContactStorage
{
public:
    virtual ~IContactStorage() = default;

    virtual void save(const std::vector<Contact> &contacts) = 0;
    virtual std::vector<Contact> load() = 0;
};