#pragma once
#include "Pixel.h"
#include <string>

class Image;
class Operation
{
protected:
	std::string name = "";
	virtual void run_noround(Image& image) const = 0;
public:
	Operation() = default;
	Operation(const std::string& name);

	virtual void run(Image& image) const= 0;
	virtual ~Operation();
	virtual std::string stringify() const = 0;


};

