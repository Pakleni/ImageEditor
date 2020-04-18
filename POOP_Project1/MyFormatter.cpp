#include "MyFormatter.h"
#include <fstream>
#include "InvalidFile.h"

MyFormatter::MyFormatter(const string& _path): Formatter(_path) {}

Formatter& MyFormatter::operator<<(const Image& img) {
	ofstream file(path);

	if (!file.is_open()) throw InvalidFile();

	file << img.stringify();

	file.close();

	return (*this);
}

Formatter& MyFormatter::operator>>(Image& img) {
	img.load(path);

	return (*this);
}

const Formatter& MyFormatter::operator<<(const Image& img) const {
	*(const_cast<const MyFormatter*>(this)) << img;
	return (*this);
}

const Formatter& MyFormatter::operator>>(Image& img) const {
	*(const_cast<const MyFormatter*>(this)) >> img;
	return (*this);
}