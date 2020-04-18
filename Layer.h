#pragma once
#include <string>
#include "Pixel.h"
#include <vector>
#include "Selection.h"
using namespace std;
class Layer
{
	unsigned int width = 0, height = 0;
	vector<vector<Pixel>> pixels = vector<vector<Pixel>>(height, vector<Pixel>(width, Pixel()));
	int alpha = 100;
	bool active = 1, visible = 1;

public:
	string stringify() const;
	Layer(unsigned int width, unsigned int height);
	Layer();
	~Layer();

	void parse(const string&);

	void expand(unsigned int new_width, unsigned int new_height);
	vector<Pixel>& operator[](unsigned int index);
	const vector<Pixel>& operator[](unsigned int index) const;

	unsigned int getHeight() const;
	unsigned int getWidth() const;

	bool isVisible() const;
	void setVisible(bool);
	bool isActive() const;
	void setActive(bool);

};