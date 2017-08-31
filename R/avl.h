/******************************************
 * AVL Tree Header File                   *
 * Authors: Aman Karunakaran              *
 *          Kurt Ayalp                    *
 * Last Modified: 6/7/17                  *
 ******************************************/
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

// probably useful
int max(int x, int y) {
    return x > y ? x : y;
}

template <typename T>
class AVL {
    //friend class HauntedTree<T, AVL<T> >;
    public:
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
        }
        
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
        Node<T>* root = nullptr;

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
