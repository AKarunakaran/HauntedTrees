/******************************************
 * Haunted Tree Header File				  *
 * Authors: Aman Karunakaran			  *
 *			Kurt Ayalp					  *
 * Last Modified: 6/7/17				  *
 ******************************************/
#include <iostream>
#include <vector>

template <typename T, typename tree>
class HauntedTree {
	public:
		template <typename D>
		struct Node {
			D datum;
			std::vector<std::pair<double, bool>> weights;
			double totalWeight;
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

		void add(const T& elt, double weight) {
			if(!this->root) {
				Node<T>* toAdd = new Node<T>{0, std::vector<std::pair<double, bool>>{}, 0, 0, 0, 0, 0, 0, true};
				this->root = toAdd;
			}
			auto superItr = this->superTree->root;
			Node<T>* itr = this->root;
			while(superItr->datum != elt) {
				if(superItr->datum > elt) {
					superItr = superItr->left;
					if(itr->left) {
						itr = itr->left;
					} else {
						Node<T>* toAdd = new Node<T>{0, std::vector<std::pair<double, bool>>{}, 0, 0, 0, itr, 0, 0, true};
						itr->left = toAdd;
						itr = itr->left;
					}
				} else {
					superItr = superItr->right;
					if(itr->right) {
						itr = itr->right;
					} else {
						Node<T>* toAdd = new Node<T>{0, std::vector<std::pair<double, bool>>{}, 0, 0, 0, itr, 0, 0, true};
						itr->right = toAdd;
						itr = itr->right;
					}
				}
			}
			if(!itr->ghost) {
				auto& itrWeights = itr->weights;
				for(int i = 0; i < itrWeights.size(); ++i) {
					if(!itrWeights[i].second && itrWeights[i].first == weight) {
						itrWeights[i].second = true;
						break;
					}
				}
			} else {
				itr->ghost = 0;
				itr->datum = elt;
				auto& theseWeights = itr->weights;
				auto& superWeights = superItr->weights;
				theseWeights.resize(superWeights.size());
				bool added = false;
				int i = 0;
				while(superWeights[i] != weight) {
					theseWeights[i].first = superWeights[i];
					theseWeights[i].second = false;
					++i;
				}
				theseWeights[i].first = superWeights[i];
				theseWeights[i].second = true;
				++i;
				while(i < theseWeights.size()) {
					theseWeights[i].first = superWeights[i];
					theseWeights[i].second = false;
					++i;
				}
				/*
				for(int i = 0; i < theseWeights.size(); ++i) {
					theseWeights[i].first = superWeights[i];
					if(!added && superWeights[i] == weight) {
						added = true;
						theseWeights[i].second = true;
					} else {
						theseWeights[i].second = false;
					}
				}*/
			}
			itr->totalWeight += weight;
			fixUp(itr->parent);
		}

		//Merges two haunted trees. Destroys other in the process.
		void treeUnion(HauntedTree<T, tree>* other) {
			Node<T> *thisItr = this->root, *otherItr = other->root;
			union_h(thisItr, otherItr, 0, false);
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
			while(node) {
				if(node->left)
					node->leftSubtreeSize = node->left->totalWeight + node->left->leftSubtreeSize + node->left->rightSubtreeSize;
				if(node->right)
					node->rightSubtreeSize = node->right->totalWeight + node->right->leftSubtreeSize + node->right->rightSubtreeSize;
				node = node->parent;
			}
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

			if(!otherItr->ghost) {
				if(thisItr->ghost) {
					thisItr->datum = otherItr->datum;
					thisItr->weights = otherItr->weights;
					thisItr->ghost = false;
				} else {
					auto& otherWeights = otherItr->weights;
					auto& theseWeights = thisItr->weights;
					for(int i = 0; i < otherWeights.size(); ++i) {
						if(otherWeights[i].second && !theseWeights[i].second) {
							thisItr->totalWeight += theseWeights[i].first;
							theseWeights[i].second = true;
						}
					}
				}
			}

			union_h(thisItr->left, otherItr->left, thisItr, true);
			union_h(thisItr->right, otherItr->right, thisItr, false);
		}

		void print_h(Node<T>* n, int t) {
			if(!n)
				return;
			if(t == 0) {
				if(n->ghost) {
					std::cout << "g ";
				} else {
					std::cout << n->datum << ":{";
					for(int i = 0; i < n->weights.size(); ++i) {
						if(n->weights[i].second) std::cout << n->weights[i].first << " ";
					}
					std::cout << "} ";
				}
			}
			print_h(n->left, t);
			if(t == 1) {
				if(n->ghost) {
					std::cout << "g ";
				} else {
					std::cout << n->datum << ":{";
					for(int i = 0; i < n->weights.size(); ++i) {
						if(n->weights[i].second) std::cout << n->weights[i].first << " ";
					}
					std::cout << "} ";
				}
			}
			print_h(n->right, t);
			if(t == 2)  {
				if(n->ghost) {
					std::cout << "g ";
				} else {
					std::cout << n->datum << ":{";
					for(int i = 0; i < n->weights.size(); ++i) {
						if(n->weights[i].second) std::cout << n->weights[i].first << " ";
					}
					std::cout << "} ";
				}
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
