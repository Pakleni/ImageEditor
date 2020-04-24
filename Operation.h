#pragma once
#include "Pixel.h"
#include <string>

class Image;
class CompositeOperation;

class Operation
{
protected:
	std::string name = "";
	virtual void run_noround(Image& image) const = 0;
public:
	Operation(const std::string& name);
	Operation() = default;
	virtual ~Operation() = default;

	void run(Image& image) const;
	virtual void parse(const std::string&);
	virtual std::string getC() const;

	std::string stringify() const;
	std::string getName() const;

	friend CompositeOperation;

	virtual Operation * clone() = 0;
};
