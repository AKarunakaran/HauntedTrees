/******************************************
 * Tree Functinality Test File			  *
 * Author: Aman Karunakaran				  *
 * Last Modified: 6/7/17				  *
 ******************************************/
//#include "../hauntedTrees.h"
#include "../hTreeArray.h"
#include "../basicTree.h"
#include "../avl.h"
#include "../rbTree.h"
#include <iostream>
using namespace std;

int main() {
	// AVL/haunted comparison
	Tree<int> t;
	t.add(3, 1);
	t.add(6, 1);
	t.add(7, 1);
	t.add(1, 1);
	t.add(2, 1);
	t.add(3, 1);
	t.add(10, 1);
	t.add(0, 1);
	t.print();
	cout << endl;
	
	HauntedTree<int, Tree<int> > h(&t);
	AVL<int> avl;
	h.add(0, 1);
	avl.add(0, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(3, 1);
	avl.add(3, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(10, 1);
	avl.add(10, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(2, 1);
	avl.add(2, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(1, 1);
	avl.add(1, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(3, 1);
	avl.add(3, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(7, 1);
	avl.add(7, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;
	h.add(6, 1);
	avl.add(6, 1);
	h.print();
	cout << h.median() << " " << avl.median() << endl;

	
	/*AVL<int> avl2;
	avl2.add(1, 1);
	avl2.add(1, .5);
	avl2.add(1, 3);
	avl2.add(5, 1);
	avl2.add(3, 1);
	avl2.add(3, .75);
	avl2.print_diagram();

	HauntedTree<int, AVL<int>> h1(&avl2), h2(&avl2);
	h1.add(1, .5);
	h1.print();
	h2.add(1, 1);
	h2.add(5, 1);
	h2.add(3, 1);
	h1.add(3, .75);
	h1.print();
	h2.print();
	h1.treeUnion(h2);
	h1.print();*/

	/*//Red-Black test
	RedBlack<int> rb, rb2;
	rb.add(20, 1);
	rb.add(19, 1);
	rb.print();
	//cout << rb.median() << endl;

	rb2.add(20, 1);
	rb2.print();
	
	//cout << rb2.median() << endl;

	rb.treeUnion(&rb2);
	rb.print();*/
	

	return 0;
}