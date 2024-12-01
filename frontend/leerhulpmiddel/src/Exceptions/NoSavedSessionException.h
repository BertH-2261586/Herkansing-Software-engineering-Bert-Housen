#pragma once
#include <exception>


class NoSavedSessionException : public std::exception {
public:
	NoSavedSessionException(const char* message = "No saved session could be retrieved from device!") : std::exception(message) {}
};