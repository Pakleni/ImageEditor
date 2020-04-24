#include "Operation.h"
#include "Image.h"

Operation::Operation(const std::string& _name) : name(_name) {}

void Operation::run(Image& img) const {
	this->run_noround(img);
	for (auto& pix : img) { pix.round(); }
}

std::string Operation::getName() const {
	return name;
}

std::string Operation::getC() const {
	return "";
}

std::string Operation::stringify() const {
	return "{\"name\":\"" + getName() + "\",\"c\":[" + this->getC() + "]}";
}

void Operation::parse(const std::string&) {}