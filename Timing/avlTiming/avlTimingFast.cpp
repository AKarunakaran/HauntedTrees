#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "../../hauntedTrees.h"
#include "../../avl.h"
using namespace std;

struct levelSet {
	AVL<int>* avlTree;
	int median;
	int left;
};

struct dataPoint {
	double weight;
	int value;
};

/*void generateData(vector<dataPoint>& data, bool weighted) {
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
}*/

void generateData(vector<dataPoint>& data, bool weighted) {
	if(weighted) {
		for(int i = 1; i < data.size(); ++i) {
			data[i].value = (rand() % 100) + i/500;
			data[i].weight = 3*((double) rand() / (double) (RAND_MAX + 1.));
		}
	} else {
		for(int i = 1; i < data.size(); ++i) {
			data[i].value = (rand() % 100) + i/500;
		}
	}
}

void merge(vector<levelSet>& levelSets) {
	auto& prevTree = levelSets[levelSets.size()-2].avlTree;
	auto& backTree = levelSets[levelSets.size()-1].avlTree;
	if(prevTree->getSize() < backTree->getSize()) {
		AVL<int>* temp = prevTree;
		prevTree = backTree;
		backTree = temp;
	}
	prevTree->fast_merge(*(backTree));
	levelSets.pop_back();
	levelSets.back().median = levelSets.back().avlTree->median();
}

void process(int n, bool weighted) {
	vector<dataPoint> data(n, {1, 0});
	vector<levelSet> levelSets;

	generateData(data, weighted);

	for(int i = 0; i < n; ++i) {
		AVL<int>* avlToAdd = new AVL<int>;
		avlToAdd->add(data[i].value, data[i].weight);
		levelSets.push_back({avlToAdd, data[i].value, i});
		while(levelSets.size() > 1 && levelSets[levelSets.size()-1].median <= levelSets[levelSets.size()-2].median) {
			merge(levelSets);
		}
	}
}

void nextAVLfile(char* file) {
	char str[16];
	int count = 1;
	strcpy(str, "avlFastTimeData");
	sprintf(file, "%s%d.txt", str, count);
	ifstream inFS(file);
	while(inFS.good() && count < 100) {
		inFS.close();
		++count;
		sprintf(file, "%s%d.txt", str, count);
		inFS.open(file);
	}
	if(count == 100)
		strcpy(file, "Error");
}

int main() {
	srand(time(NULL));
	char c;
	cout << "Weighted? (y/n): ";
	cin >> c;
	bool weighted = (c == 'y');
	vector<int> cacheFlush(10000, 0);
	for(int i = 0; i < 10000; ++i)
		cacheFlush[i] = i;
	char file[22];
	nextAVLfile(file);
	ofstream outFS(file);

	for(int i = 30; i <= 300030; i += 1000) {
		for(int j = 0; j < 10000; ++j)
			cacheFlush[j] = j;
		clock_t begin = clock();
		process(i, weighted);
		clock_t end = clock();
		double t = double(end - begin)/CLOCKS_PER_SEC;
		outFS << i << " " << t << endl;
	}
	outFS << endl;
	outFS.close();
	
	return 0;
}