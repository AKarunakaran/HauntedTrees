/******************************************
 * Basic Tree Header File				  *
 * Authors: Aman Karunakaran			  *
 *			Kurt Ayalp					  *
 * Last Modified: 6/7/17				  *
 ******************************************/
#include <iostream>
#include <vector>

template <typename T>
class Tree {
	friend class HauntedTree<T, Tree<T> >;
	public:
		template <typename D>
		struct Node {
			D datum;
			std::vector<double> weights;
			double totalWeight;
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
				root = new Node<T>;
				root->datum = elt;
				root->weights.push_back(weight);
				root->totalWeight = weight;
				return;
			}
			if(itr->datum == elt) {
				itr->weights.push_back(weight);
				itr->totalWeight += weight;
				return;
			}
			if(itr->datum > elt)
				left = true;
			else
				left = false;
			while((left && itr->left) || (!left && itr->right)) {
				if(left)
					itr = itr->left;
				else
					itr = itr->right;
				if(itr->datum > elt) {
					left = true;
					if(itr->left && itr->left->datum == elt) {
						break;
					}
				}
				else {
					left = false;
					if(itr->right && itr->right->datum == elt) {
						break;
					}
				}
			}
			if(left && itr->left && itr->left->datum == elt) {
				itr = itr->left;
				itr->weights.push_back(weight);
				itr->totalWeight += weight;
			} else if(!left && itr->right && itr->right->datum == elt) {
				itr = itr->right;
				itr->weights.push_back(weight);
				itr->totalWeight += weight;
			} else {
				Node<T>* toAdd = new Node<T>;
				toAdd->datum = elt;
				toAdd->totalWeight = weight;
				toAdd->weights.push_back(weight);
				toAdd->parent = itr;
				if(left)
					itr->left = toAdd;
				else
					itr->right = toAdd;
			}
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
			return this->root->totalWeight + this->root->rightSubtreeSize + this->root->leftSubtreeSize;
		}

		T& median() {
			Node<T>* itr = root;
		    double sum = itr->leftSubtreeSize + itr->rightSubtreeSize + itr->totalWeight;
		    double leftElts = 0, rightElts = 0;
		    while(leftElts + itr->leftSubtreeSize > sum/2 || rightElts + itr->rightSubtreeSize > sum/2) {
		    	//std::cout << sum << " " << leftElts + itr->leftSubtreeSize << " " << rightElts + itr->rightSubtreeSize << std::endl;
		        if((leftElts + itr->leftSubtreeSize) < sum/2) {
		            leftElts += itr->totalWeight + itr->leftSubtreeSize;
		            itr = itr->right;
		        }
		        else {
		            rightElts += itr->totalWeight + itr->rightSubtreeSize;
		            itr = itr->left;
		        }
		    }
		    //std::cout << sum << " " << leftElts + itr->leftSubtreeSize << " " << rightElts + itr->rightSubtreeSize << std::endl;
		    return itr->datum;
		}

	private:
		Node<T>* root;

		void fixUp(Node<T>* node) {
			while(node) {
				if(node->left)
					node->leftSubtreeSize = node->left->totalWeight + node->left->leftSubtreeSize + node->left->rightSubtreeSize;
				if(node->right)
					node->rightSubtreeSize = node->right->totalWeight + node->right->leftSubtreeSize + node->right->rightSubtreeSize;
				node = node->parent;
			}
		}

		void print_h(Node<T>* n, int t) {
			if(!n) return;
			if(t == 0) {
				std::cout << n->datum << ":{";
				for(int i = 0; i < n->weights.size(); ++i) {
					std::cout << n->weights[i] << " ";
				}
				std::cout << "} ";
			}
			print_h(n->left, t);
			if(t == 1) {
				std::cout << n->datum << ":{";
				for(int i = 0; i < n->weights.size(); ++i) {
					std::cout << n->weights[i] << " ";
				}
				std::cout << "} ";
			}
			print_h(n->right, t);
			if(t == 2)  {
				std::cout << n->datum << ":{";
				for(int i = 0; i < n->weights.size(); ++i) {
					std::cout << n->weights[i] << " ";
				}
				std::cout << "} ";
			}
		}

		void destroy_node(Node<T>* node) {
			if (node == nullptr)
	            return;
	        destroy_node(node->left);
	        destroy_node(node->right);
	        delete node;
		}
};