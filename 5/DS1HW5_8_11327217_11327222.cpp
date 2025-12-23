//11327217 蔡易勳     11327222 林暐倫 
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <climits>
#include <stack>
#include <queue>
void PrintTitle();
void SkipSpace(std::string &str);
std::string ReadInput();
struct Node {
  int hp;                    // 生命值（BST key）
  std::vector<int> ids;      // 相同 hp 的所有編號
  Node* left;
  Node* right;

  Node(int hp, int id) {
    this->hp = hp;
    left = nullptr;
    right = nullptr;
    ids.push_back(id); // 預設編號
  } 
};


class BinarySearchTreeIterative { // only store id and hp
 private:
    Node* root;
    void inorderHelper(Node* cur, std::vector<Node*>& nodes) {
        if (!cur) return;
        inorderHelper(cur->left, nodes);
        nodes.push_back(cur);
        inorderHelper(cur->right, nodes);
    }

    Node* buildMinBST(std::vector<Node*>& nodes, int l, int r) {
        if (l > r) return nullptr;

        int mid = (l + r) / 2;
        Node* cur = nodes[mid];

        cur->left  = buildMinBST(nodes, l, mid - 1);
        cur->right = buildMinBST(nodes, mid + 1, r);

        return cur;
    }

    void printLevelOrder(Node* root) {
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);

        int level = 1;
        std::cout << "HP tree:\n";

        while (!q.empty()) {
            int levelSize = q.size();
            std::cout << "<level " << level++ << "> ";

            for (int i = 0; i < levelSize; ++i) {
                Node* cur = q.front(); q.pop();

                std::cout << "(" << cur->hp << ", ";
                for (size_t j = 0; j < cur->ids.size(); ++j) {
                    std::cout << cur->ids[j];
                    if (j + 1 < cur->ids.size())
                        std::cout << "|";
                }
                std::cout << ")";
                if (i != levelSize - 1) {
                  std::cout << " ";
                }
                if (cur->left)  q.push(cur->left);
                if (cur->right) q.push(cur->right);
            }
            std::cout << "\n";
        }
    }

 public:
    BinarySearchTreeIterative()  {
      root = nullptr;
    }

    // 插入：生命值 + 編號 task1
    void insert(int hp, int id) {
        if (root == nullptr) {
            root = new Node(hp, id);
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (hp < current->hp) {
                current = current->left;
            }
            else if (hp > current->hp) {
                current = current->right;
            }
            else {
                // hp 相同 → 存到同一個節點
                current->ids.push_back(id);
                return;
            }
        }

        if (hp < parent->hp)
            parent->left = new Node(hp, id);
        else
            parent->right = new Node(hp, id);
    }

    // 搜尋：回傳該生命值的所有編號 task2
    void rangeSearchIterative(int low, int high, std::vector<Node*>& result, int& visitedCount) {
      visitedCount = 0;
      result.clear();
      if (root == nullptr) return;

      std::stack<Node*> st;
      st.push(root);

      while (!st.empty()) {
        Node* current = st.top();
        st.pop();

        visitedCount++;   // 走訪節點

        if (current->hp < low) {
            // 只可能在右子樹
            if (current->right != nullptr)
                st.push(current->right);
        }
        else if (current->hp > high) {
            // 只可能在左子樹
            if (current->left != nullptr)
                st.push(current->left);
        }
        else {
            // 命中範圍
            result.push_back(current);

            if (current->left != nullptr && current->hp > low)
                st.push(current->left);

            if (current->right != nullptr && current->hp < high)
                st.push(current->right);
        }
     }
  }

    Node* deleteExtreme(bool deleteMin) {
      if (root == nullptr) return nullptr;
      Node* parent = nullptr;
      Node* current = root;

      if (deleteMin) {
          // 找最小 hp
          while (current->left != nullptr) {
              parent = current;
              current = current->left;
          }

          // 刪除節點
          if (parent == nullptr)
              root = current->right;   // root 是最小
          else
              parent->left = current->right;
      } else if (!deleteMin) {
          // 找最大 hp
        while (current->right != nullptr) {
            parent = current;
            current = current->right;
        }

        if (parent == nullptr)
            root = current->left;    // root 是最大
        else
            parent->right = current->left;
      }

      return current; //  回傳被刪的節點（還沒 delete）
    }

    void rebuildMinHeight() {
        std::vector<Node*> nodes;
        inorderHelper(root, nodes);
        root = buildMinBST(nodes, 0, nodes.size() - 1);
    }
    void printHPtree() {
        printLevelOrder(root);
    }

    int height() {
        if (root == nullptr) return 0;  // 空樹
        std::queue<Node*> q;
        q.push(root);
        int h = 0;
        while (!q.empty()) {
            int levelSize = q.size();
            h++;  // 每跑一次 while = 一層 = 高度 +1

            for (int i = 0; i < levelSize; i++) {
                Node* cur = q.front();
                q.pop();

                if (cur->left)
                    q.push(cur->left);
                if (cur->right)
                    q.push(cur->right);
            }
        }
        return h;
    }

    void clear() {
      deleteTreeRecursive(root);
      root = nullptr;
    }

    ~BinarySearchTreeIterative() {
      clear();
    }

