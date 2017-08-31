//
//  unimodalRegL1.cpp
//  AlgoResearch
//
//  Created by Kurt Ayalp on 6/22/17.
//  Copyright © 2017 Kurt Ayalp. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
#include <fstream>
//#include <Rcpp.h>

using namespace std;
//using namespace Rcpp;

// probably useful
int max(int x, int y) {
    return x > y ? x : y;
}

template <typename T>
class AVL {
    public:
        AVL() {
            root = nullptr;
        }

        template <typename D>
        struct Node {
            D datum;
            std::vector<double> weights;
            double totalWeight;
            double leftSubtreeSize;
            double rightSubtreeSize;
            //The next two variables will be updated similarly to the previous
            //two, and are used to calculate the error.
            //The insert/rotate functions have been slightly modified to update
            //these variables; see lines 451, 462-3, 477-8, 500, 502, 514, 516
            double leftSubtreeWY;
            double rightSubtreeWY;
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
            void fix_height() {
                height = 1 + max(left_height(), right_height());
            }
        };
    
        void add(T datum, double weight) {
            root = insert_node(root, datum, weight);
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
    
        double error(T med) {
            Node<T>* current = root;
            //each of these is the sum of W or WY, and the less than and greater
            //than are relative to the median
            double WYgreater = 0, WYless = 0, Wgreater = 0, Wless = 0;
            
            //if the node we're looking at is not equal to the median,
            //we can add data from current and the subtree that the median is
            //not in to the sum variables declared above
            while (current && current->datum != med) {
                double currentWY = current->datum * current->totalWeight;
                if (med < current->datum) {
                    WYgreater += current->rightSubtreeWY + currentWY;
                    Wgreater += current->rightSubtreeSize + current->totalWeight;
                    current = current->left;
                }
                else {
                    WYless += current->leftSubtreeWY + currentWY;
                    Wless += current->leftSubtreeSize + current->totalWeight;
                    current = current->right;
                }
            }
            
            //means we've found the median so just add the data from both subtrees
            if (current) {
                WYgreater += current->rightSubtreeWY;
                Wgreater += current->rightSubtreeSize;
                WYless += current->leftSubtreeWY;
                Wless += current->leftSubtreeSize;
            }
            
            return WYgreater - (med * Wgreater) + (med * Wless) - WYless;
        }
        
        /**************** <Printing function used for debugging> ****************/
        /*
        void print_diagram() const {
            struct pos {
                int depth;
                int parent_dir;
            };
            std::vector<std::pair<double, pos>> points;
            std::function<void(Node<T>*, pos p)> traverse_depth = [&](Node<T>* n, pos p) {
                if (n == nullptr) {
                    return;
                }
                traverse_depth(n->left, {p.depth+1, 1});
                points.push_back(std::pair<int, pos>({n->datum, p}));
                traverse_depth(n->right, {p.depth+1, -1});
            };
            traverse_depth(root, {0, 0});
            // points is now filled
            int width = 2 * (int)points.size();
            int height = 0;
            for (int i = 0; i < (int)points.size(); i++) {
                height = max(height, points[i].second.depth);
            }
            height *= 2;
            height++;
            // now, we can build the buffer:
            std::vector<std::vector<char>> buffer(width, std::vector<char>(height, ' '));
            // add the numbers
            for (int i = 0; i < (int)points.size(); i++) {
                int n = points[i].first;
                int d = points[i].second.depth;
                buffer[2*i+1][d*2] = char((n % 10) + '0');
                if (n >= 10) {
                    // note: will truncate 3+ digit numbers to their last 2 digits
                    buffer[2*i+0][d*2] = char((n / 10) % 10 + '0');
                }
            }
            // add the corner edges
            for (int i = 0; i < (int)points.size(); i++) {
                int d = points[i].second.depth;
                int dir = points[i].second.parent_dir;
                if (dir == 0) {
                    continue; // root
                }
                if (points[i + dir].second.depth == d-1) {
                    // adjacent parent
                    buffer[2*i + (dir > 0 ? 2 : 0)][d*2-1] = (dir > 0 ? '/' : '\\');
                } else {
                    int c = 2*i + (dir > 0 ? 2 : -1);
                    buffer[c][d*2-1] = (dir > 0 ? '/' : '\\');
                    buffer[c + dir][d*2-2] = '-';
                    for (int j = i+2*dir; points[j].second.depth != d-1; j += dir) {
                        buffer[2*j][d*2-2] = '-';
                        buffer[2*j+1][d*2-2] = '-';
                    }
                    
                }
            }
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    std::cout << buffer[x][y];
                }
                std::cout << std::endl;
            }
        }*/
        
        /**************** </Printing function used for debugging> ****************/

        /*void fast_merge(AVL<T>& other) {
            merging_stack.push(root);
            
            //used for getting the right inorder traversal without recursion
            std::stack<Node<T>*> inorder_stack;
            Node<T>* traverser = other.root;
            //push the root of the other tree as long as it's not empty
            //and find the first node to insert
            while (traverser) {
                inorder_stack.push(traverser);
                traverser = traverser->left;
            }
            
            //iteratively push nodes to a stack, and perform an
            //insertion whenever there is no left child, then remove
            //that from the stack
            while (!inorder_stack.empty()) {
                for(int i = 0; i < inorder_stack.top()->weights.size(); ++i) {
                    fast_insert(inorder_stack.top()->datum, inorder_stack.top()->weights[i]);
                }
                
                //don't lose track of the right children, so set
                //traverser before popping
                traverser = inorder_stack.top()->right;
                inorder_stack.pop();
                
                while (traverser) {
                    inorder_stack.push(traverser);
                    traverser = traverser->left;
                }
            }
            
            //clear these containers so they don't mess with future merges
            clear_stack(merging_stack);
            clear_stack(successor_stack);
        }*/

        void treeUnion(AVL<T>* other) {
            this->merge(other, other->root);
        }

        void merge(AVL<T>* other, Node<T>* node) {
            if(node->left) merge(other, node->left);
            if(node->right) merge(other, node->right);
            for(int i = 0; i < node->weights.size(); ++i) {
                this->add(node->datum, node->weights[i]);
            }
        }

        ~AVL() {
            destroy_node(root);
        }

        double size() {
            return (root) ? (root->totalWeight + root->leftSubtreeSize + root->rightSubtreeSize) : 0;
        }

    private:
        Node<T>* root;

        /*
        //Containers used for the Brown and Tarjan merging algorithm.
        //These must be member variables because they are used across
        //an entire merge, not just one call to fast_insert()
        std::stack<Node<T>*> merging_stack;
        std::stack<Node<T>*> successor_stack;
        
        //another stack container, this one is used for updating data after
        //insertions and checking if any rotations are needed
        std::stack<Node<T>*> merging_stack_temp;
        
        //An insertion using the Brown and Tarjan merge algorithm.
        //Takes the datum as input rather than the node pointer in
        //case the tree that's merging in gets destroyed after merging
        void fast_insert(T& in_datum, double weight) {
            //find the highest node in the tree and in the last node's
            //path that's less than datum
            while (!successor_stack.empty() &&
                   in_datum >= successor_stack.top()->datum) {
                while (merging_stack.top() != successor_stack.top()) {
                    merging_stack.pop();
                }
                successor_stack.pop();
            }
            
            //variable for the last node visited on the path to insertion so far,
            //for easier readability
            Node<T>* current_top = merging_stack.top();
            
            //add until the top of the path is null, which would mean
            //the new datum gets inserted there
            while (current_top) {
                if (in_datum < current_top->datum) {
                    successor_stack.push(current_top);
                    merging_stack.push(current_top->left);
                } else {
                    merging_stack.push(current_top->right);
                }
                
                current_top = merging_stack.top();
            }
            
            //remove the null pointer at the top
            merging_stack.pop();
            
            //insert the new node and push to merging_stack
            current_top = merging_stack.top();
            if (in_datum == current_top->datum) {
                //
            } else if (in_datum < current_top->datum) {
                current_top->left = new Node<T>{ in_datum, weight, 0, 0, 1, nullptr, nullptr };
                merging_stack.push(current_top->left);
            } else {
                current_top->right = new Node<T>{ in_datum, weight, 0, 0, 1, nullptr, nullptr };
                merging_stack.push(current_top->right);
            }
            
            //now balance if needed
            fast_merge_balance();
        }
        
        //This function and the new ones below it are used for balancing and
        //fitting that to the merging algorithm, but the bulk of the algorithm
        //is the previous two functions
        void fast_merge_balance() {
            //temp stack like merging_stack_temp so we can update all values in the
            //path/successor and then retrieve them both for the next insertion
            std::stack<Node<T>*> successor_temp;
            
            Node<T>* current_top;
            
            while (!merging_stack.empty()) {
                merging_stack_temp.push(merging_stack.top());
                if (!successor_temp.empty() &&
                        merging_stack.top() == successor_stack.top()) {
                    successor_temp.push(successor_stack.top());
                    successor_stack.pop();
                }
                merging_stack.pop();
                current_top = merging_stack_temp.top();
                
                //update height before checking balance
                current_top->fix_height();
                //If we do a rotation, that node is where we leave off the path for
                //the next insertion since it wouldn't be worth adjusting the lower
                //nodes. So we clear the temp stack since it has those nodes
                if (check_and_balance(current_top)) {
                    if (!successor_stack.empty() &&
                        current_top == successor_stack.top()) {
                        //if the top of successor was rotated away then we just
                        //pop it off, and the new node in that spot will be the
                        //first evaluated on the next insert
                        successor_stack.pop();
                    }
                    current_top = merging_stack_temp.top(); //changes when balancing
                    clear_stack(merging_stack_temp);
                    clear_stack(successor_temp);
                    merging_stack_temp.push(current_top);
                }
                //if a rebalancing occurred these should be extra
                fix_left_size(current_top);
                fix_right_size(current_top);
            }
            //now push the temp stacks back into the originals
            while (!merging_stack_temp.empty()) {
                merging_stack.push(merging_stack_temp.top());
                merging_stack_temp.pop();
            }
            while (!successor_temp.empty()) {
                successor_stack.push(successor_temp.top());
                successor_temp.pop();
            }
        }
        
        //checks whether a rotation needs to be made and rotates if it does,
        //and returns whether or not a rotation was made
        bool check_and_balance(Node<T>* current) {
            if (current->balance() > 1) { //rotate right
                if (current->left->balance() == -1){
                    //rotate left first
                    current->left = rotate_left(current->left);
                }
                //now check if this node is the right or left subchild
                //of its parent, and assign accordingly
                //Also has to update the top of the temp stack
                if (!merging_stack.empty() &&
                    current->datum < merging_stack.top()->datum) {
                    merging_stack.top()->left =
                        merging_stack_temp.top() = rotate_right(current);
                }
                else if (!merging_stack.empty() &&
                         current->datum >= merging_stack.top()->datum) {
                    merging_stack.top()->right =
                        merging_stack_temp.top() = rotate_right(current);
                }
                else { //means merging_stack is empty so this is the root
                    root = merging_stack_temp.top() = rotate_right(current);
                }
                return true;
            }
            else if (current->balance() < -1) { //rotate left
                if (current->right->balance() == 1){
                    //rotate right first
                    current->right = rotate_left(current->right);
                }
                //now check if this node is the right or left subchild
                //of its parent, and assign accordingly
                //Also has to update the top of the temp stack
                if (!merging_stack.empty() &&
                    current->datum < merging_stack.top()->datum) {
                    merging_stack.top()->left =
                        merging_stack_temp.top() = rotate_left(current);
                }
                else if (!merging_stack.empty() &&
                         current->datum >= merging_stack.top()->datum) {
                    merging_stack.top()->right =
                        merging_stack_temp.top() = rotate_left(current);
                }
                else { //means merging_stack is empty so this is the root
                    root = merging_stack_temp.top() = rotate_left(current);
                }
                return true;
            }
            
            return false;
        }
        
        void fix_left_size(Node<T>* node) {
            if (!node->left) {
                node->leftSubtreeSize = 0;
            } else {
                double left_left_size = node->left->leftSubtreeSize;
                double left_right_size = node->left->rightSubtreeSize;
                node->leftSubtreeSize =
                    left_left_size + left_right_size + node->left->weight;
            }
        }
        
        void fix_right_size(Node<T>* node) {
            if (!node->right) {
                node->rightSubtreeSize = 0;
            } else {
                double right_left_size = node->right->leftSubtreeSize;
                double right_right_size = node->right->rightSubtreeSize;
                node->rightSubtreeSize =
                    right_left_size + right_right_size + node->right->weight;
            }
        }
        
        void clear_stack(std::stack<Node<T>*>& s) {
            while (!s.empty()) {
                s.pop();
            }
        }
        
        //used for debugging
        //doesn't take a reference so we don't have to worry about pushing
        //the elements back in
        void print_stack(std::stack<Node<T>*> s) {
            while (!s.empty()){
                std::cout << s.top()->datum << std::endl;
                s.pop();
            }
        }*/

        // insert_node returns the increase in height for the provided tree
        Node<T>* insert_node(Node<T>* node, T& datum, double weight) {
            if (node == nullptr) {
                // at a leaf position in the tree, so create a new node
                Node<T>* toAdd = new Node<T>;
                toAdd->datum = datum;
                toAdd->totalWeight = weight;
                toAdd->weights.push_back(weight);
                toAdd->height = 1;
                toAdd->leftSubtreeSize = toAdd->rightSubtreeSize =
                    toAdd->leftSubtreeWY = toAdd->rightSubtreeWY = 0;
                toAdd->left = toAdd->right = 0;
                return toAdd;
            }
            if(datum == node->datum) {
                node->weights.push_back(weight);
                node->totalWeight += weight;
            } else if(datum < node->datum) {
                node->left = insert_node(node->left, datum, weight);
                node->fix_height(); // remember to fix the height of a node after modifying its children
                node->leftSubtreeSize = node->left->leftSubtreeSize + node->left->rightSubtreeSize + node->left->totalWeight;
                node->leftSubtreeWY = node->left->leftSubtreeWY + node->left->rightSubtreeWY +
                                      (node->left->datum * node->left->totalWeight);
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
                node->rightSubtreeSize = node->right->leftSubtreeSize + node->right->rightSubtreeSize + node->right->totalWeight;
                node->rightSubtreeWY = node->right->leftSubtreeWY + node->right->rightSubtreeWY +
                                       (node->right->datum * node->right->totalWeight);
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
            node->rightSubtreeWY = newParent->leftSubtreeWY;
            newParent->leftSubtreeSize = node->leftSubtreeSize + node->rightSubtreeSize + node->totalWeight;
            newParent->leftSubtreeWY = node->leftSubtreeWY + node->rightSubtreeWY + (node->datum * node->totalWeight);
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
            node->leftSubtreeWY = newParent->rightSubtreeWY;
            newParent->rightSubtreeSize = node->leftSubtreeSize + node->rightSubtreeSize + node->totalWeight;
            newParent->rightSubtreeWY = node->leftSubtreeWY + node->rightSubtreeWY + (node->datum * node->totalWeight);
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


struct regression_data {
    AVL<int> levelset;
    size_t left;
    double error;
    int mean; //this is an int since we never divide to find it
};

// [[Rcpp::export]]
vector<regression_data> prefixIsoRegL1(const vector<int>& ycoords,
                                       const vector<double>& weights) {
    size_t data_size = ycoords.size();
    vector<regression_data> reg_data(data_size);
    
    for (size_t i = 0; i < data_size; ++i) {
        //initialize level set
        regression_data& data_at_i = reg_data[i]; //create a reference to possibly
                                                  //save some operator [] calls
        data_at_i.levelset.add(ycoords[i], weights[i]);
        data_at_i.mean = ycoords[i];
        data_at_i.left = i;
        
        size_t prev_level_set = reg_data[i].left - 1; //for readability
        
        //the first part of this conditional checks that there is still a level
        //set to the left of i's level set. There is not one iff left[i] is 0,
        //and then 0 - 1 would be max for size_t
        while (prev_level_set < numeric_limits<size_t>::max() &&
               reg_data[i].mean <= reg_data[prev_level_set].mean) {

            //merge the level sets
            reg_data[i].levelset.treeUnion(&reg_data[prev_level_set].levelset);
            
            //(could be a good idea to delete the tree of prev_level_set here,
            //since we won't need it anymore)
            
            reg_data[i].left = reg_data[prev_level_set].left;
            prev_level_set = reg_data[i].left - 1;
            
            reg_data[i].mean = reg_data[i].levelset.median();
        }
        

        //find error
        double levelerror = reg_data[i].levelset.error(reg_data[i].mean);
        bool not_farthest_left = prev_level_set < numeric_limits<size_t>::max();
        double prev_error = not_farthest_left ? reg_data[prev_level_set].error : 0;
        reg_data[i].error = levelerror + prev_error;
    }
    
    return reg_data;
    
}

// [[Rcpp::export]]
vector<regression_data> unimodalRegL1(vector<int>& ycoords, vector<double>& weights) {
    vector<regression_data> increasing = prefixIsoRegL1(ycoords, weights);
    cout << "test1" << endl;
    
    //reverse vectors, then call prefix L1 on it again
    reverse(ycoords.begin(), ycoords.end());
    reverse(weights.begin(), weights.end());
    vector<regression_data> decreasing = prefixIsoRegL1(ycoords, weights);
    cout << "test2" << endl;
    
    //now find the best point for the mode, and copy the data used in
    //the optimal unimodal regression into a new vector
    
    size_t size = increasing.size();
    //initialized to be the min of fully increasing or fully decreasing
    double min_error = min(decreasing.back().error, increasing.back().error);
    //if all are decreasing, set last increasing to be size to denote none
    size_t last_increasing = min_error == increasing.back().error ? size - 1 : size;

    cout << "test3" << endl;
    
    //the prefix regression did the decreasing regression in reverse order,
    //so instead of reversing the resulting vector we just go from the back
    for (size_t i = 0; i < size - 2; ++i) {
        double error_at_i = increasing[i].error + decreasing[size - 2 - i].error;
        if (min_error > error_at_i) {
            min_error = error_at_i;
            last_increasing = i;
        }
    }

    vector<regression_data> unimodal(size);

    cout << "test4" << endl;
    

    if (last_increasing == size) {
        return decreasing;//REMOVE
        unimodal = decreasing;
        vector<int> finalReg(size);
        for (size_t i = 0; i < size; ++i) {
            finalReg[i] = unimodal[i].mean;
        }
        //return finalReg;
    }
    else if (last_increasing == size - 1) {
        return increasing;//REMOVE
        unimodal = increasing;
        vector<int> finalReg(size);
        for (size_t i = 0; i < size; ++i) {
            finalReg[i] = unimodal[i].mean;
        }
        //return finalReg;
    }
    
    //vector<regression_data> unimodal(size);
    for (size_t i = 0; i <= last_increasing; ++i) {
        unimodal[i] = increasing[i];
    }
    for (size_t i = last_increasing + 1; i < size; ++i) { //double check bounds here***
        unimodal[i] = decreasing[size - 1 - i];
    }

    cout << "test5" << endl;

    return unimodal; //REMOVE

    vector<int> finalReg(size);
    for (size_t i = 0; i < size; ++i) {
        finalReg[i] = unimodal[i].mean;
    }

    //return finalReg;   
}

int main() {
    ifstream in;
    in.open("uniRegData1.txt");
    size_t size;
    in >> size;
    vector<int> ys(size);
    vector<double> ws(size);
    size_t i = 0;
    while (in >> ys[i] >> ws[i]) {
        ++i;
    }
    
    vector<regression_data> unimodal_data = unimodalRegL1(ys, ws);

    cout << "Test6" << endl;
    
    cout <<"\tmean values:\n";
    for (size_t i = 0; i < size; ++i) {
        cout << unimodal_data[i].mean << ' ';
    }
    cout << endl << "\terror values:\n";
    for (size_t i = 0; i < size; ++i) {
        cout << unimodal_data[i].error << ' ';
    }
    cout << endl;
    
    return 0;
}


