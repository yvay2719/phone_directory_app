cmake_minimum_required(VERSION 3.16)

project(PhoneDirectoryApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core library
add_library(phonebook_core STATIC
    src/Contact.cpp
    src/PhoneNumber.cpp
    src/Validator.cpp
    src/FileStorage.cpp
    src/PhoneBook.cpp
)

target_include_directories(phonebook_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/HeaderFiles
)

# Console application
add_executable(phonebook
    src/main.cpp
    src/ConsoleUI.cpp
)

target_link_libraries(phonebook PRIVATE phonebook_core)

# Tests
enable_testing()

add_executable(contact_tests tests/ContactTests.cpp)
target_link_libraries(contact_tests PRIVATE phonebook_core)
add_test(NAME ContactTests COMMAND contact_tests)

add_executable(validator_tests tests/ValidatorTests.cpp)
target_link_libraries(validator_tests PRIVATE phonebook_core)
add_test(NAME ValidatorTests COMMAND validator_tests)

add_executable(file_storage_tests tests/FileStorageTests.cpp)
target_link_libraries(file_storage_tests PRIVATE phonebook_core)
add_test(NAME FileStorageTests COMMAND file_storage_tests)

add_executable(phonebook_tests tests/PhoneBookTests.cpp)
target_link_libraries(phonebook_tests PRIVATE phonebook_core)
add_test(NAME PhoneBookTests COMMAND phonebook_tests)