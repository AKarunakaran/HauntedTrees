#include <iostream>
#include <iomanip>
#include <vector>
#include "../hauntedTrees.h"
#include "../rbTree.h"
using namespace std;

struct levelSet {
	RedBlack<int>* rbTree;
	int median;
	int left;
};

//Merges current level set with previous
void merge(vector<levelSet>& levelSets) {
	levelSets[levelSets.size()-2].rbTree->treeUnion(levelSets[levelSets.size()-1].rbTree);
	levelSets.pop_back();
	levelSets.back().median = levelSets.back().rbTree->median();
}

int main() {
	int n;
	cout << fixed << setprecision(2);
	cin >> n;
	vector<int> data(n, 0);
	vector<levelSet> levelSets;

	//Input
	for(int i = 0; i < n; ++i) {
		cin >> data[i];
	}

	//Regression algorithm
	for(int i = 0; i < n; ++i) {
		RedBlack<int>* rbToAdd = new RedBlack<int>;
		rbToAdd->add(data[i], 1);
		levelSets.push_back({rbToAdd, data[i], i});
		while(levelSets.size() > 1 && levelSets[levelSets.size()-1].median <= levelSets[levelSets.size()-2].median) {
			merge(levelSets);
		}
	}

	for(int i = 0; i < levelSets.size(); ++i) {
		cout << levelSets[i].left << "\t";
	}
	cout << endl;
	for(int i = 0; i < levelSets.size(); ++i) {
		cout << levelSets[i].median << "\t";
	}
	cout << endl;

	return 0;
}