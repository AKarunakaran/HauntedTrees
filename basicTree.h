/******************************************
 * Basic Tree Header File				  *
 * Authors: Aman Karunakaran			  *
 *			Kurt Ayalp					  *
 * Last Modified: 5/24/17				  *
 ******************************************/
#include <iostream>

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
			destroy_node(root);
		}

		void add(const T& elt, double weight) {
			Node<T>* itr = root;
			bool left;
			if(!itr) {
				root = new Node<T>{elt, weight, 0, 0, 0, 0, 0};
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
			if(left) itr->left = new Node<T>{elt, weight, 0, 0, itr, 0, 0};
			else itr->right = new Node<T>{elt, weight, 0, 0, itr, 0, 0};
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

	private:
		Node<T>* root;

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

		void print_h(Node<T>* n, int t) {
			if(!n) return;
			if(t == 0) std::cout << n->datum << " ";
			print_h(n->left, t);
			if(t == 1) std::cout << n->datum << " ";
			print_h(n->right, t);
			if(t == 2) std::cout << n->datum << " ";
		}

		void destroy_node(Node<T>* node) {
			if (node == nullptr)
	            return;
	        destroy_node(node->left);
	        destroy_node(node->right);
	        delete node;
		}
};