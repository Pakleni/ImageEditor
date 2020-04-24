#include <iostream>
#include <string>
#include "Image.h"
#include "BMPFormatter.h"
#include "PAMFormatter.h"
#include "MyFormatter.h"
#include "BasicOperation.h"
#include "CompositeOperation.h"
#include "InvalidFile.h"

using namespace std;

int menu() {

	int c = 0;
	//UCITAVANJE SLIKE
	cout << "1. Ucitaj sliku" << endl;
	cout << "2. Ucitaj Projekat" << endl;
	//RAD SA SLOJEVIMA
	cout << "3. Dodaj sloj" << endl;
	cout << "4. Aktiviraj/Deaktiviraj sloj" << endl;
	cout << "5. Ucini sloj vidljivim/nevidljivim" << endl;
	cout << "6. Obrisi sloj" << endl;
	//DEFINISANJE SELEKCIJA
	cout << "7. Dodaj Selekciju" << endl;
	cout << "8. Izaberi Selekciju" << endl;
	cout << "9. Obrisi Selekciju" << endl;
	//OPERACIJE NAD SLIKOM
	cout << "10. Izvrsi Operaciju" << endl;
	cout << "11. Dodaj Operaciju" << endl;
	cout << "12. Ucitaj Operaciju" << endl;
	cout << "13. Sacuvaj Operaciju" << endl;
	//EKSPORTOVANJE SLIKE
	cout << "14. Sacuvaj Sliku" << endl;
	//SNIMANJE PROJEKTA
	cout << "15. Snimi Projekat" << endl;
	//KRAJ RADA
	cout << "16. Kraj rada" << endl;

	while (!(cin >> c)) {
		cerr << "Unesi opet" << endl;cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return c;
}

void loadImage(Image& img) {
	try {
		string path;
		cout << "Unesi put do slike" << endl;
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
			cerr << "Nevalidna ekstenzija" << endl;
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
	cout << "Unesi put do slike" << endl;
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
		cout << "1. Iz fajla" << endl;
		cout << "2. Sa sirinom i visinom" << endl;
		cout << "3. Bez specifikacija" << endl;

		int c;

		while (!(cin >> c) || c < 1 || c > 3) {
			cerr << "Unesi opet" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}


		if (c == 1) {
			string path;
			cout << "Unesi put do slike" << endl;
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
				cerr << "Nevalidna ekstenzija" << endl;
				return;
			}
		}
		else if (c == 2) {
			unsigned int width = 0;
			unsigned int height = 0;

			cout << "Unesi visinu" << endl;
			while (!(cin >> height)) {
				cerr << "Unesi opet" << endl; cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			cout << "Unesi sirinu" << endl;
			while (!(cin >> width)) {
				cerr << "Unesi opet" << endl; cin.clear();
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
		cout << "Unesi indeks" << endl;
		unsigned c;

		while (!(cin >> c)) {
			cerr << "Unesi opet" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		cout << "1. Deaktiviraj" << endl;
		cout << "2. Aktiviraj" << endl;

		int d;
		while (!(cin >> d || d < 1 || d > 2)) {
			cerr << "Unesi opet" << endl; cin.clear();
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
		cout << "Unesi indeks" << endl;
		unsigned c;

		while (!(cin >> c)) {
			cerr << "Unesi opet" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		cout << "1. Deaktiviraj" << endl;
		cout << "2. Aktiviraj" << endl;

		int d;
		while (!(cin >> d || d < 1 || d > 2)) {
			cerr << "Unesi opet" << endl; cin.clear();
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
		cout << "Unesi indeks" << endl;
		unsigned c;

		while (!(cin >> c)) {
			cerr << "Unesi opet" << endl; cin.clear();
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

	cout << "Unsesi ime" << endl;
	cin >> name;

	cout << "Unesi broj pravougaonika" << endl;
	unsigned c;

	while (!(cin >> c)) {
		cerr << "Unesi opet" << endl;cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	vector<Rectangle> rectangles;

	for (unsigned i = 0; i < c; i++) {
		unsigned int w, h, x, y;
		cout << "Unesi sirinu" << endl;
		while (!(cin >> w)) {
			cerr << "Unesi opet" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "Unesi visinu" << endl;
		while (!(cin >> h)) {
			cerr << "Unesi opet" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "Unesi x gornjeg levog coska" << endl;
		while (!(cin >> x)) {
			cerr << "Unesi opet" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "Unesi y gornjeg levog coska" << endl;
		while (!(cin >> y)) {
			cerr << "Unesi opet" << endl;cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		rectangles.push_back(Rectangle(w, h, x, y));
	}


	Selection select(name,rectangles);

	img.addSelect(select);
}

void chooseSelection(Image& img) {
	string name;
	cout << "Unesi ime" << endl;
	cin >> name;
	if (!img.select(name)) {
		cout << "Ne postoji trazena selekcija" << endl;
	}
}

void deleteSelection(Image& img) {
	img.setSaved(0);
	string name;
	cout << "Unesi ime" << endl;
	cin >> name;
	if (!img.deleteSelection(name)) {
		cout << "Ne postoji trazena selekcija" << endl;
	}
}

void doOperation(Image& img) {
	img.setSaved(0);

	cout << "Koja Operacija?" << endl;

	cout << "1. Dodaj" << endl;
	cout << "2. Oduzmi" << endl;
	cout << "3. Obrnuto oduzmi" << endl;
	cout << "4. Pomnozi" << endl;
	cout << "5. Podeli" << endl;
	cout << "6. Obrnuto podeli" << endl;
	cout << "7. Podigni na stepen" << endl;
	cout << "8. Logaritam" << endl;
	cout << "9. Absolutna vrednost" << endl;
	cout << "10. Minimum" << endl;
	cout << "11. Maksimum" << endl;
	cout << "12. Invertuj" << endl;
	cout << "13. Pretvori u sivo" << endl;
	cout << "14. Pretvori u crno/belo" << endl;
	cout << "15. Mediana" << endl;
	cout << "16. Popuni" << endl;
	cout << "17. Kompozitna" << endl;

	unsigned c;

	while (!(cin >> c) || c < 1 || c > 17) {
		cerr << "Unesi opet" << endl; cin.clear();
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
		else if (c == 13) { BlackWhite().run(img); }
		else if (c == 14) { Grayscale().run(img); }
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
			cerr << "Unesi opet" << endl; cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		img.getOperations()[c - 1].run(img);
	}
}

void addOperation(Image& img){
	img.setSaved(0);

	string name;
	cout << "Unesi ime" << endl;
	cin >> name;

	cout << "Koliko funkcija" << endl;
	unsigned c;

	while (!(cin >> c)) {
		cerr << "Unesi opet" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	
	vector<Operation*> operations;
	for (unsigned i = 0; i < c; i++) {
		cout << "Koja Operacija?" << endl;

		cout << "1. Dodaj" << endl;
		cout << "2. Oduzmi" << endl;
		cout << "3. Obrnuto oduzmi" << endl;
		cout << "4. Pomnozi" << endl;
		cout << "5. Podeli" << endl;
		cout << "6. Obrnuto podeli" << endl;
		cout << "7. Podigni na stepen" << endl;
		cout << "8. Logaritam" << endl;
		cout << "9. Absolutna vrednost" << endl;
		cout << "10. Minimum" << endl;
		cout << "11. Maksimum" << endl;
		cout << "12. Invertuj" << endl;
		cout << "13. Pretvori u sivo" << endl;
		cout << "14. Pretvori u crno/belo" << endl;
		cout << "15. Mediana" << endl;
		cout << "16. Popuni" << endl;
		cout << "17. Kompozitna" << endl;

		unsigned c;

		while (!(cin >> c) || c < 1 || c > 17) {
			cerr << "Unesi opet" << endl; cin.clear();
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
				cerr << "Unesi opet" << endl; cin.clear();
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
		cout << "Unesi putanju" << endl;
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
		cout << "Unesi put do slike" << endl;
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
			cerr << "Nevalidna ekstenzija" << endl;
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
		cout << "Unesi put do slike" << endl;
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
			cerr << "Nevalidna ekstenzija" << endl;
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
			cerr << "Nevalidna ekstenzija" << endl;
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
		cerr << "Unesi opet" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}


	cout << "Unesite putanju" << endl;
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

	cout << "Da li zelite da sacuvate projekat?" << endl;

	cout << "1. Da" << endl;
	cout << "2. Ne" << endl;

	unsigned c;

	while (!(cin >> c) || c < 1 || c > 2) {
		cerr << "Unesi opet" << endl; cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	if (c == 2) return;

	bool flag = 0;
	try {
		cout << "Unesite putanju" << endl;
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

int main(int argc, char* argv[]) {
	Image img;

	if (argc <3) {
		int c = 1;
		while (c != 16) {
			c = menu();
			system("cls");
			perform(img, c);
		}

		if (c) endSave(img);
	}
	else {
		javaMode(img, argc, argv);
	}
}