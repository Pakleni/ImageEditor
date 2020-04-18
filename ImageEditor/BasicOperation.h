#pragma once
#include "Operation.h"
#include <string>
#include "Image.h"

#include<functional>

using namespace std;

class CompositeOperation;

class BasicOperation :
	public Operation
{
	function<void(Image & image)> f;
protected:
	void run_noround(Image& image) const;
public:
	BasicOperation(const string & name, function<void(Image &)> function);
	void run(Image & image) const;

	string getName() const;
	string stringify() const;

	friend CompositeOperation;
};

extern BasicOperation add;
extern BasicOperation subtract;
extern BasicOperation reverse_subtract;
extern BasicOperation multiply;
extern BasicOperation divide;
extern BasicOperation reverse_divide;

extern BasicOperation power;
extern BasicOperation logarithm;
extern BasicOperation absolute;
extern BasicOperation minimum;
extern BasicOperation maximum;


extern BasicOperation invert;
extern BasicOperation grayscale;
extern BasicOperation blackwhite;
extern BasicOperation median;

extern vector<BasicOperation *> basicOperations;