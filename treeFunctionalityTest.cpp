/******************************************
 * Tree Functinality Test File			  *
 * Author: Aman Karunakaran				  *
 * Last Modified: 5/24/17				  *
 ******************************************/
#include "hauntedTrees.h"
#include "basicTree.h"
#include "avl.h"
#include "rbTree.h"
#include <iostream>
using namespace std;

int main() {
	/*
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
	cout << h.median() << " " << avl.median() << endl;*/

	RedBlack<int> rb, rb2;
	rb.add(11, 1);
	rb.print();
	rb.add(2, 1);
	rb.print();
	rb.add(14, 1);
	rb.print();
	rb.add(1, 1);
	rb.print();
	rb.add(7, 1);
	rb.print();
	rb.add(15, 1);
	rb.print();
	rb.add(5, 1);
	rb.print();
	rb.add(8, 1);
	rb.print();
	rb.add(4, 1);
	rb.print();
	cout << rb.median() << endl;

	rb2.add(6, 1);
	rb2.print();
	rb2.add(6, 1);
	rb2.print();
	rb2.add(7, 1);
	rb2.print();
	
	cout << rb2.median() << endl;

	AVL<int> avl;

	avl.add(6, 1);
	avl.add(6, 1);
	avl.add(7, 1);

	cout << avl.median() << endl;

	return 0;
}