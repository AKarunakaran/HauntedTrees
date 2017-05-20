/******************************************
 * Haunted Tree Header File				  *
 * Author: Aman Karunakaran				  *
 * Last Modified: 5/14/17				  *
 ******************************************/
#include <iostream>

template <typename T, typename tree>
class HauntedTree;

template <typename T>
class Tree {
	friend class HauntedTree<T, Tree<T> >;
	public:
		template <typename D>
		struct Node {
			D datum;
			double weight;
			double leftSubtreeSize;
			double rightSubtreeSize;
			Node* parent;
			Node* left;
			Node* right;
		};

		Tree() {
			root = 0;
		}

		~Tree() {
			Node<T>* itr = this->root;
			while(itr) {
				if(itr->left) {
					itr = itr->left;
					continue;
				}
				if(itr->right) {
					itr = itr->right;
					continue;
				}
				Node<T>* toDel = itr;
				itr = itr->parent;
				if(itr->right == toDel) itr->right = 0;
				else itr->left = 0;
				delete toDel;
			}
		}

		void add(const T& elt) {
			Node<T>* itr = root;
			bool left;
			if(!itr) {
				root = new Node<T>{elt, 0, 0, 0, 0, 0};
				return;
			}
			if(itr->datum > elt) left = true;
			else left = false;
			while((left && itr->left) || (!left && itr->right)) {
				if(left) itr = itr->left;
				else itr = itr->right;
				if(itr->datum > elt) left = true;
				else left = false;
			}
			if(left) itr->left = new Node<T>{elt, 0, 0, itr, 0, 0};
			else itr->right = new Node<T>{elt, 0, 0, itr, 0, 0};
			fixUp(itr);
		}

		void print() {
			print_h(this->root, 0);
			std::cout << std::endl;
			print_h(this->root, 1);
			std::cout << std::endl;
			print_h(this->root, 2);
			std::cout << std::endl;
		}

		int size() {
			return 1 + this->root->rightSubtreeSize + this->root->leftSubtreeSize;
		}

		T& median() {
			Node<T>* itr = root;
	        double sum = itr->leftSubtreeSize + itr->rightSubtreeSize + (itr->weight)*(itr->datum);
	        while(leftSubtreeSize >= sum/2 || rightSubtreeSize > sum/2) {
	            if(leftSubtreeSize < sum/2) itr = itr->right;
	            else itr = itr->left;
	            sum = itr->leftSubtreeSize + itr->rightSubtreeSize + (itr->weight)*(itr->datum);
	        }
        	return itr->datum;
		}

	private:
		Node<T>* root;

		void fixUp(Node<T>* node) {
			if(node->left) node->leftSubtreeSize = 1 + node->left->leftSubtreeSize + node->left->rightSubtreeSize;
			if(node->right) node->rightSubtreeSize = 1 + node->right->leftSubtreeSize + node->right->rightSubtreeSize;
			if(node == root) return;
			else fixUp(node->parent);
		}

		void print_h(Node<T>* n, int t) {
			if(!n) return;
			if(t == 0) std::cout << n->datum << " ";
			print_h(n->left, t);
			if(t == 1) std::cout << n->datum << " ";
			print_h(n->right, t);
			if(t == 2) std::cout << n->datum << " ";
		}
};

template <typename T, typename tree>
class HauntedTree {
	public:
		template <typename D>
		struct Node {
			D datum;
			int leftSubtreeSize;
			int rightSubtreeSize;
			Node* parent;
			Node* left;
			Node* right;
			bool ghost;
		};

		HauntedTree(tree* super) {
			root = 0;
			superTree = super;
		}

		~HauntedTree() {
			Node<T>* itr = this->root;
			while(itr) {
				if(itr->left) {
					itr = itr->left;
					continue;
				}
				if(itr->right) {
					itr = itr->right;
					continue;
				}
				Node<T>* toDel = itr;
				itr = itr->parent;
				if(itr->right == toDel) itr->right = 0;
				else itr->left = 0;
				delete toDel;
			}
		}

