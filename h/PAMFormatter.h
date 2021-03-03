#pragma once
#include "Formatter.h"
class PAMFormatter :
	public Formatter
{
public:
	PAMFormatter(const string& path);

	Formatter& operator<<(const Image& image);
	Formatter& operator>>(Image& image);
	Formatter& operator>>(Layer& layer);
	const Formatter& operator<<(const Image& image) const;
	const Formatter& operator>>(Image& image) const;
	const Formatter& operator>>(Layer& layer) const;
};

