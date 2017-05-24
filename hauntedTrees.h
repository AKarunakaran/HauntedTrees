/******************************************
 * Haunted Tree Header File				  *
 * Authors: Aman Karunakaran			  *
 *			Kurt Ayalp					  *
 * Last Modified: 5/24/17				  *
 ******************************************/
#include <iostream>

template <typename T, typename tree>
class HauntedTree {
	public:
		template <typename D>
		struct Node {
			D datum;
			double weight;
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
			destroy_node(root);
		}

		void add(const T& elt) {
			auto n = this->superTree->root;
			while(n && n->datum != elt) {
				if(n->datum > elt)
					n = n->left;
				else
					n = n->right;
			}
			if(!n)
				return;
			Node<T> ghost;
			ghost.leftSubtreeSize = ghost.rightSubtreeSize = 0;
			ghost.left = ghost.right = 0;
			ghost.weight = 0;
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
			//TODO: figure out what to do if there are duplicates in the base tree.
			if(!itr->ghost) {
				//
			}
			itr->ghost = 0;
			itr->datum = elt;
			itr->weight = n->weight;
			if(itr->parent)
				fixUp(itr->parent);
		}

		//Merges two haunted trees. Destroys other in the process.
		void treeUnion(HauntedTree<T, tree>* other) {
			Node<T> *thisItr = this->root, *otherItr = other->root;
			union_h(thisItr, otherItr, 0, false);
			delete other;
		}

		T& median() {
			Node<T>* itr = root;
		    double sum = itr->leftSubtreeSize + itr->rightSubtreeSize + itr->weight;
		    double leftElts = 0, rightElts = 0;
		    while(leftElts + itr->leftSubtreeSize >= sum/2 || rightElts + itr->rightSubtreeSize > sum/2) {
		    	//std::cout << sum << " " << leftElts + itr->leftSubtreeSize << " " << rightElts + itr->rightSubtreeSize << std::endl;
		        if((leftElts + itr->leftSubtreeSize) < sum/2) {
		            leftElts += itr->weight + itr->leftSubtreeSize;
		            itr = itr->right;
		        }
		        else {
		            rightElts += itr->weight + itr->rightSubtreeSize;
		            itr = itr->left;
		        }
		    }
		    //std::cout << sum << " " << leftElts + itr->leftSubtreeSize << " " << rightElts + itr->rightSubtreeSize << std::endl;
		    return itr->datum;
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

		void fixUp(Node<T>* node) {
			if(node->left)
				node->leftSubtreeSize = node->left->weight + node->left->leftSubtreeSize + node->left->rightSubtreeSize;
			if(node->right)
				node->rightSubtreeSize = node->right->weight + node->right->leftSubtreeSize + node->right->rightSubtreeSize;
			if(node == root)
				return;
			else
				fixUp(node->parent);
		}

		void union_h(Node<T>* thisItr, Node<T>* otherItr, Node<T>* thisParent, bool left) {
			if(!otherItr)
				return;
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
			if(!n)
				return;
			if(t == 0)
				std::cout << ((n->ghost) ? "g" : n->datum) << " ";
			print_h(n->left, t);
			if(t == 1)
				std::cout << ((n->ghost) ? "g" : n->datum) << " ";
			print_h(n->right, t);
			if(t == 2) 
				std::cout << ((n->ghost) ? "g" : n->datum) << " ";
		}

		void destroy_node(Node<T>* node) {
			if (node == nullptr)
	            return;
	        destroy_node(node->left);
	        destroy_node(node->right);
	        delete node;
		}
};
