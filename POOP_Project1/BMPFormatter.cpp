#include "BMPFormatter.h"
#include <fstream>
#include <intrin.h>
#include <iostream>
#include "InvalidFile.h"

const char BMPFormatter::fileType[2] = { 'B', 'M' };
const char BMPFormatter::zeros[4] = { 0x00, 0x00, 0x00, 0x00 };
const char BMPFormatter::bitmapOffset[4] = { 0x36, 0x00, 0x00, 0x00 };
const char BMPFormatter::dibSize[4] = { 0x28, 0x00, 0x00, 0x00 };
const char BMPFormatter::carret[2] = { 0x01, 0x00 };
const char BMPFormatter::bits[2] = { 0x20, 0x00 };
const char BMPFormatter::pixPerM[4] = { 0x13, 0x0B, 0x00, 0x00 };

uint32_t cptoul(char arr[]) {
	uint32_t res = 0;
	int r = 0;
	for (int i = 0; i < 4 ; i++) {
		res += (int)arr[i] << r;
		r += 8;
	}

	return res;
}

char * ultocp(uint32_t num) {
	char *res = new char[4];
	unsigned int r = 0;
	for (int i = 0; i < 4; i++) {
		res[i] = (int)((num >> r) & 0xFF);
		r += 8;
	}

	return res;
}

const Formatter& BMPFormatter::operator<<(const Image& img) const{

	ofstream file;

	file.open(path, ios_base::binary);

	if (!file.is_open()) throw InvalidFile();
	
	uint32_t w = img.getWidth();
	uint32_t h = img.getHeight();

	int row_width = 4 * w;

	const char* width = ultocp(w);
	const char* height = ultocp(h);
	const char* fileSize = ultocp(54 + row_width * h);
	const char * bitmapSize = ultocp(row_width * h);
	
	file.write(fileType, 2);
	file.write(fileSize, 4);
	file.write(zeros, 4);
	file.write(bitmapOffset, 4);
	file.write(dibSize, 4);
	file.write(width, 4);
	file.write(height, 4);
	file.write(carret, 2);
	file.write(bits, 2);
	file.write(zeros, 4);
	file.write(bitmapSize, 4);
	file.write(pixPerM, 4);
	file.write(pixPerM, 4);
	file.write(zeros, 4);
	file.write(zeros, 4);

	delete[] fileSize;
	delete[] width;
	delete[] height;
	delete[] bitmapSize;

	char* row = new char[row_width];
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			Pixel curr = img.get(i, j);
			row[4 * j] = (char)curr.getB();
			row[4 * j + 1] = (char)curr.getG();
			row[4 * j + 2] = (char)curr.getR();
			row[4 * j + 3] = (char)round(255.0 / 100.0 * curr.getA());
		}
		file.write(row, row_width);
	}
	delete[] row;

	file.close();
	
	return *this;
}

const Formatter& BMPFormatter::operator>>(Image& img) const {

	Layer layer;

	(*this) >> layer;

	img = Image(layer);

	return *this;
}

const Formatter& BMPFormatter::operator>>(Layer& layer) const {
	ifstream file;

	file.open(path, ios_base::binary);

	if (!file.is_open()) throw InvalidFile();

	char header[55];

	file.get(header, 55);

	uint32_t w = cptoul(header + 0x12);
	uint32_t h = cptoul(header + 0x16);

	layer = Layer(w, h);

	unsigned int row_width = 4 * w + 1;

	char* row = new char[row_width];

	for (int i = 0; i < h; i++) {
		file.get(row, row_width);
		for (int j = 0; j < w; j++) {
			layer[i][j] = Pixel((uint8_t)row[4 * j + 2], (uint8_t)row[4 * j + 1], (uint8_t)row[4 * j], round(100.0 / 255 * (uint8_t)row[4 * j + 3]));
		}
	}
	delete[] row;

	file.close();

	return *this;
}

BMPFormatter::BMPFormatter(const string& str): Formatter(str) {}

Formatter& BMPFormatter::operator<<(const Image& img) {
	*(const_cast<const BMPFormatter*>(this)) << img;
	return *this;
}

Formatter& BMPFormatter::operator>>(Image& img) {
	*(const_cast<const BMPFormatter*>(this)) >> img;
	return *this;
}

Formatter& BMPFormatter::operator>>(Layer& layer) {
	*(const_cast<const BMPFormatter*>(this)) >> layer;
	return *this;
}