#include "CompositeOperation.h"
#include <fstream>
#include <boost/regex.hpp>
#include <sstream>
#include "InvalidFile.h"

using namespace boost;

CompositeOperation::CompositeOperation(){}

CompositeOperation::CompositeOperation(const string& str){this->parse(str);}

CompositeOperation::CompositeOperation(const string& _name, const vector<BasicOperation *>& ops): Operation(_name), operations(ops) {}

string CompositeOperation::getName() const{
	return name;
}

char front[] = "{\"name\":\"";
char mid[] = "\",\"fun\":[";
char back[] = "]}";

string json_escape(string s) {
	size_t pos;
	pos = s.find("\\");
	while (pos != string::npos) {
		s.replace(pos, 1, "\\\\");
		pos = s.find("\\", pos+2);
	}
	pos = s.find("\"");
	while (pos != string::npos) {
		s.replace(pos, 1, "\\\"");
		pos = s.find("\"", pos+2);
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

string CompositeOperation::stringify() const {
	string save = front + json_escape(name) + mid + getName() + back;
	return save;
}

void CompositeOperation::save(const string& path) const{
	ofstream file(path);

	if (!file.is_open()) throw InvalidFile();

	string save = stringify();
	file.write(save.c_str(), save.size());

	file.close();
}

void CompositeOperation::parse(const string& str) {
	regex reg("\\\"name\\\":\\\"((?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])+)\\\"");

	smatch matches;

	if (!regex_search(str, matches, reg)) throw InvalidFile();

	name = json_unescape(matches.str(1));

	reg = regex("\\\"fun\\\":\\[(.*)\\]");
	
	if (!regex_search(str, matches, reg)) throw InvalidFile();


	reg = regex("\\\"((?:\\\\\\\\|\\\\\\\"|[^\\\\\\\"])*)\\\"");

	sregex_iterator curr(matches[1].begin(), matches[1].end(), reg);
	sregex_iterator last;

	while (curr != last) {
		string key = curr->str(1);
		if (key == "add") operations.push_back(&add);
		else if (key == "sub") operations.push_back(&subtract);
		else if (key == "rsb") operations.push_back(&reverse_subtract);
		else if (key == "mul") operations.push_back(&multiply);
		else if (key == "div") operations.push_back(&divide);
		else if (key == "rdv") operations.push_back(&reverse_divide);
		else if (key == "pow") operations.push_back(&power);
		else if (key == "log") operations.push_back(&logarithm);
		else if (key == "abs") operations.push_back(&absolute);
		else if (key == "min") operations.push_back(&minimum);
		else if (key == "max") operations.push_back(&maximum);
		else if (key == "inv") operations.push_back(&invert);
		else if (key == "b&w") operations.push_back(&blackwhite);
		else if (key == "gra") operations.push_back(&grayscale);
		else if (key == "med") operations.push_back(&median);
		curr++;
	}
}

void CompositeOperation::load(const string& path) {

	ifstream file(path);
	if (!file.is_open()) throw InvalidFile();

	stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	return parse(buffer.str());
}

void CompositeOperation::run(Image& img) const{
	run_noround(img);
	for (auto& pix : img) { pix.round(); }
}

void CompositeOperation::run_noround(Image& img) const{
	for (auto& op : operations) { op->run_noround(img); }
}