#include "Pixel.h"

#include <math.h>
#include <cmath>
#include <algorithm>
#include <vector>

#include <boost/algorithm/string.hpp>

Pixel::Pixel(int red, int green, int blue, int alpha) : r(red), g(green), b(blue), a(alpha) {}

Pixel::Pixel() {}

void Pixel::abs() {
	r = std::abs(r);
	g = std::abs(g);
	b = std::abs(b);
}

void Pixel::power(double c) {
	r = std::pow(r, c);
	g = std::pow(g, c);
	b = std::pow(b, c);
}
Pixel operator+(const Pixel& a, int c){
	return Pixel(a.r + c, a.g + c, a.b + c, a.a);
}

Pixel operator+(int c, const Pixel& p) {
	return p + c;
}

Pixel operator-(const Pixel& a, int c){
	return Pixel(a.r - c, a.g - c, a.b - c, a.a);

}

Pixel operator-(int c, const Pixel& a) {
	return Pixel(c - a.r, c - a.g, c - a.b, a.a);
}

Pixel operator/(const Pixel& a, int c){
	if (c == 0) return Pixel(INFINITY, INFINITY, INFINITY,a.a);
	return Pixel(a.r / c, a.g / c, a.b / c, a.a);
}

Pixel operator/(int c, const Pixel& a) {
	return Pixel(a.r ? c / a.r : INFINITY, a.g ? c / a.g : INFINITY, a.b ? c / a.b : INFINITY, a.a);
}

Pixel operator*(const Pixel& a, int c){
	return Pixel(a.r * c, a.g * c, a.b * c, a.a);
}

void Pixel::round() {
	r = std::min(255, std::max(0, r));
	g = std::min(255, std::max(0, g));
	b = std::min(255, std::max(0, b));
}

Pixel operator*(int c, const Pixel& p) {
	return p * c;
}

void Pixel::log() {
	r = ::log(r);
	g = ::log(g);
	b = ::log(b);
}

void Pixel::min(int c){
	r = std::min(r, c);
	g = std::min(g, c);
	b = std::min(b, c);
}

void Pixel::max(int c){
	r = std::max(r, c);
	g = std::max(g, c);
	b = std::max(b, c);
	a = std::max(a, c);
}

void Pixel::invert() {
	r = 255 - r;
	g = 255 - g;
	b = 255 - b;
}

void Pixel::grayscale() {
	int mid = r + g + b;
	mid /= 3;
	r = g = b = mid;
}

void Pixel::blackwhite() {
	int mid = r + g + b;
	mid /= 3;
	r = g = b = mid < 127 ? 0 : 255;
}

int Pixel::getR() const { return r; }
int Pixel::getG() const { return g; }
int Pixel::getB() const { return b; }
int Pixel::getA() const { return a; }

void Pixel::stringify(std::string& str) const {
	
	str += '[';
	str += std::to_string(r);
	str += ',';
	str += std::to_string(g);
	str += ',';
	str += std::to_string(b);
	str += ',';
	str += std::to_string(a);
	str += ']';
}