		int size() {
			return this->root->leftSubtreeSize + this->root->rightSubtreeSize + (1-this->root->ghost);
		}

		void add(const T& elt) {
			auto n = this->superTree->root;
			while(n && n->datum != elt) {
				if(n->datum > elt) n = n->left;
				else n = n->right;
			}
			if(!n) return;
			Node<T> ghost;
			ghost.leftSubtreeSize = ghost.rightSubtreeSize = 0;
			ghost.left = ghost.right = 0;
			ghost.ghost = true;
			if(!this->root) {
				Node<T>* toAdd = new Node<T>;
				*toAdd = ghost;
				toAdd->parent = 0;
				this->root = toAdd;
			}
			Node<T>* itr = this->root;
			n = this->superTree->root;
			while(n && n->datum != elt) {
				if(n->datum > elt) {
					n = n->left;
					if(itr->left) {
						itr = itr->left;
					} else {
						Node<T>* toAdd = new Node<T>;
						*toAdd = ghost;
						toAdd->parent = itr;
						itr->left = toAdd;
						itr = itr->left;
					}
				} else {
					n = n->right;
					if(itr->right) {
						itr = itr->right;
					} else {
						Node<T>* toAdd = new Node<T>;
						*toAdd = ghost;
						toAdd->parent = itr;
						itr->right = toAdd;
						itr = itr->right;
					}
				}
			}
			itr->ghost = 0;
			itr->datum = elt;
			if(itr->parent) fixUp(itr->parent);
		}

		//Merges two haunted trees. Destroys other in the process.
		void treeUnion(HauntedTree<T, tree>* other) {
			Node<T> *thisItr = this->root, *otherItr = other->root;
			union_h(thisItr, otherItr, 0, false);
			delete other;
		}

		T& median() {
			return findNth(this->root, (this->size()-1)/2);
		}

		void print() {
			print_h(this->root, 0);
			std::cout << std::endl;
			print_h(this->root, 1);
			std::cout << std::endl;
			print_h(this->root, 2);
			std::cout << std::endl;
		}

	private:
		Node<T>* root;
		tree* superTree;

		//Find nth element, 0-indexed
		T& findNth(Node<T>* root, int n) {
			if(n < root->leftSubtreeSize) {
				return findNth(root->left, n);
			} else if (n == root->leftSubtreeSize && !root->ghost) {
				return root->datum;
			} else {
				return findNth(root->right, n-(root->leftSubtreeSize)-(1-(root->ghost)));
			}
		}

		void fixUp(Node<T>* node) {
			if(node->left) node->leftSubtreeSize = (1 - node->left->ghost) + node->left->leftSubtreeSize + node->left->rightSubtreeSize;
			if(node->right) node->rightSubtreeSize = (1 - node->right->ghost) + node->right->leftSubtreeSize + node->right->rightSubtreeSize;
			if(node == root) return;
			else fixUp(node->parent);
		}

		void union_h(Node<T>* thisItr, Node<T>* otherItr, Node<T>* thisParent, bool left) {
			if(!otherItr) return;
			if(!thisItr) {
				if(left) {
					thisParent->left = otherItr;
					otherItr->parent->left = 0;
					otherItr->parent = thisParent;
				} else {
					thisParent->right = otherItr;
					otherItr->parent->right = 0;
					otherItr->parent = thisParent;
				}
				return;
			}
			if(thisItr->ghost && !otherItr->ghost) {
				thisItr->datum = otherItr->datum;
				thisItr->ghost = false;
			}
			union_h(thisItr->left, otherItr->left, thisItr, true);
			union_h(thisItr->right, otherItr->right, thisItr, false);
		}

		void print_h(Node<T>* n, int t) {
			if(!n) return;
			if(t == 0) {
				if(n->ghost) std::cout << "g ";
				else std::cout << n->datum << " ";
			}
			print_h(n->left, t);
			if(t == 1) {
				if(n->ghost) std::cout << "g ";
				else std::cout << n->datum << " ";
			}
			print_h(n->right, t);
			if(t == 2) {
				if(n->ghost) std::cout << "g ";
				else std::cout << n->datum << " ";
			}
		}
};
