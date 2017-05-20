
#include <iostream>

// used for printing the trees
#include <vector>
#include <utility>
#include <functional>

// probably useful
int max(int x, int y) {
    return x > y ? x : y;
}

template <typename T>
class AVL {
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
    
    void insert(T& datum) {
        root = insert_node(root, datum);
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

    const Node<T>* search(T& datum, bool print_path = false) const {
        return search_node(root, datum, print_path);
    }

    void merge(AVL<T>& other, Node<T>* root) {
        if(root->left) merge(other, root->left);
        if(root->right) merge(other, root->right);
        this->insert(root->datum);
        other.destroy_node(root);
    }

    ~AVL() {
        destroy_node(root);
    }

private:
    Node<T>* root = nullptr;
    size_t size = 0;

    // insert_node returns the increase in height for the provided tree
    Node<T>* insert_node(Node<T>* node, T& datum) {
        if (node == nullptr) {
            // at a leaf position in the tree, so create a new node
            return new Node<T>{ datum, 1, nullptr, nullptr }; // it has height 1
        }
        if (datum < node->datum) {
            node->left = insert_node(node->left, datum);
            node->fix_height(); // remember to fix the height of a node after modifying its children
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
            node->right = insert_node(node->right, datum);
            node->fix_height(); // remember to fix the height of a node after modifying its children
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

    // search_node returns the node, or nullptr
    const Node<T>* search_node(const Node<T>* node, T& datum, bool print_path) const {
        if (node == nullptr) {
            if (print_path) {
                std::cout << "M" << std::endl;
            }
            return nullptr; // not found (no node here)
        }
        if (node->datum == datum) {
            // found the target
            if (print_path) {
                std::cout << "X" << std::endl;
            }
            return node;
        }
        if (datum < node->datum) {
            // left subtree, since smaller than current node
            if (print_path) {
                std::cout << "L";
            }
            return search_node(node->left, datum, print_path);
        } else {
            if (print_path) {
                std::cout << "R";
            }
            // right subtree, since larger than current node
            return search_node(node->right, datum, print_path);
        }
    }

    // these return the new top node after rotation
    Node<T>* rotate_left(Node<T>* node) {
        Node<T>* newParent = node->right;
        node->right = newParent->left;
        newParent->left = node;
        node->fix_height();
        newParent->fix_height();
        return newParent;
    }
    Node<T>* rotate_right(Node<T>* node) {
        Node<T>* newParent = node->left;
        node->left = newParent->right;
        newParent->right = node;
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