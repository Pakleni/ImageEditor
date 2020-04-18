#pragma once
#include "Formatter.h"
class MyFormatter :
	public Formatter
{
public:
	Formatter& operator<<(const Image& image);
	Formatter& operator>>(Image& image);
	const Formatter& operator<<(const Image& image) const;
	const Formatter& operator>>(Image& image) const;

	MyFormatter(const string&);
};

