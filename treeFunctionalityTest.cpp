#include "hauntedTrees.h"
#include "avl.h"
#include <iostream>
using namespace std;

int main() {
	Tree<int> t;
	t.add(3);
	t.add(6);
	t.add(7);
	t.add(1);
	t.add(2);
	t.add(10);
	t.add(0);
	t.print();
	cout << endl;
	
	HauntedTree<int, Tree<int> > h(&t);
	h.add(0);
	cout << h.median() << endl;
	h.add(10);
	cout << h.median() << endl;
	h.add(3);
	cout << h.median() << endl;
	h.add(7);
	cout << h.median() << endl;
	h.add(1);
	cout << h.median() << endl;
	h.add(2);
	cout << h.median() << endl;
	h.add(6);
	cout << h.median() << endl;

	/*HauntedTree<int, Tree<int> > h2(&t), h3(&t);
	h2.add(0);
	h2.add(2);
	h3.add(6);
	h3.add(10);
	h2.print();
	cout << endl;
	h3.print();
	cout << endl;
	h2.treeUnion(&h3);
	h2.print();
	h2.add(3);
	h2.add(7);
	h2.print();*/

	AVL<double> avl, avl2;
	avl.add(.1, .1);
	avl.add(.35, .35);
	avl.add(.05, .05);
	avl.add(.1, .1);
	avl.add(.15, .15);
	avl.add(.05, .05);
	avl.add(.2, .2);
	cout << avl.median() << endl;

	avl2.add(.1, 1);
	avl2.add(.35, 1);
	avl2.add(.05, 1);
	avl2.add(.1, 1);
	avl2.add(.15, 1);
	avl2.add(.05, 1);
	avl2.add(.2, 1);
	cout << avl2.median() << endl;

	return 0;
}