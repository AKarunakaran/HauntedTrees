/******************************************
 * AVL Tree Header File                   *
 * Authors: Aman Karunakaran              *
 *          Kurt Ayalp                    *
 * Last Modified: 5/24/17                 *
 ******************************************/
#include <iostream>

// probably useful
int max(int x, int y) {
    return x > y ? x : y;
}

template <typename T>
class AVL {
    friend class HauntedTree<T, AVL<T> >;
    public:
        template <typename D>
        struct Node {
            D datum;
            double weight;
            double leftSubtreeSize;
            double rightSubtreeSize;
            int height;
            Node* left;
            Node* right;
            int left_height() {
                return left ? left->height : 0;
            }
            int right_height() {
                return right ? right->height : 0;
            }
            int balance() {
                return left_height() - right_height();
            }
            // when the height of its children change, call this function to
            // recalculate the height of this node, the parent
            void fix_height() {
                height = 1 + max(left_height(), right_height());
            }
        };

        void add(T datum, double weight) {
            root = insert_node(root, datum, weight);
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

        void treeUnion(AVL<T>* other) {
            this->merge(other, other->root);
        }

        void merge(AVL<T>* other, Node<T>* node) {
            if(node->left) merge(other, node->left);
            if(node->right) merge(other, node->right);
            this->add(node->datum, node->weight);
            other->destroy_node(node);
        }

        ~AVL() {
            destroy_node(root);
        }

    private:
        Node<T>* root = nullptr;
        size_t size = 0;

        // insert_node returns the increase in height for the provided tree
        Node<T>* insert_node(Node<T>* node, T& datum, double weight) {
            if (node == nullptr) {
                // at a leaf position in the tree, so create a new node
                return new Node<T>{ datum, weight, 0, 0, 1, nullptr, nullptr }; // it has height 1
            }
            if (datum < node->datum) {
                node->left = insert_node(node->left, datum, weight);
                node->fix_height(); // remember to fix the height of a node after modifying its children
                node->leftSubtreeSize = node->left->leftSubtreeSize + node->left->rightSubtreeSize + node->left->weight;
                if(node->balance() > 1) {
                    if(node->left->balance() == 1) {
                        return rotate_right(node);
                    } else {
                        node->left = rotate_left(node->left);
                        node->fix_height();
                        return rotate_right(node);
                    }
                }
            } else {
                node->right = insert_node(node->right, datum, weight);
                node->fix_height(); // remember to fix the height of a node after modifying its children
                node->rightSubtreeSize = node->right->leftSubtreeSize + node->right->rightSubtreeSize + node->right->weight;
                if(node->balance() < -1) {
                    if(node->right->balance() == -1) {
                        return rotate_left(node);
                    } else {
                        node->right = rotate_right(node->right);
                        node->fix_height();
                        return rotate_left(node);
                    }
                }
            }
            return node;
        }

        // these return the new top node after rotation
        Node<T>* rotate_left(Node<T>* node) {
            Node<T>* newParent = node->right;
            if(root == node)
                root = newParent;
            node->right = newParent->left;
            newParent->left = node;
            node->rightSubtreeSize = newParent->leftSubtreeSize;
            newParent->leftSubtreeSize = node->leftSubtreeSize + node->rightSubtreeSize + node->weight;
            node->fix_height();
            newParent->fix_height();
            return newParent;
        }
        Node<T>* rotate_right(Node<T>* node) {
            Node<T>* newParent = node->left;
            if(root == node)
                root = newParent;
            node->left = newParent->right;
            newParent->right = node;
            node->leftSubtreeSize = newParent->rightSubtreeSize;
            newParent->rightSubtreeSize = node->leftSubtreeSize + node->rightSubtreeSize + node->weight;
            node->fix_height();
            newParent->fix_height();
            return newParent;
        }
        void destroy_node(Node<T>* node) {
            if (node == nullptr) {
                return;
            }
            destroy_node(node->left);
            destroy_node(node->right);
            delete node;
        }
};