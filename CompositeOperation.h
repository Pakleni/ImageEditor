#pragma once
#include<vector>
#include<string>
#include "Operation.h"
#include "Image.h"

using namespace std;

class CompositeOperation :
	public Operation
{
	vector<Operation *> operations;

protected:
	void run_noround(Image& image) const;
public:
	CompositeOperation();
	CompositeOperation(const string& string);
	CompositeOperation(const string& name, const vector<Operation *> & operations);
	CompositeOperation(const CompositeOperation&);
	CompositeOperation& operator=(const CompositeOperation&);
	CompositeOperation(CompositeOperation&&) noexcept;
	~CompositeOperation();

	string getC() const;

	void save(const string& path) const;
	void parse(const string&);
	void load(const string& path);

	void run(Image& img) const;
	Operation* clone();
};

