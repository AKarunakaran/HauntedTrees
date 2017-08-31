#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "../../hauntedTrees.h"
#include "../../rbTree.h"
using namespace std;

struct dataPoint {
	double weight;
	int val;
};


void swap(dataPoint& a, dataPoint& b) {
	dataPoint temp = a;
	a = b;
	b = temp;
}

struct levelSet {
	vector<dataPoint> ltHeap; //Heap of points less than median -- max heap
	vector<dataPoint> gtHeap; //Heap of points greater than median -- min heap
	dataPoint median;
	double ltWeight;
	double gtWeight;
	int left;
};


//Fix up for min heap, starting from index k
void fixUpMin(vector<dataPoint>& heap, int k) {
	while(k && heap[k/2].val > heap[k].val) {
		swap(heap[k/2], heap[k]);
		k /= 2;
	}
}

//Fix up for max heap, starting from index k
void fixUpMax(vector<dataPoint>& heap, int k) {
	while(k && heap[k/2].val < heap[k].val) {
		swap(heap[k/2], heap[k]);
		k /= 2;
	}
}

//Fix down for min heap, starting from index k
void fixDownMin(vector<dataPoint>& heap, int k) {
	int heapSize = heap.size();
	while(2*k+1 < heapSize && (heap[2*k+1].val > heap[k].val || (2*k+2 < heapSize && heap[2*k+2].val > heap[k].val))) {
		if(2*k+2 < heapSize) {
			if(heap[2*k+1].val < heap[2*k+2].val) {
				swap(heap[2*k+1], heap[k]);
				k = 2*k+1;
			} else {
				swap(heap[2*k+2], heap[k]);
				k = 2*k+2;
			}
		} else {
			swap(heap[2*k+1], heap[k]);
			k = 2*k+1;
		}
	}
}

//Fix down for the max heap, starting from index k
void fixDownMax(vector<dataPoint>& heap, int k) {
	int heapSize = heap.size();
	while(2*k+1 < heapSize && (heap[2*k+1].val < heap[k].val || (2*k+2 < heapSize && heap[2*k+2].val < heap[k].val))) {
		if(2*k+2 < heapSize) {
			if(heap[2*k+1].val > heap[2*k+2].val) {
				swap(heap[2*k+1], heap[k]);
				k = 2*k+1;
			} else {
				swap(heap[2*k+2], heap[k]);
				k = 2*k+2;
			}
		} else {
			swap(heap[2*k+1], heap[k]);
			k = 2*k+1;
		}
	}
}

void addMin(vector<dataPoint>& heap, dataPoint& toAdd) {
	heap.push_back(toAdd);
	fixUpMin(heap, heap.size()-1);
}

void addMax(vector<dataPoint>& heap, dataPoint& toAdd) {
	heap.push_back(toAdd);
	fixUpMax(heap, heap.size()-1);
}

void popMin(vector<dataPoint>& heap) {
	heap[0] = heap.back();
	heap.pop_back();
	fixDownMin(heap, 0);
}

void popMax(vector<dataPoint>& heap) {
	heap[0] = heap.back();
	heap.pop_back();
	fixDownMax(heap, 0);
}

void addToLevelSet(levelSet& lSet, dataPoint& toAdd) {
	if(toAdd.val < lSet.median.val) {
		lSet.ltWeight += toAdd.weight;
		addMax(lSet.ltHeap, toAdd);
	} else {
		lSet.gtWeight += toAdd.weight;
		addMin(lSet.gtHeap, toAdd);
	}
	double totalWeight = lSet.median.weight + lSet.ltWeight + lSet.gtWeight;
	while(lSet.ltWeight > totalWeight/2 && lSet.gtWeight > totalWeight/2) {
		if(lSet.ltWeight > lSet.gtWeight) {
			addMin(lSet.gtHeap, lSet.median);
			lSet.gtWeight += lSet.median.weight;
			lSet.median = lSet.ltHeap[0];
			lSet.ltWeight -= lSet.ltHeap[0].weight;
			popMax(lSet.ltHeap);
		} else {
			addMax(lSet.ltHeap, lSet.median);
			lSet.ltWeight += lSet.median.weight;
			lSet.median = lSet.gtHeap[0];
			lSet.gtWeight -= lSet.gtHeap[0].weight;
			popMin(lSet.gtHeap);
		}
	}
}

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
			data[i].val = (rand() % 100) + i/500;
			data[i].weight = 3*((double) rand() / (double) (RAND_MAX + 1.));
		}
	} else {
		for(int i = 1; i < data.size(); ++i) {
			data[i].val = (rand() % 100) + i/500;
		}
	}
}

void merge(levelSet& a, levelSet& b) {
	for(int i = 0; i < b.ltHeap.size(); ++i) {
		addToLevelSet(a, b.ltHeap[i]);
	}
	for(int i = 0; i < b.gtHeap.size(); ++i) {
		addToLevelSet(a, b.gtHeap[i]);
	}
	addToLevelSet(a, b.median);
}

void process(int n, bool weighted) {
	vector<dataPoint> data(n, {1, 0});
	vector<levelSet> levelSets;

	generateData(data, weighted);

	for(int i = 0; i < n; ++i) {
		levelSets.push_back({{}, {}, {1, data[i].val}, 0, 0, i});
		while(levelSets.size() > 1 && levelSets[levelSets.size()-1].median.val <= levelSets[levelSets.size()-2].median.val) {
			merge(levelSets[levelSets.size()-2], levelSets[levelSets.size()-1]);
			levelSets.pop_back();
		}
	}
}

void nextHeapfile(char* file) {
	char str[13];
	int count = 1;
	strcpy(str, "heapTimeData");
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
	char file[19];
	nextHeapfile(file);
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