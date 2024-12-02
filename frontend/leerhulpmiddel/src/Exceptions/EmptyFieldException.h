#pragma once
#include <exception>

class EmptyFieldException : public std::exception
{
	public:
    EmptyFieldException(const char* message = "EmptyFieldInputGiven") : std::exception() {}
};

