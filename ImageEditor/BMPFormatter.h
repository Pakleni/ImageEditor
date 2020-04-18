#pragma once
#include "Formatter.h"

class BMPFormatter :
	public Formatter
{
	static const char fileType[2];
	static const char zeros[4];
	static const char bitmapOffset[4];
	static const char dibSize[4];
	static const char carret[2];
	static const char bits[2];
	static const char pixPerM[4];
public:
	Formatter& operator<<(const Image& image);
	Formatter& operator>>(Image& image);
	Formatter& operator>>(Layer& layer);
	const Formatter& operator<<(const Image& image) const;
	const Formatter& operator>>(Image& image) const;
	const Formatter& operator>>(Layer& layer) const;

	BMPFormatter(const string&);
};

