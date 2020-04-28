#include "Image.h"
#include <algorithm>
#include <math.h>
#include <stdexcept>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "InvalidFile.h"


using namespace boost;

Image::Image(const Layer& layer): height(layer.getHeight()), width(layer.getWidth()){
	layers.push_back(layer);
}

bool Image::select(const string & selectionName) {
	for (auto i = selections.begin(); i != selections.end(); i++) {
		if (i->getName() == selectionName) {
			activeSelection = &(*i);
			return true;
		}
	}
	return false;
}

void Image::addSelect(const Selection& selection) {
	selections.push_back(selection);
	activeSelection = &selections.back();
}

Image::~Image() {}

Image::iterator::iterator(Image * _img,unsigned _l, unsigned _h, unsigned _w): img(_img), l(_l), h(_h), w(_w){
	
}

Image::iterator& Image::iterator::operator++(){
	do { (*this).next(); }
	while ((*this) != img->end() && !(*this).selected());

	return *this;
}

Image::iterator& Image::iterator::next() {
	if (!img->layers[l].isActive()) {l++; h = 0; w = 0;}
	else if (w + 1 < img->width) w++;
	else if (h + 1 < img->height) { h++; w = 0; }
	else { l++; h = 0; w = 0; }
	return *this;
}

bool Image::iterator::operator==(iterator oth){
	if (w == oth.w && h == oth.h && l == oth.l && img == oth.img) return true;
	return false;
}

bool Image::iterator::operator!=(iterator oth){
	return !(*this == oth);
}

Pixel& Image::iterator::operator*(){
	return img->layers[l][h][w];
}

Pixel* Image::iterator::operator->(){
	return &img->layers[l][h][w];
}

bool Image::iterator::selected() {
	if (!img->layers[l].isActive())
		return false;
	if (!img->activeSelection)
		return true;
	for (Rectangle r : img->activeSelection->rectangles) {
		if (w >= r.getX() && w < r.getX() + r.getWidth() && h <= r.getY() && (r.getY() < r.getHeight() || h > r.getY() - r.getHeight()))
			return true;
	}
	return false;
}

Pixel Image::iterator::median(){

	int r = 0;
	int g = 0;
	int b = 0;

	int cnt = 0;

	for (int i = (signed)h - 1;i <= (signed)h + 1;++i) {
		if (i >= 0 && i < (signed)img->height) {
			for (int j = w - 1; j <= (signed)w + 1; ++j) {
				if (j >= 0 && j < (signed)img->width) {
					++cnt;
					r += img->layers[l][i][j].getR();
					g += img->layers[l][i][j].getG();
					b += img->layers[l][i][j].getB();
				}
			}
		}
	}

	return Pixel(r / cnt, g / cnt, b / cnt, (*this)->getA());
}

Image::iterator Image::begin(){
	iterator _begin(this, 0, 0, 0);

	if (_begin != end() && !_begin.selected()) {
		++_begin;
	}

	return _begin;
}

Image::iterator Image::end() {
	return iterator(this, layers.size(), 0, 0);
}

Image::Image() {}

Pixel Image::get(unsigned h, unsigned w) const{

	double r = 0, g = 0, b = 0, a = 0;

	for (const Layer & l : layers) {
		if (!l.isVisible()) continue;

		const Pixel& pix = l[h][w];
		double srcA = pix.getA() / 100.0;
		double d = a * (1 - srcA);

		a = srcA + d;
		if (a != 0) {
			r = (pix.getR() * srcA + r * d) / a;
			g = (pix.getG() * srcA + g * d) / a;
			b = (pix.getB() * srcA + b * d) / a;
		}
	}
	return Pixel(round(r), round(g), round(b), round(a * 100));
}

void Image::addLayer(const Layer& layer) {
	layers.push_back(layer);

	if (width > layer.getWidth() && height > layer.getHeight()) {

		layers.back().expand(width, height);
		return;
	}

	width = max(width, layer.getWidth());
	height = max(height, layer.getHeight());

	for (Layer& curr : layers) {
		curr.expand(width, height);
	}
}

