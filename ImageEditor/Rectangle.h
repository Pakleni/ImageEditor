#pragma once
#include <string>

class Selection;

class Rectangle
{
	unsigned int width, height, x, y;
public:
	Rectangle(unsigned int width, unsigned int height, unsigned int x, unsigned int y);

	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getX() const;
	unsigned int getY() const;

	std::string stringify() const;

};


