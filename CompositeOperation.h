#pragma once
#include<vector>
#include<string>
#include "BasicOperation.h"
#include "Image.h"

using namespace std;

class BasicOperation;

class CompositeOperation :
	public Operation
{
	vector<BasicOperation *> operations;
protected:
	void run_noround(Image& image) const;
public:
	CompositeOperation();
	CompositeOperation(const string& string);
	CompositeOperation(const string& name, const vector<BasicOperation *> & operations);

	string getName() const;
	string stringify() const;

	void save(const string& path) const;
	void parse(const string&);
	void load(const string& path);

	void run(Image& img) const;

};

