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
			double totalWeight;
			double leftSubtreeSize;
			double rightSubtreeSize;
			D datum;
			std::vector<std::pair<double, bool>> weights;
			bool ghost;
		};

		HauntedTree(tree* super) {
			superTree = super;
		}

		void add(const T& elt, double weight) {
			int height = 1;
			std::vector<bool> rights;
			auto itr = superTree->root;
			while(itr->datum != elt) {
				if(itr->datum > elt) {
					itr = itr->left;
					rights.push_back(0);
				} else {
					itr = itr->right;
					rights.push_back(1);
				}
				++height;
			}
			if(data.size() < ((1 << height)-1)) {
				data.resize((1 << height)-1, {0, 0, 0, 0, std::vector<std::pair<double, bool>>{}, 0});
			}
			int k = 0;
			for(int i = 0; i < rights.size(); ++i) {
				if(!data[k].ghost && data[k].totalWeight == 0)
					data[k].ghost = 1;
				k = 2*k + 1 + rights[i];
			}
			if(data[k].totalWeight == 0) {
				data[k].datum = elt;
				data[k].ghost = 0;
			}
			data[k].totalWeight += weight;
			if(data[k].weights.size() == 0) {
				data[k].weights.resize(itr->weights.size(), {0, 0});
				for(int i = 0; i < data[k].weights.size(); ++i) {
					data[k].weights[i].first = itr->weights[i];
				}
			}
			for(int i = 0; i < data[k].weights.size(); ++i) {
				if(data[k].weights[i].first == weight) {
					data[k].weights[i].second = true;
					break;
				}
			}

			k = (k-1)/2;
			while(k >= 0) {
				data[k].leftSubtreeSize = data[2*k+1].totalWeight + data[2*k+1].leftSubtreeSize + data[2*k+1].rightSubtreeSize;
				data[k].rightSubtreeSize = data[2*k+2].totalWeight + data[2*k+2].leftSubtreeSize + data[2*k+2].rightSubtreeSize;
				if(k == 0)
					break;
				k = (k-1)/2;
			}
		}

		void treeUnion(HauntedTree<T, tree>& other) {
			if(other.data.size() > data.size()) {
				data.resize(other.data.size(), {0, 0, 0, 0, std::vector<std::pair<double, bool>>{}, 0});
			}
			for(int i = other.data.size()-1; i >= 0; --i) {
				if(other.data[i].totalWeight != 0) {
					if(data[i].ghost || data[i].totalWeight == 0) {
						data[i].datum = other.data[i].datum;
						data[i].ghost = 0;
						data[i].weights.resize(other.data[i].weights.size());
						for(int j = 0; j < data[i].weights.size(); ++j) {
							data[i].weights[j] = other.data[i].weights[j];
						}
					} else {
						for(int j = 0; j < data[i].weights.size(); ++j) {
							data[i].weights[j].second |= other.data[i].weights[j].second;
						}
					}
					data[i].totalWeight += other.data[i].totalWeight;
				} else if(other.data[i].ghost && data[i].totalWeight == 0 && !data[i].ghost) {
					data[i].ghost = 1;
				}
				if(2*i+2 < data.size()) {
					data[i].leftSubtreeSize = data[2*i+1].totalWeight + data[2*i+1].leftSubtreeSize + data[2*i+1].rightSubtreeSize;
					data[i].rightSubtreeSize = data[2*i+2].totalWeight + data[2*i+2].leftSubtreeSize + data[2*i+2].rightSubtreeSize;
				}
			}
		}

		T& median() {
			int k = 0;
		    double sum = data[k].leftSubtreeSize + data[k].rightSubtreeSize + data[k].totalWeight;
		    double leftElts = 0, rightElts = 0;
		    while(leftElts + data[k].leftSubtreeSize > sum/2 || rightElts + data[k].rightSubtreeSize > sum/2) {
		    	//std::cout << sum << " " << leftElts + data[k].leftSubtreeSize << " " << rightElts + data[k].rightSubtreeSize << std::endl;
		        if((leftElts + data[k].leftSubtreeSize) < sum/2) {
		            leftElts += data[k].totalWeight + data[k].leftSubtreeSize;
		            k = 2*k+2;
		        } else {
		            rightElts += data[k].totalWeight + data[k].rightSubtreeSize;
		            k = 2*k+1;
		        }
		    }
		    //std::cout << sum << " " << leftElts + data[k].leftSubtreeSize << " " << rightElts + data[k].rightSubtreeSize << std::endl;
		    return data[k].datum;
		}

		void print() {
			for(int i = 0; i < data.size(); ++i) {
				if(data[i].ghost) {
					std::cout << "g ";
				} else {
					if(data[i].totalWeight) {
						std::cout << data[i].datum << ":{";
						for(int j = 0; j < data[i].weights.size(); ++j) {
							if(data[i].weights[j].second) std::cout << data[i].weights[j].first << ", ";
						}
						std::cout << "} ";
					} else {
						std::cout << "- ";
					}
				}
			}
			std::cout << std::endl;
		}


	private:
		tree* superTree;
		std::vector<Node<T>> data;

};
