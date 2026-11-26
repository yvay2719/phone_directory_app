#pragma once

#include <string>
#include "IContactStorage.h"

class FileStorage : public IContactStorage
{
public:
    FileStorage(const std::string &filePath);

    void save(const std::vector<Contact> &contacts) override;
    std::vector<Contact> load() override;

private:
    std::string m_filePath;
};