private:
    void deleteTreeRecursive(Node* node) {
        if (!node) return;
        deleteTreeRecursive(node->left);
        deleteTreeRecursive(node->right);
        delete node;
    }
};

class Raichu{  // 雷丘放電
 private:
  int id;
  std::string name;
  std::string type1;
  std::string type2;
  int total;
  int hp;
  int attack;
  int defense;
  int sp_atk;
  int sp_def;
  int speed;
  int generation;
  std::string legendary;

  public:
    void setRaichu(int id, std::string name, std::string type1, std::string type2,
                int total, int hp, int attack, int defense,
                int sp_atk, int sp_def, int speed,
                int generation, std::string legendary) {

      this->id = id;
      this->name = name;
      this->type1 = type1;
      this->type2 = type2;
      this->total = total;
      this->hp = hp;
      this->attack = attack;
      this->defense = defense;
      this->sp_atk = sp_atk;
      this->sp_def = sp_def;
      this->speed = speed;
      this->generation = generation;
      this->legendary = legendary;
    }
    int getId() {
      return id;
    }

    std::string getName() {
      return name;
    }

    std::string getType1() {
      return type1;
    }

    int getTotal() {
      return total;
    }

    int getHp() {
      return hp;
    }

    int getAttack() {
      return attack;
    }

    int getDefense() {
      return defense;
    }

    int getSpAttack() {
      return sp_atk;
    }
    int getSpDefense() {
      return sp_def;
    }

};

class Pokemon {
 private:
  BinarySearchTreeIterative bst;
  std::vector<Raichu> raichus;
  int max_hp;
  
