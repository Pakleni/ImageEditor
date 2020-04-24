#include "PAMFormatter.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>
#include "InvalidFile.h"

const char PAM[] = "P7";
const char WIDTH[] = "WIDTH ";
const char HEIGHT[] = "HEIGHT ";
const char DEPTH[] = "DEPTH 4";
const char MAXVAL[] = "MAXVAL 255";
const char TYPE[] = "TUPLTYPE RGB_ALPHA";
const char END[] = "ENDHDR";
const char SEP[1] = { 0x0A };

using namespace std;

PAMFormatter::PAMFormatter(const string& str) : Formatter(str) {}
	
const Formatter& PAMFormatter::operator<<(const Image& img)const {
	ofstream file;

	file.open(path, ios_base::binary);

	if (!file.is_open()) throw InvalidFile();

	int w = img.getWidth();
	int h = img.getHeight();

	file.write(PAM, sizeof(PAM) -1);
	file.write(SEP, 1);
	file.write(WIDTH, sizeof(WIDTH) -1);
	file << w;
	file.write(SEP, 1);
	file.write(HEIGHT, sizeof(HEIGHT) -1);
	file << h;
	file.write(SEP, 1);
	file.write(DEPTH, sizeof(DEPTH) -1);
	file.write(SEP, 1);
	file.write(MAXVAL, sizeof(MAXVAL) - 1);
	file.write(SEP, 1);
	file.write(TYPE, sizeof(TYPE) - 1);
	file.write(SEP, 1);
	file.write(END, sizeof(END) - 1);
	file.write(SEP, 1);

	
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			Pixel curr = img.get(i, j);

			file << (unsigned char)curr.getR();
			file << (unsigned char)curr.getG();
			file << (unsigned char)curr.getB();
			file << (unsigned char)((int)round(255.0 / 100.0 * curr.getA()));
		}
	}

	file.close();

	return *this;
}

const Formatter& PAMFormatter::operator>>(Image& img)const {

	Layer layer;

	(*this) >> layer;

	img = Image(layer);

	return *this;
}

 const Formatter& PAMFormatter::operator>>(Layer& layer)const {
	ifstream file;

	file.open(path, ios_base::binary);

	if (!file.is_open()) throw InvalidFile();

	stringstream buffer;

	buffer << file.rdbuf();

	std::vector<std::string> results;

	boost::split(results, buffer.str(), [](char c) {return c == 0x0A; });


	int width = stoi(results[1].substr(6));
	int height = stoi(results[2].substr(7));
	string type = results[5].substr(9);

	results.erase(results.begin(), results.begin() + 7);

	string join_with = { 0x0A };

	stringstream pixels(boost::algorithm::join(results, join_with));

	layer = Layer(width, height);

	char* row = nullptr;
	if (type == "BLACKANDWHITE") {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				row = new char[2];
				pixels.get(row, 2);
				layer[i][j] = Pixel((uint8_t)row[0] << 8, (uint8_t)row[0] << 8, (uint8_t)row[0] << 8, 100);
			}
		}
	}
	else if (type == "GRAYSCALE") {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				row = new char[2];
				pixels.get(row, 2);
				layer[i][j] = Pixel((uint8_t)row[0], (uint8_t)row[0], (uint8_t)row[0], 100);
			}
		}
	}
	else if (type == "RGB") {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				row = new char[4];
				pixels.get(row, 4);
				layer[i][j] = Pixel((uint8_t)row[0], (uint8_t)row[1], (uint8_t)row[2], 100);
			}
		}
	}
	else if (type == "BLACKANDWHITE_ALPHA") {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				row = new char[3];
				pixels.get(row, 3);
				layer[i][j] = Pixel((uint8_t)row[0] << 8, (uint8_t)row[0] << 8, (uint8_t)row[0] << 8, round(100.0 / 255 * (uint8_t)row[1]));
			}
		}
	}
	else if (type == "GRAYSCALE_ALPHA") {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				row = new char[3];
				pixels.get(row, 3);
				layer[i][j] = Pixel((uint8_t)row[0], (uint8_t)row[0], (uint8_t)row[0], round(100.0 / 255 * (uint8_t)row[1]));
			}
		}
	}
	else if (type == "RGB_ALPHA") {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				row = new char[5];
				pixels.get(row, 5);
				layer[i][j] = Pixel((uint8_t)row[0], (uint8_t)row[1], (uint8_t)row[2], round(100.0 / 255 * (uint8_t)row[3]));
			}
		}
	}
	delete[] row;

	file.close();
}

Formatter& PAMFormatter::operator<<(const Image& img) {
	*(const_cast<const PAMFormatter*>(this)) << img;
	return *this;
}

Formatter& PAMFormatter::operator>>(Image& img) {
	*(const_cast<const PAMFormatter*>(this)) >> img;
	return *this;
}

Formatter& PAMFormatter::operator>>(Layer& layer) {
	*(const_cast<const PAMFormatter*>(this)) >> layer;
	return *this;
}
