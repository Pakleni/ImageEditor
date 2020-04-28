#include <iostream>
#include <string>
#include "Image.h"
#include "BMPFormatter.h"
#include "PAMFormatter.h"
#include "MyFormatter.h"
#include "BasicOperation.h"
#include "CompositeOperation.h"
#include "InvalidFile.h"
#include <SDL.h>
#include <thread>
#include <math.h>

#define WIDTH 1080
#define HEIGHT 720

using namespace std;

int menu() {

	int c = 0;
	//UCITAVANJE SLIKE
	cout << "1. Load image" << endl;
	cout << "2. Load project" << endl;
	//RAD SA SLOJEVIMA
	cout << "3. Add layer" << endl;
	cout << "4. Activate/Deactivate layer" << endl;
	cout << "5. Make layer (in)visible" << endl;
	cout << "6. Delete layer" << endl;
	//DEFINISANJE SELEKCIJA
	cout << "7. Add Selection" << endl;
	cout << "8. Choose Selection" << endl;
	cout << "9. Delete Selection" << endl;
	//OPERACIJE NAD SLIKOM
	cout << "10. Perform Operation" << endl;
	cout << "11. Add Operation" << endl;
	cout << "12. Load Operation" << endl;
	cout << "13. Save Operation" << endl;
	//EKSPORTOVANJE SLIKE
	cout << "14. Save image" << endl;
	//SNIMANJE PROJEKTA
	cout << "15. Save project" << endl;
	//KRAJ RADA
	cout << "16. Exit" << endl;

	while (!(cin >> c)) {
		cerr << "Try Again" << endl;cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	
	return c;
}

void loadImage(Image& img) {
	try {
		string path;
		cout << "Enter path" << endl;
		cin >> path;

		size_t bmp, pam, json;

		bmp = path.find(".bmp");
		pam = path.find(".pam");
		json = path.find(".json");

		if (bmp != string::npos && (pam == string::npos || pam < bmp) && (json == string::npos || json < bmp)) {
			BMPFormatter form(path);
			form >> img;
		}
		else if (pam != string::npos && (json == string::npos || json < pam)) {
			PAMFormatter form(path);
			form >> img;
		}
		else {
			cerr << "Non-valid extension" << endl;
			return;
		}
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		img = Image();
	}
}

void loadProject(Image& img) {
	try {

	string path;
	cout << "Enter path" << endl;
	cin >> path;

	MyFormatter form(path);

	form >> img;
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		img = Image();
	}
}

void addLayer(Image& img) {
	Layer layer;
	bool flag = 1;
	try {
		img.setSaved(0);
		cout << "1. From file" << endl;
		cout << "2. Width and Height" << endl;
		cout << "3. No specifications" << endl;

		int c;

		while (!(cin >> c) || c < 1 || c > 3) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}


		if (c == 1) {
			string path;
			cout << "Enter path" << endl;
			cin >> path;

			size_t bmp, pam, json;

			bmp = path.find(".bmp");
			pam = path.find(".pam");
			json = path.find(".json");

			if (bmp != string::npos && (pam == string::npos || pam < bmp) && (json == string::npos || json < bmp)) {
				BMPFormatter form(path);
				form >> layer;
			}
			else if (pam != string::npos && (json == string::npos || json < pam)) {
				PAMFormatter form(path);
				form >> layer;
			}
			else {
				cerr << "Non-valid extension" << endl;
				return;
			}
		}
		else if (c == 2) {
			unsigned int width = 0;
			unsigned int height = 0;

			cout << "Enter height" << endl;
			while (!(cin >> height)) {
				cerr << "Try Again" << endl; cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			cout << "Enter width" << endl;
			while (!(cin >> width)) {
				cerr << "Try Again" << endl; cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			layer.expand(width, height);
		}

	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		flag = 0;
	}
	if (flag) img.addLayer(layer); 
}

void activateLayer(Image& img) {
	try {

		if (img.layerCount() == 0) {
			cout << "No Layers" << endl;
			return;
		}

		cout << "Enter index [0-" << img.layerCount()-1 << "]" <<  endl;
		unsigned c;

		while (!(cin >> c)) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		cout << "1. Deactivate" << endl;
		cout << "2. Activate" << endl;

		int d;
		while (!(cin >> d || d < 1 || d > 2)) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (d == 1) {
			img[c].setActive(false);
		}
		else {
			img[c].setActive(true);
		}
	}
	catch (std::out_of_range& e) {
		cerr << e.what() << endl;
	}
}

void setLayerVisible(Image& img) {
	try {

		if (img.layerCount() == 0) {
			cout << "No Layers" << endl;
			return;
		}

		cout << "Enter index [0-" << img.layerCount() - 1 << "]" << endl;
		unsigned c;

		while (!(cin >> c)) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		cout << "1. Invisible" << endl;
		cout << "2. Visible" << endl;

		int d;
		while (!(cin >> d || d < 1 || d > 2)) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (d == 1) {
			img[c].setVisible(false);
		}
		else {
			img[c].setVisible(true);
		}
	}
	catch (std::out_of_range& e) {
		cerr << e.what() << endl;
	}
}

void deleteLayer(Image& img) {
	try {
		img.setSaved(0);
		if (img.layerCount() == 0) {
			cout << "No Layers" << endl;
			return;
		}

		cout << "Enter index [0-" << img.layerCount() - 1 << "]" << endl;
		unsigned c;
		while (!(cin >> c)) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		img.removeLayer(c);
	}
	catch (std::out_of_range& e) {
		cerr << e.what() << endl;
	}
}

void addSelection(Image& img) {
	img.setSaved(0);
	string name;

	cout << "Enter name" << endl;
	cin >> name;

	cout << "Enter number of rectangles" << endl;
	unsigned c;

	while (!(cin >> c)) {
		cerr << "Try Again" << endl;cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	vector<Rectangle> rectangles;

	for (unsigned i = 0; i < c; i++) {
		unsigned int w, h, x, y;
		cout << "Enter width" << endl;
		while (!(cin >> w)) {
			cerr << "Try Again" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "Enter height" << endl;
		while (!(cin >> h)) {
			cerr << "Try Again" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "Enter x of upper left corner" << endl;
		while (!(cin >> x)) {
			cerr << "Try Again" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "Enter y of upper left corner" << endl;
		while (!(cin >> y)) {
			cerr << "Try Again" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		rectangles.push_back(Rectangle(w, h, x, y));
	}


	Selection select(name,rectangles);

	img.addSelect(select);
}

void chooseSelection(Image& img) {
	string name;

	for (auto& i : img.getSelections()) {
		cout << i.getName() << endl;
	}

	cout << "Enter name" << endl;
	cin >> name;
	if (!img.select(name)) {
		cout << "This Selection doesn't exist" << endl;
	}
}

void deleteSelection(Image& img) {
	img.setSaved(0);

	for (auto& i : img.getSelections()) {
		cout << i.getName() << endl;
	}

	string name;
	cout << "Enter name" << endl;
	cin >> name;
	if (!img.deleteSelection(name)) {
		cout << "This Selection doesn't exist" << endl;
	}
}

void doOperation(Image& img) {
	img.setSaved(0);

	cout << "Which operation?" << endl;

	cout << "1. Add" << endl;
	cout << "2. Subtract" << endl;
	cout << "3. Reverse subtract" << endl;
	cout << "4. Multiply" << endl;
	cout << "5. Divide" << endl;
	cout << "6. Reverse divide" << endl;
	cout << "7. Power" << endl;
	cout << "8. Logarithm" << endl;
	cout << "9. Absolute Value" << endl;
	cout << "10. Minimum" << endl;
	cout << "11. Maximum" << endl;
	cout << "12. Invert" << endl;
	cout << "13. Grayscale" << endl;
	cout << "14. Black & White" << endl;
	cout << "15. Median" << endl;
	cout << "16. Fill" << endl;
	cout << "17. Composite" << endl;

	unsigned c;

	while (!(cin >> c) || c < 1 || c > 17) {
		cerr << "Try Again" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	if (c != 17) {
		if (c >= 1 && c <= 6 || c == 10 || c == 11) {
			int d;
			cin >> d;
			if (c == 1) { Add(d).run(img); }
			else if (c == 2) { Subtract(d).run(img); }
			else if (c == 3) { ReverseSubtract(d).run(img); }
			else if (c == 4) { Multiply(d).run(img); }
			else if (c == 5) { Divide(d).run(img); }
			else if (c == 6) { ReverseDivide(d).run(img); }
			else if (c == 10) { Minimum(d).run(img); }
			else if (c == 11) { Maximum(d).run(img); }
		}
		else if (c == 7 || c == 8) {
			double d;
			cin >> d;
			if (c == 7) { Power(d).run(img); }
			else if (c == 8) { Logarithm(d).run(img); }
		}
		else if (c == 9) { Absolute().run(img); }
		else if (c == 12) { Invert().run(img); }
		else if (c == 13) { Grayscale().run(img); }
		else if (c == 14) { BlackWhite().run(img); }
		else if (c == 15) { Median().run(img); }
		else if (c == 16) {
			int r, g, b, a;
			cin >> r >> g >> b >> a;
			Fill(r, g, b, a).run(img);
		}
	}
	else {
		unsigned i = 0;
		for (auto& op: img.getOperations()) {
			cout << ++i << ". " << op.getName() << endl;
		}
		unsigned c;

		while (!(cin >> c) || c < 1 || c > i) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		img.getOperations()[c - 1].run(img);
	}
}

void addOperation(Image& img){
	img.setSaved(0);

	string name;
	cout << "Enter name" << endl;
	cin >> name;

	cout << "How many functions?" << endl;
	unsigned c;

	while (!(cin >> c)) {
		cerr << "Try Again" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	vector<Operation*> operations;
	for (unsigned i = 0; i < c; i++) {
		cout << "Which operation?" << endl;

		cout << "1. Add" << endl;
		cout << "2. Subtract" << endl;
		cout << "3. Reverse subtract" << endl;
		cout << "4. Multiply" << endl;
		cout << "5. Divide" << endl;
		cout << "6. Reverse divide" << endl;
		cout << "7. Power" << endl;
		cout << "8. Logarithm" << endl;
		cout << "9. Absolute Value" << endl;
		cout << "10. Minimum" << endl;
		cout << "11. Maximum" << endl;
		cout << "12. Invert" << endl;
		cout << "13. Grayscale" << endl;
		cout << "14. Black & White" << endl;
		cout << "15. Median" << endl;
		cout << "16. Fill" << endl;
		cout << "17. Composite" << endl;

		unsigned c;

		while (!(cin >> c) || c < 1 || c > 17) {
			cerr << "Try Again" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (c >= 1 && c <= 6 || c == 10 || c == 11) {
			int con;
			cin >> con;
			if (c == 1) { operations.push_back(new Add(con)); }
			else if (c == 2) { operations.push_back(new Subtract(con)); }
			else if (c == 3) { operations.push_back(new ReverseSubtract(con)); }
			else if (c == 4) { operations.push_back(new Multiply(con)); }
			else if (c == 5) { operations.push_back(new Divide(con)); }
			else if (c == 6) { operations.push_back(new ReverseDivide(con)); }
			else if (c == 10) { operations.push_back(new Minimum(con)); }
			else if (c == 11) { operations.push_back(new Maximum(con)); }
		}
		else if (c == 7 || c == 8) {
			double con;
			cin >> con;
			if (c == 7) { operations.push_back(new Power(con)); }
			else if (c == 8) { operations.push_back(new Logarithm(con)); }
		}
		else if (c == 9) { operations.push_back(new Absolute()); }
		else if (c == 12) { operations.push_back(new Invert()); }
		else if (c == 13) { operations.push_back(new BlackWhite()); }
		else if (c == 14) { operations.push_back(new Grayscale()); }
		else if (c == 15) { operations.push_back(new Median()); }
		else if (c == 16) {
			int r, g, b, a;
			cin >> r >> g >> b >> a;
			operations.push_back(new Fill(r,g,b,a));
		}
		else if (c == 17){
			unsigned b = 0;
			for (auto& op : img.getOperations()) {
				cout << ++b << ". " << op.getName() << endl;
			}
			unsigned d;
			if (b == 0) return;

			while (!(cin >> d) || d < 1 || d > b) {
				cerr << "Try Again" << endl; cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			operations.push_back(new CompositeOperation(img.getOperations()[d - 1]));
		}
	}

	img.addOperation(CompositeOperation(name, operations));
}

void loadOperation(Image& img) {
	bool flag = 1;
	img.setSaved(0);

	CompositeOperation operation;
	try {
		string path;
		cout << "Enter path" << endl;
		cin >> path;

		operation.load(path);
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		flag = 0;
	}
	img.addOperation(operation);
}

void savePicture(Image& img) {
	try {
		string path;
		cout << "Enter path" << endl;
		cin >> path;

		size_t bmp, pam, json;

		bmp = path.find(".bmp");
		pam = path.find(".pam");
		json = path.find(".json");

		if (bmp != string::npos && (pam == string::npos || pam < bmp) && (json == string::npos || json < bmp)) {
			BMPFormatter form(path);
			form << img;
		}
		else if (pam != string::npos && (json == string::npos || json < pam)) {
			PAMFormatter form(path);
			form << img;
		}
		else {
			cerr << "Non-valid extension" << endl;
			return;
		}
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
	}
}

void saveProject(Image& img) {
	try {
		img.setSaved(1);
		string path;
		cout << "Enter path" << endl;
		cin >> path;

		MyFormatter form(path);

		form << img;
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
	}
}

void javaMode(Image& img, int argc, char* argv[]) {

	string path(argv[1]);
	string op_path(argv[2]);

	size_t bmp, pam, json;

	bmp = path.find(".bmp");
	pam = path.find(".pam");
	json = path.find(".json");
	try {
		if (bmp != string::npos && (pam == string::npos || pam < bmp) && (json == string::npos || json < bmp)) {
			BMPFormatter form(path);
			form >> img;
		}
		else if (pam != string::npos && (json == string::npos || json < pam)) {
			PAMFormatter form(path);
			form >> img;
		}
		else if (json != string::npos) {
			MyFormatter form(path);
			form >> img;
		}
		else {
			cerr << "Non-valid extension" << endl;
			return;
		}
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		return;
	}

	CompositeOperation operation;

	try {
		operation.load(op_path);
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		return;
	}

	operation.run(img);


	try {
		if (bmp != string::npos && (pam == string::npos || pam < bmp) && (json == string::npos || json < bmp)) {
			BMPFormatter form(path);
			form << img;
		}
		else if (pam != string::npos && (json == string::npos || json < pam)) {
			PAMFormatter form(path);
			form << img;
		}
		else if (json != string::npos) {
			MyFormatter form(path);
			form << img;
		}
		else {
			cerr << "Non-valid extension" << endl;
			return;
		}
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		return;
	}
}

void saveOperation(Image& img) {
	unsigned i = 0;
	for (auto& op : img.getOperations()) {
		cout << ++i << ". " << op.getName() << endl;
	}
	unsigned c;
	if (i == 0) return;

	while (!(cin >> c) || c < 1 || c > i) {
		cerr << "Try Again" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}


	cout << "Enter path" << endl;
	string path;
	cin >> path;
	try {
		img.getOperations()[c - 1].save(path);
	}
	catch(InvalidFile & e) {
		cerr << e.what() << endl;
	}
}

void perform(Image& img, int c) {
	switch (c) {
	case 1:
		loadImage(img);
		break;
	case 2:
		loadProject(img);
		break;
	case 3:
		addLayer(img);
		break;
	case 4:
		activateLayer(img);
		break;
	case 5:
		setLayerVisible(img);
		break;
	case 6:
		deleteLayer(img);
		break;
	case 7:
		addSelection(img);
		break;
	case 8:
		chooseSelection(img);
		break;
	case 9:
		deleteSelection(img);
		break;
	case 10:
		doOperation(img);
		break;
	case 11:
		addOperation(img);
		break;
	case 12:
		loadOperation(img);
		break;
	case 13:
		saveOperation(img);
		break;
	case 14:
		savePicture(img);
		break;
	case 15:
		saveProject(img);
		break;
	}
}

void endSave(Image& img) {
	if (img.getSaved()) return;

	cout << "Do you want to save?" << endl;

	cout << "1. Yes" << endl;
	cout << "2. No" << endl;

	unsigned c;

	while (!(cin >> c) || c < 1 || c > 2) {
		cerr << "Try Again" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	if (c == 2) return;

	bool flag = 0;
	try {
		cout << "Enter path" << endl;
		string path;
		cin >> path;
		MyFormatter form(path);

		form << img;
	}
	catch (InvalidFile& e) {
		cerr << e.what() << endl;
		flag = 1;
	}
	if (flag) return endSave(img);
}

void editImage(Image* img, volatile bool* running, volatile bool* update, volatile bool* finished) {

	int c = 1;
	while (c != 16 && running) {
		c = menu();
		system("cls");
		perform(*img, c);

		*update = true;
	}

	endSave(*img);
	*finished = true;

	*running = false;
}

int updateWindow(Image& img, SDL_Window* window, SDL_Renderer* renderer) {

	SDL_RenderClear(renderer);

	unsigned char * pixels;

	pixels = new unsigned char[4 * img.getHeight() * img.getWidth()];

	for (int i = 0; i < img.getHeight(); ++i) {
		for (int j = 0; j < img.getWidth(); ++j) {
			pixels[(i * img.getWidth() + j) * 4    ] = img.get(i, j).getR();
			pixels[(i * img.getWidth() + j) * 4 + 1] = img.get(i, j).getG();
			pixels[(i * img.getWidth() + j) * 4 + 2] = img.get(i, j).getB();
			pixels[(i * img.getWidth() + j) * 4 + 3] = img.get(i, j).getA() * 255 / 100;
		}
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom((void *) pixels, img.getWidth(), img.getHeight(), 32, 4 * img.getWidth(), SDL_PIXELFORMAT_RGBA32);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);


	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_FreeSurface(surface);
	delete[] pixels;
	if (!surface) return EXIT_FAILURE;


	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
	Image img;

	if (argc == 3) {
		javaMode(img, argc, argv);
		return EXIT_SUCCESS;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf(SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window* window = SDL_CreateWindow("Image Editor",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	if (window == NULL) {
		printf(SDL_GetError());
		return EXIT_FAILURE;
	}


	volatile bool running = true;
	volatile bool update = false;
	volatile bool finished = false;

	thread t (editImage ,&img ,&running, &update, &finished);

	t.detach();

	SDL_Event windowEvent;

	int sx = 0, sy = 0;
	int dx = 0, dy = 0;
	int selId = 0;

	while (running) {

		while (SDL_PollEvent(&windowEvent))
			if (SDL_QUIT == windowEvent.type) {
				running = 0;
			}
			else if (SDL_MOUSEBUTTONDOWN == windowEvent.type) {
				sx = windowEvent.button.x * img.getWidth() / WIDTH;
				sy = windowEvent.button.y * img.getHeight() / HEIGHT;
			}
			else if (SDL_MOUSEBUTTONUP == windowEvent.type) {
				dx = windowEvent.button.x * img.getWidth() / WIDTH;
				dy = windowEvent.button.y * img.getHeight() / HEIGHT;

				Rectangle rect(abs(sx - dx), abs(sy - dy), fmin(sx, dx), fmax(sy, dy));
				vector<Rectangle> rects;
				rects.push_back(rect);
				Selection sel(to_string(selId++), rects);
				img.addSelect(sel);
			}

		
		if (update) {
			if (updateWindow(img, window, renderer)) {
				running = false;
			}
			update = false;
		}
		
		SDL_Delay(1000 / 60);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	running = false;
	while (!finished);

	return EXIT_SUCCESS;
}