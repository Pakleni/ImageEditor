#include "BasicOperation.h"
#include <algorithm>
#include <iostream>

using namespace std;

BasicOperation::BasicOperation(const string& _name, function<void(Image&)> _f): Operation(_name), f(_f) {}

vector<BasicOperation *> basicOperations{
	&add,
	&subtract,
	&reverse_subtract,
	&multiply,
	&divide,
	&reverse_divide,
	&power,
	&logarithm,
	&absolute,
	&minimum,
	&maximum,
	&invert,
	&grayscale,
	&blackwhite,
	&median
};

void BasicOperation::run(Image & img) const{
	run_noround(img);
	for (auto& pix: img) {pix.round();}
}

void BasicOperation::run_noround(Image& img) const {
	f(img);
}

string BasicOperation::getName() const{
	return "\""+name+"\"";
}

string BasicOperation::stringify() const{
	return "{\"name\":"+getName()+",\"func\":["+getName()+"]}";
}

//FUNCTIONS
BasicOperation add("add", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix = pix + c; }});

BasicOperation subtract("sub", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix = pix - c; }});

BasicOperation reverse_subtract("rsb", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix = c - pix; }});

BasicOperation multiply("mul", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix = pix * c; }});

BasicOperation divide("div", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix = pix / c; }});

BasicOperation reverse_divide("rdv", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix = c / pix; }});

BasicOperation power("pow", [](Image& img)->void {
	double c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix.power(c); }});

BasicOperation logarithm("log", [](Image& img)->void {
	for (auto& pix : img) { pix.log(); }});

BasicOperation absolute("abs", [](Image& img)->void {
	for (auto& pix : img) { pix.abs(); }});

BasicOperation minimum("min", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix.min(c); }});

BasicOperation maximum("max", [](Image& img)->void {
	int c;
	cout << "How much ? ";
	cin >> c;
	for (auto& pix : img) { pix.max(c); }});

BasicOperation invert("inv", [](Image& img)->void {
	for (auto& pix : img) { pix.invert(); }});

BasicOperation blackwhite("b&w", [](Image& img)->void {
	for (auto& pix : img) { pix.blackwhite(); }});

BasicOperation grayscale("gra", [](Image& img)->void {
	for (auto& pix : img) { pix.grayscale(); }});

BasicOperation median("med", [](Image& img)->void {
	Image temp = img;
	vector <Pixel> medians;
	for (auto pix = temp.begin(); pix != temp.end(); ++pix) {
		medians.push_back(pix.median());
	};
	int i = 0;
	for (auto& pix : img) { pix = medians[i++]; }});
