#pragma once
#include <string>
#include <vector>
#include <set>
#include "Rectangle.h"

using namespace std;

class Selection
{
	string name = "";
	vector<Rectangle> rectangles;
public:

	Selection();
	Selection(const string& name, const vector<Rectangle>& rectangles);
	const string& getName() const;
	string stringify() const;
	void parse(const string&);

	const vector<Rectangle>& getRectangles() const;
};
