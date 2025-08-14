#pragma once
#include <exception>
#include <string>

class EmptyFieldException : public std::exception {
    std::string msg;
public:
    explicit EmptyFieldException(const char* message = "EmptyFieldInputGiven")
        : msg(message) {}

    const char* what() const noexcept override {
        return msg.c_str();
    }
};

