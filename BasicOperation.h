#include "Operation.h"
#include <string>

using namespace std;

class Image;
class Add : public Operation {
	int c = 0;
public:

	Add();
	Add(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Subtract : public Operation {
	int c = 0;
public:

	Subtract();
	Subtract(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class ReverseSubtract : public Operation {
	int c = 0;
public:

	ReverseSubtract();
	ReverseSubtract(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Multiply : public Operation {
	int c = 0;
public:

	Multiply();
	Multiply(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Divide : public Operation {
	int c = 0;
public:

	Divide();
	Divide(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class ReverseDivide : public Operation {
	int c = 0;
public:

	ReverseDivide();
	ReverseDivide(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Power : public Operation {
	double c = 0;
public:

	Power();
	Power(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Logarithm : public Operation {
	double c = 0;
public:

	Logarithm();
	Logarithm(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Absolute : public Operation {
public:

	Absolute();

	void run_noround(Image& img) const;
	Operation* clone();
};

class Minimum : public Operation {
	int c = 0;
public:

	Minimum();
	Minimum(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Maximum : public Operation {
	int c = 0;
public:

	Maximum();
	Maximum(int _c);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation* clone();
};

class Invert : public Operation {
public:

	Invert();

	void run_noround(Image& img) const;
	Operation* clone();
};

class BlackWhite : public Operation {
public:

	BlackWhite();

	void run_noround(Image& img) const;
	Operation* clone();
};

class Grayscale : public Operation {
public:

	Grayscale();

	void run_noround(Image& img) const;
	Operation* clone();
};

class Median : public Operation {
public:

	Median();

	void run_noround(Image& img) const;
	Operation* clone();
};

class Fill : public Operation {
	Pixel c;
public:


	Fill();
	Fill(int r, int g, int b, int a);

	void run_noround(Image& img) const;
	void parse(const string&);
	string getC() const;
	Operation * clone();
};
