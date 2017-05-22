#include <iostream>
#include <iomanip>
#include <vector>
#include "hauntedTrees.h"
#include "avl.h"
using namespace std;

struct levelSet {
	AVL<int>* avlTree;
	int median;
	int left;
};

//Merges current level set with previous
void merge(vector<levelSet>& levelSets) {
	levelSets[levelSets.size()-2].avlTree->treeUnion(levelSets[levelSets.size()-1].avlTree);
	levelSets.pop_back();
	levelSets.back().median = levelSets.back().avlTree->median();
}

int main() {
	int n;
	cout << fixed << setprecision(2);
	cin >> n;
	vector<int> data(n, 0);
	vector<levelSet> levelSets;
	AVL<int> baseTree;

	//Input
	for(int i = 0; i < n; ++i) {
		cin >> data[i];
	}
	for(int i = 0; i < n; ++i) {
		baseTree.add(data[i], 1);
	}

	//Regression algorithm
	for(int i = 0; i < n; ++i) {
		AVL<int>* avlToAdd = new AVL<int>;
		avlToAdd->add(data[i], 1);
		levelSets.push_back({avlToAdd, data[i], i});
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