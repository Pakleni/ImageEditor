	#pragma once
#include <string>

class Layer;

class Pixel
{
	int r = 0, g = 0, b = 0, a = 0;

public:
	Pixel();
	Pixel(int red, int green, int blue, int alpha);
	friend Pixel operator+(const Pixel& pixel, int constant);
	friend Pixel operator-(const Pixel& pixel, int constant);
	friend Pixel operator-(int constant, const Pixel& pixel);
	friend Pixel operator/(const Pixel& pixel, int constant);
	friend Pixel operator/(int constant, const Pixel& pixel);
	friend Pixel operator*(const Pixel& pixel, int constant);

	int getR() const;
	int getG() const;
	int getB() const;
	int getA() const;


	void round();

	void log();
	void abs();
	void min(int constant);
	void max(int constant);
	void invert();
	void power(double constant);
	void grayscale();
	void blackwhite();

	friend class Layer;
	
	void stringify(std::string &) const;
};

Pixel operator+(int constant, const Pixel& pixel);
Pixel operator*(int constant, const Pixel& pixel);