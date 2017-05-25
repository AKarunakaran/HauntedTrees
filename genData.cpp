#include <iostream>
#include <vector>
using namespace std;

struct dataPoint {
	double weight;
	int value;
};

void generateData(vector<dataPoint>& data, bool weighted) {
	if(weighted) {
		for(int i = 1; i < data.size(); ++i) {
			if(i % 50 == 0)
				data[i].value = 2*(i-25) + ((rand() % 7) - 3);
			else
				data[i].value = 2*i + ((rand() % 7) - 3);
			data[i].weight = 3*((double) rand() / (double) (RAND_MAX + 1.));
		}
	} else {
		for(int i = 1; i < data.size(); ++i) {
			if(i % 50 == 0)
				data[i].value = 2*(i-25) + ((rand() % 7) - 3);
			else
				data[i].value = 2*i + ((rand() % 7) - 3);
		}
	}
}

int main() {
	int n = 1000;
	bool weighted = false;
	vector<dataPoint> data(n, {1, 0});

	generateData(data, weighted);

	cout << n << endl;
	for(int i = 0; i < n; ++i) {
		cout << data[i].value << endl;
	}

	return 0;
}