#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include "hauntedTrees.h"
#include "avl.h"
using namespace std;

//Returns the starting index of the previous level set, -1 if there is no previous level set
levelSet& previousLevelSet(deque<levelSet>& levelSets) {
	levelSet top = levelSets.back();
	levelSets.pop_back();
	levelSet& second = levelSets.back();
	levelSets.push_back(top);
	return second;
}

//Merges current level set with previous
void merge(deque<levelSet>& levelSets) {
	levelSet top = levelSets.back();
	levelSets.pop_back();
	if(top.max > levelSets.back().max) levelSets.back().max = top.max;
	if(top.min < levelSets.back().min) levelSets.back().min = top.min;
	levelSets.back().mean = (levelSets.back().max + levelSets.back().min)/2.0;
}

int main() {
	int n;
	cout << fixed << setprecision(2);
	cin >> n;
	vector<int> data(n, 0);
	deque<levelSet> levelSets;

	//Input
	for(int i = 0; i < n; ++i) {
		cin >> data[i];
	}

	//Regression algorithm
	for(int i = 0; i < n; ++i) {
		levelSet lSet = {(double) data[i], data[i], data[i], i};
		levelSets.push_back(lSet);
		while(levelSets.size() > 1 && levelSets.back().mean <= previousLevelSet(levelSets).mean) {
			merge(levelSets);
		}
	}

	//Printing the result
	levelSet temp;
	cout << endl;
	for(int i = 0; i < levelSets.size(); ++i) {
		temp = levelSets.front();
		cout << temp.left << "\t";
		levelSets.pop_front();
		levelSets.push_back(temp);
	}
	cout << endl;
	for(int i = 0; i < levelSets.size(); ++i) {
		temp = levelSets.front();
		cout << temp.mean << "\t";
		levelSets.pop_front();
		levelSets.push_back(temp);
	}
	cout << endl;

	return 0;
}