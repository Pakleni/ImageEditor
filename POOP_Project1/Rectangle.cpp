#include "Rectangle.h"

Rectangle::Rectangle(unsigned int w, unsigned int h, unsigned int x_cord, unsigned int y_cord) : width(w), height(h), x(x_cord), y(y_cord) {}

std::string Rectangle::stringify() const {
	return "{\"w\":" + std::to_string(width) + ",\"h\":" + std::to_string(height) + ",\"x\":" + std::to_string(x) + ",\"y\":" + std::to_string(y) + "}";
}

unsigned int Rectangle::getWidth() const { return width; }
unsigned int Rectangle::getHeight() const {return height;}
unsigned int Rectangle::getX() const{return x;}
unsigned int Rectangle::getY() const{return y;}