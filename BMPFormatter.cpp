#include "BMPFormatter.h"
#include <fstream>
#include <intrin.h>
#include <iostream>
#include "InvalidFile.h"

const char BMPFormatter::fileType[2] = { 'B', 'M' };
const char BMPFormatter::zeros[4] = { 0x00, 0x00, 0x00, 0x00 };
const char BMPFormatter::bitmapOffset[4] = { 0x7a, 0x00, 0x00, 0x00 };
const char BMPFormatter::dibSize[4] = { 0x6c, 0x00, 0x00, 0x00 };
const char BMPFormatter::carret[2] = { 0x01, 0x00 };
const char BMPFormatter::compression[4] = { 0x03, 0x00, 0x00, 0x00 };
const char BMPFormatter::bits[2] = { 0x20, 0x00 };
const char BMPFormatter::pixPerM[4] = { 0x13, 0x0B, 0x00, 0x00 };
const char BMPFormatter::Red[4] = { 0x00, 0x00, 0xff, 0x00 };
const char BMPFormatter::Green[4] = { 0x00, 0xff, 0x00, 0x00 };
const char BMPFormatter::Blue[4] = { 0xff, 0x00, 0x00, 0x00 };
const char BMPFormatter::Alpha[4] = { 0x00, 0x00, 0x00, 0xff };
const char BMPFormatter::ColorSpace[4] = { 0x73, 0x52 ,0x47, 0x42 };



uint32_t cptoul(char arr[]) {
	uint32_t res = 0;
	int r = 0;
	for (int i = 0; i < 4 ; i++) {
		res += (uint8_t)arr[i] << r;
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
	const char* fileSize = ultocp(122 + row_width * h);
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
	file.write(compression, 4);
	file.write(bitmapSize, 4);
	file.write(pixPerM, 4);
	file.write(pixPerM, 4);
	file.write(zeros, 4);
	file.write(zeros, 4);
	file.write(Red, 4);
	file.write(Green, 4);
	file.write(Blue, 4);
	file.write(Alpha, 4);
	file.write(ColorSpace, 4);

	for (int i = 0; i < 12; i++) {
		file.write(zeros, 4);
	}

	delete[] fileSize;
	delete[] width;
	delete[] height;
	delete[] bitmapSize;

	char* row = new char[row_width];
	for (int i = h - 1; i >= 0; --i) {
		for (int j = w - 1; j >= 0; --j) {
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

	char width_f[4];
	char height_f[4];

	file.seekg(file.beg + 0x12);
	file.read(width_f, 4);
	file.read(height_f, 4);

	uint32_t w = cptoul(width_f);
	uint32_t h = cptoul(height_f);

	file.seekg(file.beg + 0xA);

	char offset_f[4];
	file.read(offset_f, 4);
	uint32_t offset = cptoul(offset_f);

	file.seekg(file.beg + offset);

	layer = Layer(w, h);

	char pixel_f[4];

	for (int i = h - 1; i >= 0; --i) {
		for (int j = 0; j < w; ++j) {
			file.read(pixel_f, 4);
			layer[i][j] = Pixel((uint8_t)pixel_f[2], (uint8_t)pixel_f[1], (uint8_t)pixel_f[0], round(100.0 / 255 * (uint8_t)pixel_f[3]));
		}
	}

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