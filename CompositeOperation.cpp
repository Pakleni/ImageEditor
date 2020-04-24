#include "CompositeOperation.h"
#include "BasicOperation.h"
#include <fstream>
#include <boost/regex.hpp>
#include <sstream>
#include "InvalidFile.h"

using namespace boost;

string json_escape(string s) {
	size_t pos;
	pos = s.find("\\");
	while (pos != string::npos) {
		s.replace(pos, 1, "\\\\");
		pos = s.find("\\", pos + 2);
	}
	pos = s.find("\"");
	while (pos != string::npos) {
		s.replace(pos, 1, "\\\"");
		pos = s.find("\"", pos + 2);
	}
	return s;
}

string json_unescape(string s) {
	size_t pos;
	pos = s.find("\\\\");
	while (pos != string::npos) {
		s.replace(pos, 2, "\\");
		pos = s.find("\\\\", pos + 1);
	}
	pos = s.find("\\\"");
	while (pos != string::npos) {
		s.replace(pos, 2, "\"");
		pos = s.find("\\\"", pos + 1);
	}
	return s;
}

CompositeOperation::CompositeOperation(){}

CompositeOperation::CompositeOperation(const string& str){

	regex reg("\\\"name\\\":\\\"((?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])*)\\\",\\\"c\\\":\\[(.*)\\]");

	smatch matches;
	if (!regex_search(str, matches, reg)) throw InvalidFile();
	name = json_unescape(matches[1]);

	this->parse(matches[2]);
}

CompositeOperation::CompositeOperation(const string& _name, const vector<Operation *>& ops): Operation(_name), operations(ops) {}

CompositeOperation::CompositeOperation(const CompositeOperation& cop) {
	name = cop.getName();
	for (Operation* op : cop.operations) {
		operations.push_back(op->clone());
	};
}

CompositeOperation& CompositeOperation::operator=(const CompositeOperation& cop) {
	if (this != &cop) {
		name = cop.getName();
		for (Operation* op : cop.operations) {
			operations.push_back(op->clone());
		};
	}

	return *this;
}

CompositeOperation::CompositeOperation(CompositeOperation&& cop) noexcept {
	name = cop.getName();
	operations = cop.operations;
	cop.operations = vector<Operation*>();
}

CompositeOperation::~CompositeOperation() {
	for (Operation* op : operations) {
		delete op;
	}
}

string CompositeOperation::getC() const {
	string c;

	for (Operation* op : operations) {
		c += op->stringify();
		c += ',';
	}

	if (c.size() > 0) c.pop_back();

	return c;
}

void CompositeOperation::save(const string& path) const{
	ofstream file(path);

	if (!file.is_open()) throw InvalidFile();

	string save = stringify();
	file.write(save.c_str(), save.size());

	file.close();
}

void CompositeOperation::parse(const string& str) {
	
	regex reg ("\\{\\\"name\\\":\\\"((?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])*)\\\",\\\"c\\\":\\[((?:(?:(?R)|(?:(?:[0-9]*,?)*)),?)*)\\]\\}");

	sregex_iterator curr(str.begin(), str.end(), reg);
	sregex_iterator last;

	while (curr != last) {
		string key = curr->str(1);
		string c = curr->str(2);
		Operation* op;
		if (key == "add") {op = new Add();}
		else if (key == "sub") {op = new Subtract();}
		else if (key == "rsb") { op = new ReverseSubtract(); }
		else if (key == "mul") { op = new Multiply(); }
		else if (key == "div") { op = new Divide(); }
		else if (key == "rdv") { op = new ReverseDivide(); }
		else if (key == "pow") { op = new Power(); }
		else if (key == "log") { op = new Logarithm(); }
		else if (key == "abs") { op = new Absolute(); }
		else if (key == "min") { op = new Minimum(); }
		else if (key == "max") { op = new Maximum(); }
		else if (key == "inv") { op = new Invert(); }
		else if (key == "b&w") { op = new BlackWhite(); }
		else if (key == "gra") { op = new Grayscale(); }
		else if (key == "med") { op = new Median(); }
		else if (key == "fill") { op = new Fill(); }
		else { op = new CompositeOperation(); op->name = key; }
		op->parse(c);
		operations.push_back(op);

		curr++;
	}
}

void CompositeOperation::load(const string& path) {

	ifstream file(path);
	if (!file.is_open()) throw InvalidFile();

	stringstream buffer;
	buffer << file.rdbuf();

	regex reg("\\\"name\\\":\\\"((?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])*)\\\",\\\"c\\\":\\[(.*)\\]");

	smatch matches;
	if (!regex_search(buffer.str(), matches, reg)) throw InvalidFile();
	name = json_unescape(matches[1]);

	file.close();

	return parse(matches[2]);
}

void CompositeOperation::run(Image& img) const{
	run_noround(img);
	for (auto& pix : img) { pix.round(); }
}

void CompositeOperation::run_noround(Image& img) const{
	for (auto& op : operations) { op->run_noround(img); }
}

Operation* CompositeOperation::clone() {
	return new CompositeOperation(*this);
}