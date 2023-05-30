#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class InterpolationSearch {
public:
	void set_a(double x) { a = x; }
	void set_b(double x) { b = x; }
	void set_precision(int n) { precision = n; }

	InterpolationSearch() {}
	InterpolationSearch(double a, double b, int p) : a(a), b(b), precision(p) {}

	bool interpolation_search(int& n);
	bool binary_search(int& n);

private:
	double a, b;
	int precision;
	double arr(int pos);
};

double InterpolationSearch::arr(int pos) {
	return a + (double)pos / pow(10, precision);
}

bool InterpolationSearch::interpolation_search(int& try_cnt) {
	double hi_d = (b - a) * pow(10, precision) + 0.5;
	int lo = 0, hi = (int)hi_d;
	cout << (double)(b - a) * pow(10, precision);
	cout << " " << hi;
	int option = 1;
	try_cnt = 0;
	std::srand(std::time(nullptr));
	while (arr(lo) != arr(hi) && option != 0) {
		double min = arr(lo);
		double max = arr(hi);
		double x = min + (double)(rand()) / ((double)(RAND_MAX / (max - min)));

		int pos = (double)lo + (((double)(hi - lo) / (max - min)) * (x - min)) + 0.5;

		double pivot = arr(pos);

		cout << endl << "Da li je broj:" << endl << "0 - u opsegu preciznosti" << endl << "1 - manji" << endl << "2 - veci" << endl << "od " << pivot << endl;
		cin >> option;

		if (option == 1) {
			hi = pos - 1;
		}
		else {
			lo = pos + 1;
		}
		try_cnt++;
	}
	if (arr(lo) == arr(hi)) {
		cout << endl << "Da li je broj:" << endl << "0 - u opsegu preciznosti" << endl << "1 - manji" << endl << "2 - veci" << endl << "od " << arr(lo) << endl;
		cin >> option;
		if (option == 0) return true;
		else return false;
	}
	return true;
}

bool InterpolationSearch::binary_search(int& try_cnt) {
	double hi_d = (b - a) * pow(10, precision) + 0.5;
	int lo = 0, hi = (int)hi_d;
	try_cnt = 0;
	int option;
	while (lo <= hi) {
		int mid = lo + (hi - lo) / 2;
		double pivot = arr(mid);
		cout << endl << "Da li je broj:" << endl << "0 - u opsegu preciznosti" << endl << "1 - manji" << endl << "2 - veci" << endl << "od " << pivot << endl;
		cin >> option;

		try_cnt++;
		if (option == 0) {
			return true;
		}
		else
			if (option == 1) {
				hi = mid - 1;
			}
			else {
				lo = mid + 1;
			}
	}
	return false;
}


int main() {
	double a, b;
	int option = 1, precision, n;
	InterpolationSearch s;

	while (option != 0) {
		cout << endl;
		cout << "1. Unos donje granice" << endl;
		cout << "2. Unos gornje granice" << endl;
		cout << "3. Unos preciznosti" << endl;
		cout << "4. Binarna pretraga" << endl;
		cout << "5. Interpolaciona pretraga" << endl;
		cout << "0. Izlazak" << endl;
		cout << "Izaberite opciju: ";
		cin >> option;
		switch (option)
		{
		case 1:
			cout << "Unesite granicu: ";
			cin >> a;
			s.set_a(a);
			break;

		case 2:
			cout << "Unesite granicu: ";
			cin >> b;
			s.set_b(b);
			break;

		case 3:
			cout << "Unesite broj decimala: ";
			cin >> n;
			s.set_precision(n);
			break;

		case 4:
			n = 0;
			if (s.binary_search(n)) {
				cout << endl << "Bilo je potrebno " << n << " koraka";
			}
			else {
				cout << endl << "Broj nije pronadjen";
			}
			break;

		case 5:
			n = 0;
			if (s.interpolation_search(n)) {
				cout << endl << "Bilo je potrebno " << n << " koraka";
			}
			else {
				cout << endl << "Broj nije pronadjen";
			}
			break;
		default:
			break;
		}
	}
	return 0;
}
