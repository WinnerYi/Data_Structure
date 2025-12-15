//11327217 蔡易勳     11327222 林暐倫 
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono> // count time
#include <iomanip>
#include <climits>
void PrintTitle();
void SkipSpace(std::string &str);
std::string ReadInput();

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BinarySearchTreeIterative {
private:
    Node* root;

public:
    BinarySearchTreeIterative() : root(nullptr) {}

    // 1. Iterative Insertion
    void insert(int val) {
        Node* newNode = new Node(val);
        if (root == nullptr) {
            root = newNode;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (val < current->data) {
                current = current->left;
            } else if (val > current->data) {
                current = current->right;
            } else {
                // Value already exists, handle as needed (here we just return)
                delete newNode; // Avoid memory leak for duplicate
                return;
            }
        }

        // Attach the new node to the correct parent pointer
        if (val < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    }

    // 2. Iterative Search
    bool search(int val) {
        Node* current = root;
        while (current != nullptr) {
            if (current->data == val) {
                return true; // Found the value
            } else if (val < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false; // Value not found
    }

    // 3. Iterative In-order Traversal (requires a stack)
    // This is the most common way to do in-order iteratively
    void inorder() {
        if (root == nullptr) return;

        // We use the standard library stack for traversal
        std::vector<Node*> stack;
        Node* current = root;

        while (current != nullptr || !stack.empty()) {
            // Reach the left most Node of the current Node
            while (current != nullptr) {
                stack.push_back(current);
                current = current->left;
            }

            // Current must be NULL at this point
            current = stack.back();
            stack.pop_back();

            std::cout << current->data << " ";

            // We have visited the node and its left subtree. Now, go to the right subtree.
            current = current->right;
        }
        std::cout << std::endl;
    }

    // 4. Iterative Deletion (More complex, but achievable)
    // Deletion is significantly more complex without recursion and often left recursive for clarity, 
    // but a full iterative solution exists. The implementation below handles most cases iteratively.
    void deleteNode(int val) {
        Node* parent = nullptr;
        Node* current = root;
        bool isLeftChild = false;

        // Find the node to delete and its parent
        while (current != nullptr && current->data != val) {
            parent = current;
            if (val < current->data) {
                isLeftChild = true;
                current = current->left;
            } else {
                isLeftChild = false;
                current = current->right;
            }
        }

        if (current == nullptr) return; // Value not in BST

        // Case 1: Node to be deleted has no children or one child
        if (current->left == nullptr || current->right == nullptr) {
            Node* child = (current->left == nullptr) ? current->right : current->left;

            if (parent == nullptr) { // Deleting the root node
                root = child;
            } else if (isLeftChild) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            delete current;
        } 
        // Case 3: Node to be deleted has two children
        else {
            // Find the in-order successor (smallest in the right subtree)
            Node* successorParent = current;
            Node* successor = current->right;
            while (successor->left != nullptr) {
                successorParent = successor;
                successor = successor->left;
            }

            // Copy the successor's data to the current node
            current->data = successor->data;

            // Delete the successor (which now has at most one child: its original right child)
            if (successorParent->left == successor) {
                successorParent->left = successor->right;
            } else { // If successor was the immediate right child of current
                successorParent->right = successor->right;
            }
            delete successor;
        }
    }


    // Destructor (Still uses recursion internally to clean up efficiently)
    ~BinarySearchTreeIterative() {
        deleteTreeRecursive(root); // Cleanup is hard to do iteratively without extra structures
    }

private:
    void deleteTreeRecursive(Node* node) {
        if (node == nullptr) return;
        deleteTreeRecursive(node->left);
        deleteTreeRecursive(node->right);
        delete node;
    }
};




int main() {
 
  bool has_command2 = false;
  while (true) {
    PrintTitle();
    int cmd;
    std::cin >> cmd;
    if (std::cin.fail()) { // 檢查輸入是否失敗
        std::cin.clear();              
        std::cin.ignore(10000, '\n');  
        return 0;
    } else if (cmd == 0 ){
      return 0;
    } else if (cmd == 1) {
      printf("\n");
      goods.ResetGoods();
      if (goods.fetchFile()) {

        goods.taskOne();
      }
      has_command2 = false;
      list.ResetSorted();
      
    } else if (cmd == 2) {
      printf("\n");
      if (has_command2) {
        list.ResetSorted();
      }
      if (list.fetchFile()) {
        printf("\n");
        list.printOrders();
        list.MultiChef(1);
        has_command2 = true;
      }
      
      list.ResetCancel();
 
  
    } else if (cmd == 3) {
   
      if (!has_command2) {
        std::cout << "\n### Execute command 2 first! ###\n\n";
        continue;
      }
      list.MultiChef(2);
      list.ResetCancel();
     
    } else if (cmd == 4) {
     
      if (!has_command2) {
        std::cout << "\n### Execute command 2 first! ###\n\n";
        continue;
      }
      printf("\n");
      std::cout << "Input the number of queues: ";
      int num;
      std::cin >> num;
      list.MultiChef(num);
      list.ResetCancel();
    } else {
      printf("\n");
      std::cout << "Command does not exist!\n";
    }
    printf("\n");  
  }
}

std::string ReadInput() {
  std::string input;
  while (1) {
    std::getline(std::cin, input);
    SkipSpace(input);
    if (input.empty()) continue;
    else break;
  }
  return input;
}
void SkipSpace(std::string &str) {
  for (int i = 0; i < str.size(); i++) {
    if (str[i] != ' ') break;
    if (str[i] == ' ') {
      str.erase(str.begin() + i);
      i--;
    }
  }
  for (int i = str.size()-1; i >= 0; i--) {
    if (str[i] != ' ') break;
    if (str[i] == ' ') {
      str.erase(str.begin() + i);
    }
  }
}

void PrintTitle () {
  std::cout << "*** (^_^) Data Structure (^o^) ***\n";
  std::cout << "** Binary Search Tree on Pokemon *\n";
  std::cout << "* 0. QUIT                        *\n";
  std::cout << "* 1. Read a file to build HP BST *\n";
  std::cout << "* 2. Range search on HP field    *\n";
  std::cout << "* 3. Delete the min on HP field  *\n";    
  std::cout << "* 4. Rebuild the balanced HP BST *\n";
  std::cout << "**********************************\n";
  std::cout << "Input a command(0, 1, 2, 3, 4): ";
} 