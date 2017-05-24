#include "hauntedTrees.h"
#include "basicTree.h"
#include "avl.h"
#include <iostream>
using namespace std;

int main() {
	Tree<int> t;
	t.add(3, 1);
	t.add(6, 1);
	t.add(7, 1);
	t.add(1, 1);
	t.add(2, 1);
	t.add(10, 1);
	t.add(0, 1);
	t.print();
	cout << endl;
	
	HauntedTree<int, Tree<int> > h(&t);
	AVL<int> avl;
	h.add(0);
	avl.add(0, 1);
	cout << h.median() << " " << avl.median() << endl;
	h.add(3);
	avl.add(3, 1);
	cout << h.median() << " " << avl.median() << endl;
	h.add(10);
	avl.add(10, 1);
	cout << h.median() << " " << avl.median() << endl;
	h.add(2);
	avl.add(2, 1);
	cout << h.median() << " " << avl.median() << endl;
	h.add(1);
	avl.add(1, 1);
	cout << h.median() << " " << avl.median() << endl;
	h.add(7);
	avl.add(7, 1);
	cout << h.median() << " " << avl.median() << endl;
	h.add(6);
	avl.add(6, 1);
	cout << h.median() << " " << avl.median() << endl;

	return 0;
}