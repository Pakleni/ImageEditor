#include "BasicOperation.h"
#include "Image.h"
#include <algorithm>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

Add::Add() : Operation("add") {};
Add::Add(int _c) : Operation("add"), c(_c) {};

void Add::run_noround(Image& img) const {
	for (auto& pix : img) { pix = pix + c; }
}
void Add::parse(const string & str) {
	c = stoi(str);
}
string Add::getC() const {
	return to_string(c) ;
}

Subtract::Subtract() : Operation("sub") {};
Subtract::Subtract(int _c) : Operation("sub"), c(_c) {};

void Subtract::run_noround(Image& img) const {
	for (auto& pix : img) { pix = pix - c; }
}
void Subtract::parse(const string & str) {
	c = stoi(str);
}
string Subtract::getC() const {
	return to_string(c) ;
}

ReverseSubtract::ReverseSubtract() : Operation("rsb") {};
ReverseSubtract::ReverseSubtract(int _c) : Operation("rsb"), c(_c) {};

void ReverseSubtract::run_noround(Image& img) const {
	for (auto& pix : img) { pix = c - pix; }
}
void ReverseSubtract::parse(const string & str) {
	c = stoi(str);
}
string ReverseSubtract::getC() const {
	return to_string(c) ;
}


Multiply::Multiply() : Operation("mul") {};
Multiply::Multiply(int _c) : Operation("mul"), c(_c) {};

void Multiply::run_noround(Image& img) const {
	for (auto& pix : img) { pix = pix * c; }
}
void Multiply::parse(const string & str) {
	c = stoi(str);
}
string Multiply::getC() const {
	return to_string(c) ;
}


Divide::Divide() : Operation("div") {};
Divide::Divide(int _c) : Operation("div"), c(_c) {};

void Divide::run_noround(Image& img) const {
	for (auto& pix : img) { pix = pix / c; }
}
void Divide::parse(const string & str) {
	c = stoi(str);
}
string Divide::getC() const {
	return to_string(c) ;
}


ReverseDivide::ReverseDivide() : Operation("div") {};
ReverseDivide::ReverseDivide(int _c) : Operation("div"), c(_c) {};

void ReverseDivide::run_noround(Image& img) const {
	for (auto& pix : img) { pix = pix / c; }
}
void ReverseDivide::parse(const string & str) {
	c = stoi(str);
}
string ReverseDivide::getC() const {
	return to_string(c) ;
}


Power::Power() : Operation("pow") {};
Power::Power(int _c) : Operation("pow"), c(_c) {};

void Power::run_noround(Image& img) const {
	for (auto& pix : img) { pix.power(c); }
}
void Power::parse(const string & str) {
	c = stod(str);
}
string Power::getC() const {
	return to_string(c) ;
}


Logarithm::Logarithm() : Operation("log") {};
Logarithm::Logarithm(int _c) : Operation("log"), c(_c) {};

void Logarithm::run_noround(Image& img) const {
	for (auto& pix : img) { pix.log(c); }
}
void Logarithm::parse(const string & str) {
	c = stod(str);
}
string Logarithm::getC() const {
	return to_string(c) ;
}


Absolute::Absolute() : Operation("abs") {};

void Absolute::run_noround(Image& img) const {
	for (auto& pix : img) { pix.abs(); }
}


Minimum::Minimum() : Operation("min") {};
Minimum::Minimum(int _c) : Operation("min"), c(_c) {};

void Minimum::run_noround(Image& img) const {
	for (auto& pix : img) { pix.min(c); }
}
void Minimum::parse(const string & str) {
	c = stoi(str);
}
string Minimum::getC() const {
	return to_string(c) ;
}

Maximum::Maximum() : Operation("max") {};
Maximum::Maximum(int _c) : Operation("max"), c(_c) {};

void Maximum::run_noround(Image& img) const {
	for (auto& pix : img) { pix.max(c); }
}
void Maximum::parse(const string & str) {
	c = stoi(str);
}
string Maximum::getC() const {
	return to_string(c) ;
}


Invert::Invert() : Operation("inv") {};

void Invert::run_noround(Image& img) const {
	for (auto& pix : img) { pix.invert(); }
}

BlackWhite::BlackWhite() : Operation("b&w") {};

void BlackWhite::run_noround(Image& img) const {
	for (auto& pix : img) { pix.blackwhite(); }
}

Grayscale::Grayscale() : Operation("gra") {};

void Grayscale::run_noround(Image& img) const {
	for (auto& pix : img) { pix.grayscale(); }
}

Median::Median() : Operation("med") {};

void Median::run_noround(Image& img) const {
	Image temp = img;
	vector <Pixel> medians;
	for (auto pix = temp.begin(); pix != temp.end(); ++pix) {
		medians.push_back(pix.median());
	};
	int i = 0;
	for (auto& pix : img) { pix = medians[i++]; }
}


Fill::Fill() : Operation("fill") {};
Fill::Fill(int r, int g, int b, int a) : Operation("fill"), c(Pixel(r, g, b, a)) {};

void Fill::run_noround(Image& img) const {
	for (auto& pix : img) { pix = c; }
}
void Fill::parse(const string & str) {
	vector<string> results;
	boost::split(results, str, [](char c) {return c == ','; });
	int r = stoi(results[0]);
	int g = stoi(results[1]);
	int b = stoi(results[2]);
	int a = stoi(results[3]);
	c = Pixel(r, g, b, a);
}
string Fill::getC() const {
	return to_string(c.getR()) + ',' + to_string(c.getG()) + ',' + to_string(c.getB()) + ',' + to_string(c.getA()) ;
}

Operation* Add::clone() {
	return new Add(*this);
}
Operation* Subtract::clone() {
	return new Subtract(*this);
}
Operation* ReverseSubtract::clone(){
	return new ReverseSubtract(*this);
}
Operation*Multiply::clone(){
	return new Multiply(*this);
}
Operation*Divide::clone(){
	return new Divide(*this);
}
Operation*ReverseDivide::clone(){
	return new ReverseDivide(*this);
}
Operation*Power::clone(){
	return new Power(*this);
}
Operation*Logarithm::clone(){
	return new Logarithm(*this);
}
Operation*Absolute::clone(){
	return new Absolute(*this);
}
Operation*Minimum::clone(){
	return new Minimum(*this);
}
Operation*Maximum::clone(){
	return new Maximum(*this);
}
Operation*Invert::clone(){
	return new Invert(*this);
}
Operation*BlackWhite::clone(){
	return new BlackWhite(*this);
}
Operation*Grayscale::clone(){
	return new Grayscale(*this);
}
Operation*Median::clone(){
	return new Median(*this);
}
Operation*Fill::clone(){
	return new Fill(*this);
}