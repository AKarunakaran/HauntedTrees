#include <iostream>
#include <iomanip>
#include <vector>
#include "hauntedTrees.h"
#include "avl.h"
using namespace std;

struct levelSet {
	HauntedTree<int, AVL<int> >* hTree;
	int median;
	int left;
};

//Merges current level set with previous
void merge(vector<levelSet>& levelSets) {
	levelSets[levelSets.size()-2].hTree->treeUnion(levelSets[levelSets.size()-1].hTree);
	levelSets.pop_back();
	levelSets.back().median = levelSets.back().hTree->median();
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
		HauntedTree<int, AVL<int> >* hTreeToAdd = new HauntedTree<int, AVL<int> >(&baseTree);
		hTreeToAdd->add(data[i]);
		levelSets.push_back({hTreeToAdd, data[i], i});
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