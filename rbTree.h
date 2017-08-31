/******************************************
 * Red-Black Tree Header File             *
 * Authors: Aman Karunakaran              *
 *          Kurt Ayalp                    *
 * Last Modified: 6/7/17                  *
 ******************************************/
#include <iostream>
#include <vector>

#define RED true
#define BLACK false

template <typename T>
class RedBlack {
    friend class HauntedTree<T, RedBlack<T> >;
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
            bool color;
        };

        void add(T datum, double weight) {
            Node<T> *par = nullptr, *itr = root;
            while(itr) {
                par = itr;
                if(datum == itr->datum)
                    break;
                itr = (datum < itr->datum) ? itr->left : itr->right;
            }
            if(!par || !itr) {
                Node<T>* toAdd = new Node<T>;
                toAdd->datum = datum;
                toAdd->weights.push_back(weight);
                toAdd->totalWeight = weight;
                toAdd->parent = par;
                toAdd->left = toAdd->right = 0;
                toAdd->color = RED;
                if(par) {
                    if(datum < par->datum)
                        par->left = toAdd;
                    else
                        par->right = toAdd;
                    fixUpWeight(par);
                    fixUp(toAdd);
                } else {
                    root = toAdd;
                    root->color = BLACK;
                }
            } else {
                itr->weights.push_back(weight);
                itr->totalWeight += weight;
                fixUpWeight(par);
            }
        }

        T& median() {
            Node<T>* itr = root;
            double sum = itr->leftSubtreeSize + itr->rightSubtreeSize + itr->totalWeight;
            double leftElts = 0, rightElts = 0;
            while(leftElts + itr->leftSubtreeSize > sum/2 || rightElts + itr->rightSubtreeSize > sum/2) {
                if((leftElts + itr->leftSubtreeSize) < sum/2) {
                    leftElts += itr->totalWeight + itr->leftSubtreeSize;
                    itr = itr->right;
                }
                else {
                    rightElts += itr->totalWeight + itr->rightSubtreeSize;
                    itr = itr->left;
                }
            }
            return itr->datum;
        }

        void treeUnion(RedBlack<T>* other) {
            this->merge(other, other->root);
        }

        void merge(RedBlack<T>* other, Node<T>* node) {
            if(node->left) merge(other, node->left);
            if(node->right) merge(other, node->right);
            for(int i = 0; i < node->weights.size(); ++i) {
                this->add(node->datum, node->weights[i]);
            }
        }

        ~RedBlack() {
            destroy_node(root);
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
        Node<T>* root = nullptr;

        void fixUp(Node<T>* node) {
            while(node != root && node->parent->color == RED) {
                if(node->parent == node->parent->parent->left) {
                    Node<T>* uncle = node->parent->parent->right;
                    if(uncle && uncle->color == RED) {
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        node = node->parent->parent;
                        node->color = RED;
                    } else if(node == node->parent->right) {
                        node = node->parent;
                        rotateLeft(node);
                    } else {
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        rotateRight(node->parent->parent);
                    }
                } else {
                    Node<T>* uncle = node->parent->parent->left;
                    if(uncle && uncle->color == RED) {
                        node->parent->color = BLACK;
                        uncle->color = BLACK;
                        node = node->parent->parent;
                        node->color = RED;
                    } else if(node == node->parent->left) {
                        node = node->parent;
                        rotateRight(node);
                    } else {
                        node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        rotateLeft(node->parent->parent);
                    }
                }
            }
            root->color = BLACK;
        }

        void fixUpWeight(Node<T>* node) {
            while(node) {
                if(node->left)
                    node->leftSubtreeSize = node->left->totalWeight + node->left->leftSubtreeSize + node->left->rightSubtreeSize;
                if(node->right)
                    node->rightSubtreeSize = node->right->totalWeight + node->right->leftSubtreeSize + node->right->rightSubtreeSize;
                node = node->parent;
            }
        }

        void rotateLeft(Node<T>* node) {
            Node<T>* newParent = node->right;
            node->right = newParent->left;
            if(newParent->left)
                newParent->left->parent = node;
            newParent->parent = node->parent;
            if(!node->parent)
                root = newParent;
            else if (node == node->parent->left)
                node->parent->left = newParent;
            else
                node->parent->right = newParent;
            newParent->left = node;
            node->parent = newParent;
            node->rightSubtreeSize = newParent->leftSubtreeSize;
            newParent->leftSubtreeSize = node->leftSubtreeSize + node->rightSubtreeSize + node->totalWeight;
        }

        void rotateRight(Node<T>* node) {
            Node<T>* newParent = node->left;
            node->left = newParent->right;
            if(newParent->right)
                newParent->right->parent = node;
            newParent->parent = node->parent;
            if(!node->parent)
                root = newParent;
            else if (node == node->parent->left)
                node->parent->left = newParent;
            else
                node->parent->right = newParent;
            newParent->right = node;
            node->parent = newParent;
            node->leftSubtreeSize = newParent->rightSubtreeSize;
            newParent->rightSubtreeSize = node->leftSubtreeSize + node->rightSubtreeSize + node->totalWeight;
        }

        void destroy_node(Node<T>* node) {
            if (node == nullptr) {
                return;
            }
            destroy_node(node->left);
            destroy_node(node->right);
            delete node;
        }

        void print_h(Node<T>* n, int t) {
            if(!n) return;
            if(t == 0) {
                std::cout << n->datum << ((n->color == RED) ? "r" : "b") << ":{";
                for(int i = 0; i < n->weights.size(); ++i) {
                    std::cout << n->weights[i] << " ";
                }
                std::cout << "} ";
            }
            print_h(n->left, t);
            if(t == 1) {
                std::cout << n->datum << ((n->color == RED) ? "r" : "b") << ":{";
                for(int i = 0; i < n->weights.size(); ++i) {
                    std::cout << n->weights[i] << " ";
                }
                std::cout << "} ";
            }
            print_h(n->right, t);
            if(t == 2) {
                std::cout << n->datum << ((n->color == RED) ? "r" : "b") << ":{";
                for(int i = 0; i < n->weights.size(); ++i) {
                    std::cout << n->weights[i] << " ";
                }
                std::cout << "} ";
            }
        }
};