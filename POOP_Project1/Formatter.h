#pragma once
#include <string>
#include "Image.h"

using namespace std;

class Formatter
{
protected:
	string path;

public:
	Formatter(const string& path);

	virtual Formatter& operator<<(const Image& image)= 0;
	virtual Formatter& operator>>(Image& image) = 0;

	virtual const Formatter& operator<<(const Image& image) const = 0;
	virtual const Formatter& operator>>(Image& image) const = 0;

};

