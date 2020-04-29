#include "Selection.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "InvalidFile.h"

using namespace boost;

extern string json_escape(string);
extern string json_unescape(string);

Selection::Selection(const string & selection_name, const vector<Rectangle> & rects) : name(selection_name), rectangles(rects) {}

Selection::Selection() {}

string Selection::stringify() const {
	
	vector<string> rect_str;
	for (auto& rect : rectangles) {
		rect_str.push_back(rect.stringify());
	}

	return "{\"name\":\"" + json_escape(name) + "\",\"rects\":[" + boost::join(rect_str, ",") + "]}";
}

void Selection::parse(const string& str) {
	regex reg("\\\"name\\\":\\\"((?:\\\\\\\"|\\\\\\\\|[^\\\\\\\"])+)\\\"");

	smatch matches;

	if (!regex_search(str, matches, reg)) throw InvalidFile();

	name = json_unescape(matches.str(1));

	reg = "\"rects\":\\[(.*)\\]";
	
	if (!regex_search(str, matches, reg)) throw InvalidFile();

	reg = "\\{\"w\":([0-9]*),\"h\":([0-9]*),\"x\":([0-9]*),\"y\":([0-9]*)\\}";
	
	sregex_iterator m1(matches[1].begin(), matches[1].end(), reg);
	sregex_iterator m2;
	
	while(m1 != m2){

		rectangles.push_back(Rectangle(
			stoi((*m1)[1]),
			stoi((*m1)[2]),
			stoi((*m1)[3]),
			stoi((*m1)[4])
		));

		m1++;
	}

}

const string& Selection::getName() const { return name; }

const vector<Rectangle>& Selection::getRectangles() const { return rectangles; }