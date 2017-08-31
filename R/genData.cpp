#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
	ofstream fout("uniRegData1.txt");
	int n = 10000;

	fout << n << endl;
	for(int i = 0; i < n/2; ++i) {
		fout << max(0, 2*i + (rand() % 7) - 3) << " " << 1 << endl;
	}
	for(int i = n/2; i < n; ++i) {
		fout << max(0, 2*(10000-i) + (rand() % 7) - 3) << " " << 1 << endl;
	}

	return 0;
}