void Image::removeLayer(unsigned i) {
	if (i < layers.size()) {
		layers.erase(layers.begin() + i);
	}
}

unsigned Image::getWidth() const {
	return width;
}

unsigned Image::getHeight() const {
	return height;
}

Layer& Image::operator[](unsigned ind) {
	if (ind < layers.size()) {
		return layers[ind];
	}
	else throw std::out_of_range("Index is out of range\n");
}

const Layer& Image::operator[](unsigned ind) const {
	if (ind < layers.size()) {
		return layers[ind];
	}
	else throw std::out_of_range("Index is out of range\n");
}

const char out0[]= "{\"width\":";
const char out1[] = ",\"height\":";
const char out2[] = ",\"layers\":[";
const char out3[] = "],\"functions\":[";
const char out4[] = "],\"selections\":[";
const char out5[] = "]}";

string Image::stringify() const {
	
	vector<string> layers_str;
	for (auto& layer : layers) {
		layers_str.push_back(layer.stringify());
	}

	vector<string> functions_str;
	for (auto& func : operations) {
		functions_str.push_back(func.stringify());
	}

	vector<string> selections_str;
	for (auto& sel : selections) {
		selections_str.push_back(sel.stringify());
	}


	string str = out0;	
	str += to_string(width);	
	str += out1;
	str += to_string(height);
	str += out2;
	str += boost::algorithm::join(layers_str, ",") ;
	str += out3;
	str += boost::algorithm::join(functions_str, ",") ;
	str += out4;
	str += boost::algorithm::join(selections_str, ",") ;
	str += out5;
	

	return str;

}

void Image::addOperation(const CompositeOperation& operation) {
	operations.push_back(operation);
}

void Image::load(const string& path) {

	(*this) = Image();

	ifstream file(path);

	if (!file.is_open()) throw InvalidFile();

	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string str = buffer.str();

	regex reg("\\{\\\"width\\\":([0-9]*),\\\"height\\\":([0-9]*),\\\"layers\\\":\\[(.*)\\],\\\"functions\\\":\\[(.*)\\],\\\"selections\\\":\\[(.*)\\]\\}");
		
	smatch matches;
	
	if (!regex_search(str, matches, reg)) throw InvalidFile();

	width = stoi(matches[1]);
	height = stoi(matches[2]);

	sregex_iterator m2;


	reg = "\\{([^\\}]*)\\}";
	sregex_iterator m1(matches[3].begin(), matches[3].end(), reg);
	while (m1!=m2) {
		layers.push_back(Layer(width,height));
		layers.back().parse((*m1)[1]);
		++m1;
	}
	reg = "\\{(\\\"name\\\":\\\"(?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])*\\\",\\\"c\\\":\\[(?:(?:(?R)|(?:(?:[0-9]*,?)*)),?)*\\])\\}";
	m1 = sregex_iterator(matches[4].begin(), matches[4].end(), reg);
	while (m1!=m2){
		operations.push_back(CompositeOperation(m1->str(1)));
		++m1;
	}
	reg = "\\{(\\\"name\\\":\\\"(?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])*\\\",\\\"rects\\\":\\[[^\\]]*\\])\\}";
	m1 = sregex_iterator(matches[5].begin(), matches[5].end(), reg);
	while (m1!=m2) {
		selections.push_back(Selection());
		selections.back().parse(m1->str(1));
		++m1;
	}

}

void Image::setSaved(bool _saved) { saved = _saved; }

bool Image::getSaved() { return saved;  }

const vector<CompositeOperation>& Image::getOperations() const { return operations; }

const vector<Selection>& Image::getSelections() const { return selections; }

const int Image::layerCount() const { return layers.size(); };

bool Image::deleteSelection(const string& selectionName) {
	for (auto i = selections.begin(); i != selections.end(); ++i) {
		if (i->getName() == selectionName) {
			if (activeSelection == &(*i)) {
				activeSelection = nullptr;
			}
			selections.erase(i);
			return true;
		}
	}
	return false;
}
