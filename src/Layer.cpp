#include "Layer.h"
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "InvalidFile.h"

using namespace boost;

Layer::Layer(unsigned int w, unsigned int h) : width(w), height(h) {}

Layer::~Layer() {}

Layer::Layer() {}

void Layer::expand(unsigned int w, unsigned int h) {

	for (auto &i : pixels) {
		i.resize(w);
	}
	pixels.resize(h,vector<Pixel>(w));
	height = h;
	width = w;
}

vector<Pixel>& Layer::operator[](unsigned int h){
	if (h < height) {
		return pixels[h];
	}
	else throw std::out_of_range("Index is out of range\n");
}

const vector<Pixel>& Layer::operator[](unsigned int h) const{
	if (h < height) {
		return pixels[h];
	}
	else throw std::out_of_range("Index is out of range\n");
}

unsigned int Layer::getHeight() const {	return height;	}

unsigned int Layer::getWidth() const {	return width;	}

bool Layer::isActive() const{ return active; }

bool Layer::isVisible() const{ return visible; }

string Layer::stringify() const{

	string pixels_str = "{\"pixels\":[";

	for (auto& row : pixels) {
		for (auto& pix : row) {
			pix.stringify(pixels_str);
			pixels_str += ",";
		}
	}
	
	if (width&&height > 0) pixels_str.pop_back();

	pixels_str += "],\"alpha\":";
	pixels_str += to_string(alpha);
	pixels_str += ",\"active\":";
	pixels_str += active ? "true" : "false";
	pixels_str += ",\"visible\":";
	pixels_str += visible ? "true" : "false";
	pixels_str += "}";

	return pixels_str;
}

struct InvalidFile;

void Layer::parse(const string& str){

	regex  reg("\\[([0-9]*),([0-9]*),([0-9]*),([0-9]*)\\]");

	sregex_iterator m1(str.begin(), str.end(), reg);
	sregex_iterator m2;
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (m1 == m2) throw InvalidFile();

			pixels[i][j] = Pixel(
				stoi((*m1)[1]), 
				stoi((*m1)[2]), 
				stoi((*m1)[3]), 
				stoi((*m1)[4]));

			++m1;
		}
	}

	regex reg1("\\\"pixels\\\":\\[(.*)\\],\\\"alpha\\\":([0-9]*),\\\"active\\\":(.*),\\\"visible\\\":(.*)");

	smatch matches;

	if (!regex_search(str, matches, reg1)) throw InvalidFile();

	this->alpha = stoi(matches[2]);

	if (!(matches[3].compare("true"))) {
		this->active = true;
	}
	else {
		this->active = false;
	}

	if (!(matches[4].compare("true"))) {
		this->visible = true;
	}
	else {
		this->visible = false;
	}

}

void Layer::setVisible(bool val) { visible = val; }

void Layer::setActive(bool val) { active = val; }