 public:
  Pokemon() {
    max_hp = 0;
  }
  void reSet() {
    bst.clear();
    raichus.clear();
    raichus.shrink_to_fit();  // 釋放多餘容量
    max_hp = 0;

  }
  bool fetchFile() {
    std::ifstream in;
    while (1) {
      std:: cout << "Input a file number [0: quit]: ";
      std::string file_num = ReadInput();
      if (file_num == "0") return false;
      std::string txt_path = "input" + file_num + ".txt";
      in.open(txt_path);
      if(in.fail()){ 
        std::cout << std::endl << "### " << txt_path + " does not exist! ###" << std::endl;
        printf("\n");
        continue;
      }
      break;
    }
    std::string title;
    std::getline(in, title);

    std::string line;
    while (std::getline(in, line)) {
      std::stringstream ss(line);
      int id, total, hp, attack, defense, sp_atk, sp_def, speed, generation;
      std::string name, type1, type2, legendary;

      std::string id_str, total_str, hp_str, attack_str,
                  defense_str, sp_atk_str, sp_def_str,
                  speed_str, generation_str;
      std::getline(ss, id_str, '\t');
      std::getline(ss, name, '\t');
      std::getline(ss, type1, '\t');
      std::getline(ss, type2, '\t');
      std::getline(ss, total_str, '\t');
      std::getline(ss, hp_str, '\t');
      std::getline(ss, attack_str, '\t');
      std::getline(ss, defense_str, '\t');
      std::getline(ss, sp_atk_str, '\t');
      std::getline(ss, sp_def_str, '\t');
      std::getline(ss, speed_str, '\t');
      std::getline(ss, generation_str, '\t');
      std::getline(ss, legendary); // 最後一欄到行尾
      id        = std::stoi(id_str);
      total      = std::stoi(total_str);
      hp         = std::stoi(hp_str);
      attack     = std::stoi(attack_str);
      defense    = std::stoi(defense_str);
      sp_atk     = std::stoi(sp_atk_str);
      sp_def     = std::stoi(sp_def_str);
      speed      = std::stoi(speed_str);
      generation = std::stoi(generation_str);
      if (hp > max_hp) {
        max_hp = hp;
      }
      Raichu r;
      r.setRaichu(id, name, type1, type2,
                  total, hp, attack, defense,
                  sp_atk, sp_def, speed,
                  generation, legendary);
      bst.insert(hp, id);
      raichus.push_back(r);
  }   
    in.close();
    return true;
}

int getRaichuSize() {
  return raichus.size();
}
int getTreeHeight() {
  return bst.height();
}

void taskOne() {
  std::cout << '\t' << "#" << '\t' << std::setw(19) << std::left << "Name" << '\t'<< std::setw(10) << std::left << "Type 1"  << '\t' << "HP";
  
  std::cout << std::endl;
  for (int i = 0; i < raichus.size(); i++) {
    std::cout << "[" << std::right << std::setw(3) << i + 1 << "]";
    std::cout << '\t' << raichus[i].getId();
    std::cout << '\t' << std::left<< std::setw(20) << raichus[i].getName() << "\t";
    std::cout << std::left<< std::setw(10) << raichus[i].getType1() << "\t";
    std::cout << std::left<< std::setw(6) << raichus[i].getHp();
    std::cout << "\n";
  }
  std::cout << "HP tree height = " << bst.height() << std::endl;
}

void taskTwo() { // done
  int low, high;
  while (1) {
    std::cout << "\nInput a non-negative integer: ";
    std::cin >> low;
    if (std::cin.fail()) { // 檢查輸入是否失敗
        std::cin.clear();              
        std::cin.ignore(10000, '\n'); 
        std::cout << "\n### It is NOT a non-negative integer. ###\nTry again: "; 
        continue;;
    }
    if (low > (max_hp * 2)) {
      std::cout << "\n### It is NOT in [0," << max_hp * 2 << "]. ###\nTry again: ";
      continue;
    }
    if (low >= 0) break;
    if (low < 0) {
      std::cout << "\n### It is NOT a non-negative integer. ###\nTry again: ";
    }
  }
  while (1) {
    std::cout << "\nInput a non-negative integer: ";
    std::cin >> high;
    if (std::cin.fail()) { // 檢查輸入是否失敗
        std::cin.clear();              
        std::cin.ignore(10000, '\n'); 
        std::cout << "\n### It is NOT a non-negative integer. ###\nTry again: "; 
        continue;;
    }
    if (high > (max_hp * 2)) {
      std::cout << "\n### It is NOT in [0," << max_hp * 2 << "]. ###\nTry again: ";
      continue;
    }
    if (high >= 0) break;
    if (high < 0) {
      std::cout << "\n### It is NOT a non-negative integer. ###\nTry again: ";
    }
  }
  if (low > high) {
    int temp = low;
    low = high;
    high = temp;
  }

  std::vector<Node*> result;
  int visitedCount = 0;
  bst.rangeSearchIterative(low, high, result, visitedCount);
  
  
  if (result.empty()) {
    std::cout << "No record was found in the specified range." << std::endl;
  } else {
    for (int i = 0; i < result.size() - 1; i++) {
      for (int j = 0; j < result.size() - 1 - i; j++) {
        if (result[j]->hp < result[j+1]->hp) {
            // 交換
            Node* temp = result[j];
            result[j] = result[j+1];
            result[j+1] = temp;
        }
     }
   }
    std::cout << "\t#\t" << std::setw(19) << std::left << "Name" << "\t" << std::setw(10) << std::left
              <<  "Type 1" << "\t" << "Total" << "\t" << "HP" << "\t" << "Attack" << "\t" << "Defense"; 
    std::cout << std::endl;

    int idx = 1;
    for (int i = 0; i < result.size(); i++) {  
      Node* node = result[i];
      for (int j = 0; j < node->ids.size(); j++) { // 走這個節點的每個 id
        int id = node->ids[j];
        for (int k = 0; k < raichus.size(); k++) { // 從頭找 raichus
            if (raichus[k].getId() == id) {
                std::cout << "[" << std::right << std::setw(3) << idx << "]";
                idx++;
                std::cout << '\t' <<raichus[k].getId();
                std::cout << '\t' << std::setw(20) << std::left << raichus[k].getName() << '\t'; 
                std::cout << std::setw(10) << raichus[k].getType1() << '\t';
                std::cout << std::setw(6) << std::left << raichus[k].getTotal() << '\t';
                std::cout << raichus[k].getHp() << '\t';
                std::cout << raichus[k].getAttack() << '\t';
                std::cout << raichus[k].getDefense() << '\n';
                break; 
            }
        }
      }

    }
  }
  std::cout << "Number of visited nodes = " << visitedCount << std::endl;
}

void taskThree(bool deleteMin) {
  Node* deleted = bst.deleteExtreme(deleteMin);
  std::cout << "\t#\t" << std::setw(19) << std::left << "Name" << "\t" << std::setw(10) << std::left
              <<  "Type 1" << "\t" << "Total" << "\t" << "HP" << "\t" << "Attack" << "\t" << "Defense" << '\t' << "Sp. Atk" << '\t' << "Sp. Def"; 
   std::cout << std::endl;
  int idx = 1;
  for (int i = 0; i < deleted->ids.size(); i++) {
      int id = deleted->ids[i];
      for (int j = 0; j < raichus.size(); j++) {
        if (raichus[j].getId() == id) {
          std::cout << "[" << std::right << std::setw(3) << idx << "]";
          std::cout << '\t' << raichus[j].getId();
          std::cout << '\t' << std::setw(20) << std::left << raichus[j].getName() << '\t';
          std::cout <<  std::setw(10) << raichus[j].getType1() << '\t';
          std::cout <<  std::setw(6)<< std::left << raichus[j].getTotal() << '\t';
          std::cout << raichus[j].getHp() << '\t';
          std::cout << raichus[j].getAttack() << '\t';
          std::cout << raichus[j].getDefense() << '\t';
          std::cout << std::setw(6) << std::left << raichus[j].getSpAttack() << "\t";
          std::cout << raichus[j].getSpDefense() << std::endl;
          idx++;
          break;
        }
      }
    }
  delete deleted;
  std::cout << "HP tree height = " << bst.height() << std::endl;
}

void taskFour() {
  bst.rebuildMinHeight();
  bst.printHPtree();
}
};
int main() {
 
  Pokemon pokemon;
   bool deleteMin = true;
  while (true) {
    PrintTitle();
    int cmd;
    std::cin >> cmd;
    if (std::cin.fail()) { // 檢查輸入是否失敗
      std::cin.clear();              
      std::cin.ignore(10000, '\n');  
      std::cout << "\nCommand does not exist!\n\n";
      continue;
    } else if (cmd == 0 ){
      return 0;
    } else if (cmd == 1) {
      pokemon.reSet();
      printf("\n");
      if (pokemon.fetchFile()) {
        pokemon.taskOne();
      }
      deleteMin = true;
    } else if (cmd == 2) {
      if (pokemon.getTreeHeight() == 0) {
        std::cout << "\n----- Execute Mission 1 first! -----\n\n";
        continue;
      }
      pokemon.taskTwo();
      
    } else if (cmd == 3) {
      if (pokemon.getTreeHeight() == 0) {
        std::cout << "\n----- Execute Mission 1 first! -----\n\n";
        continue;
      }
      printf("\n");
      pokemon.taskThree(deleteMin);
      deleteMin = !deleteMin;
     
    } else if (cmd == 4) {
      if (pokemon.getTreeHeight() == 0) {
        std::cout << "\n----- Execute Mission 1 first! -----\n\n";
        continue;
      }
      deleteMin = true;
      printf("\n");
      pokemon.taskFour();
    
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
  std::cout << "Input a choice(0, 1, 2, 3, 4): ";
} 