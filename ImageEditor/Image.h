#pragma once
#include <vector>
#include <string>
#include "Layer.h"
#include "Selection.h"
#include "CompositeOperation.h"

using namespace std;

class CompositeOperation;

class Image
{

	unsigned int height = 0, width = 0;
	vector<Layer> layers;
	vector<Selection> selections;
	vector<CompositeOperation> operations;

	Selection* activeSelection = nullptr;
	bool saved = 0;
public:

	class iterator {
		Image* img;
		unsigned l, h, w;
		iterator& next();
		iterator(Image * image,unsigned layer, unsigned height, unsigned width);
	public:
		iterator& operator++();
		bool operator==(iterator);
		bool operator!=(iterator);
		Pixel& operator*();
		Pixel* operator->();

		bool selected();
		Pixel median();

		friend Image;
	};

	Image(const Layer& base_layer);
	Image();
	~Image();
	bool select(const string & selection_name);
	void addSelect(const Selection & selection);
	void addOperation(const CompositeOperation & operation);
	void addLayer(const Layer & layer);
	void removeLayer(unsigned layer_index);
	bool deleteSelection(const string & selection_name);

	Layer& operator[](unsigned index);
	const Layer& operator[](unsigned index) const;

	void load(const string&);

	unsigned getWidth() const;
	unsigned getHeight() const;
	Pixel get(unsigned height, unsigned width) const;
	string stringify() const;
	
	void setSaved(bool);
	bool getSaved();

	const vector<CompositeOperation>& getOperations() const;

	iterator begin();
	iterator end();
};

