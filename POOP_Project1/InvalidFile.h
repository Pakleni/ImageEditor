#pragma once
#include <exception>

class InvalidFile : public std::exception {
public:
	const char* what() const throw () {
		return "Invalid file data or doesn't exist on specified path";
	}
};

