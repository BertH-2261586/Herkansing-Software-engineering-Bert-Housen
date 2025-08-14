#pragma once
#include <exception>
#include <string>

class NoSavedSessionException : public std::exception {
    std::string msg;
public:
    explicit NoSavedSessionException(const char* message = "EmptyFieldInputGiven")
        : msg(message) {}

    const char* what() const noexcept override {
        return msg.c_str();
    }
};
