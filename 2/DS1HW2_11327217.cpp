// 11327217 蔡易勳
#include <iostream>
#include <string>
#include <climits>
#include <iomanip>

struct Term {
  double coefficient;
  int power;
  Term *next;
};
// function Declaration
void PrintTitle();
bool isNonNegInt (std::string s);
bool isDouble(std::string s);
void SkipSpace(std::string &str);
void PrintPoly(Term *head);
Term *GetNewTerm (double coefficient, int power);
// Class Declaration
class Polynomial{ 
 private:
  Term *input_head = nullptr; 
  Term *reduced_head = nullptr;
 public:
  Polynomial() {
    reduced_head = nullptr;
    input_head = nullptr;
  }
  ~Polynomial() {
    ResetInput(); 
    ResetReduced();
  }
  Term *GetReducedHead() const{
    return reduced_head;
  }
  bool Empty() const{
    if (reduced_head == nullptr) return true;
    return false;
  }
  void InputCoefficientPower(double &coefficient, int &power) {
    std::string str_coefficient; 
    std::string str_power;
    while (1) {
      str_coefficient = "";
      std::cout << "Input the coefficient: ";
      std::getline(std::cin, str_coefficient);
      SkipSpace(str_coefficient);
      if (str_coefficient.empty()) {
        continue;
      }
      else if (!isDouble(str_coefficient)) { // **判斷是否輸入非數字
        std::cout << "### Not a floating-point number! ###\n";
        continue;
      } else break;
    }
    coefficient = std::stod(str_coefficient); 
    while (1) {
      str_power = "";
      std::cout << "Input the power: ";
      std::getline(std::cin, str_power);
      SkipSpace(str_power);
      if (str_power.empty()) { // **判斷是否輸入非數字
        continue;
      } else if (!isNonNegInt(str_power)) {
        std::cout << "### Not a non-negative integer! ###\n";
        continue;
      } else break;
    }
    power = std::stoi(str_power);
  }
  void ResetInput() { // 釋放記憶體
    Term *cur = input_head;
    while (cur != nullptr) {
      Term *temp = cur;
      cur = cur->next;
      delete temp;
    } 
    input_head = nullptr;
  }
  void ResetReduced() { // 釋放記憶體
    Term *cur = reduced_head;
    while (cur != nullptr) {
      Term *temp = cur;
      cur = cur->next;
      delete temp;
    } 
    reduced_head = nullptr;
  }
  void StoreToInput (double &coefficient, int &power) {
    Term *new_term = new Term;
    if (coefficient != 0) {
      new_term ->coefficient = coefficient;
      new_term ->power = power;
      if (input_head == nullptr || new_term->power < input_head->power) {
      new_term->next = input_head;
      input_head = new_term; 
      } else {
        Term *cur = input_head;
        if (new_term->power == cur->power && new_term->coefficient < cur->coefficient) {
          new_term->next = input_head;
          input_head = new_term; 
          return;
        }
        while (cur->next != nullptr && new_term->power > cur->next->power) {
          cur = cur->next; 
        }
        new_term->next = cur->next;
       cur->next = new_term;
      }
    }
  }
  void StoreToReduced (double &coefficient, int &power) {
    Term *new_term = new Term;
    if (coefficient != 0) {
      new_term ->coefficient = coefficient;
      new_term ->power = power;
      if (reduced_head == nullptr || new_term->power < reduced_head->power) {
      new_term->next = reduced_head;
      reduced_head = new_term; 
      } else {
        Term *cur = reduced_head;
        while (cur->next != nullptr && new_term->power >= cur->next->power) {
          cur = cur->next; 
        }
        if (cur->power == new_term->power) {
          cur->coefficient += new_term->coefficient;
          delete new_term;
        } else {
          new_term->next = cur->next;
          cur->next = new_term;
        }
      }
    }
    Term *check_cur = reduced_head;
    Term *prev = nullptr;
    while (check_cur != nullptr) {
      if (check_cur->coefficient == 0) {
        Term *tmp = check_cur;
        if (prev == nullptr) {
          reduced_head = check_cur->next;
        } else  prev->next = check_cur->next;
        check_cur = check_cur->next;
        delete tmp;
        continue;
      }
      prev = check_cur;
      check_cur = check_cur->next;
    }
  }
  void PrintInputPoly() const{
    PrintPoly(input_head);
  }
  void PrintReducedPoly() const{  
    PrintPoly(reduced_head);
  }
  void Reduced() { // cmd == 1
    double coefficient;
    int power; 
    while (true) {
      std::string input;
      InputCoefficientPower(coefficient, power);
      StoreToInput(coefficient, power);
      StoreToReduced(coefficient, power);
      std::cout << "Input one more term? [q/Q to quit]\n";
      
      std::getline(std::cin, input); 
      SkipSpace(input);
      if (input == "q" || input == "Q") {
        break;
      }
    }
    std::cout << "The input polynomial:";
    PrintInputPoly();
    std::cout << "The reduced polynomial:";
    PrintReducedPoly();

  }

  // const 是 constant（常數） 的縮寫，意思是「這個東西不能被改變！」, 我只是拿 poly2 來參考，不會改動它的內容
  void Addition(const Polynomial &poly2) { // cmd == 2
    PrintReducedPoly();
    std::cout << "[+]";
    poly2.PrintReducedPoly();
    std::cout << "------------------------------\n";

    Term *cur1 = reduced_head; // main poly
    Term *prev1 = nullptr;
    Term *cur2 = poly2.GetReducedHead();
    if (cur1 == nullptr) {
      while(cur2 != nullptr) {
        Term *temp = GetNewTerm(cur2->coefficient, cur2->power);
        if (reduced_head == nullptr) {
          reduced_head = temp;
          prev1 = reduced_head;
        } else {
          prev1->next = temp;
          prev1 = prev1->next;
        }
        cur2 = cur2->next;
      }

      PrintReducedPoly();
      return;
    }
   
    while(cur2 != nullptr) {
      Term *temp = GetNewTerm(cur2->coefficient, cur2->power);
      bool inserted = false;
      
      while (cur1 != nullptr) {
        if (cur2->power == cur1->power) {
          cur1->coefficient += cur2->coefficient;
          if (cur1->coefficient == 0) {
            if (prev1 == nullptr) {
              reduced_head = cur1->next; 
            } else {
              prev1->next = cur1->next;
            }
            Term *tmp = cur1;
            cur1 = cur1->next;
            delete tmp;
            inserted = true;
            break;
          }
          inserted = true;
          break;
        } else if (cur2->power < cur1->power) {
          if (prev1 == nullptr) {   
            temp->next = cur1;
            reduced_head = temp;
          } else {
            temp->next = cur1;
            prev1->next = temp;
          }
          inserted = true;
          break;
        }
        
        prev1 = cur1;
        cur1 = cur1->next;
      }
      if (!inserted) { // insert in tail
        prev1->next = temp;
        prev1 = temp;
      }
      cur2 = cur2->next;
    }

    PrintReducedPoly();
  
  }
  void Subtraction(const Polynomial &poly3) { // cmd == 3
    PrintReducedPoly();
    std::cout << "[-]";
    poly3.PrintReducedPoly();
    std::cout << "------------------------------\n";
    Term *cur1 = reduced_head; // main poly
    Term *prev1 = nullptr;
    Term *cur3 = poly3.GetReducedHead();
    if (cur1 == nullptr) {
      while(cur3 != nullptr) {
        Term *temp = GetNewTerm(-(cur3->coefficient), cur3->power);
        if (reduced_head == nullptr) {
          reduced_head = temp;
          prev1 = reduced_head;
        } else {
          prev1->next = temp;
          prev1 = prev1->next;
        }
        cur3 = cur3->next;
      }

      PrintReducedPoly();
      return;
    }
    while(cur3 != nullptr) {
      Term *temp = GetNewTerm(-(cur3->coefficient), cur3->power);
      bool inserted = false;
      while (cur1 != nullptr) {
        if (cur3->power == cur1->power) {
          cur1->coefficient -= cur3->coefficient;
          if (cur1->coefficient == 0) {
            if (prev1 == nullptr) {
              reduced_head = cur1->next; 
            } else {
              prev1->next = cur1->next;
            }
            Term *tmp = cur1;
            cur1 = cur1->next;
            delete tmp;
            inserted = true;
            break;
          }
          inserted = true;
          break;
        } else if (cur3->power < cur1->power) {
          if (prev1 == nullptr) {   
            temp->next = cur1;
            reduced_head = temp;
          } else {
            temp->next = cur1;
            prev1->next = temp;
          }
          inserted = true;
          break;
        }
        
        prev1 = cur1;
        cur1 = cur1->next;
      }
      if (!inserted) { // insert in tail
        prev1->next = temp;
        prev1 = temp;
      }
      cur3 = cur3->next;
    }

    PrintReducedPoly();
    
  }
  
};
void Multiplication(const Polynomial &p4, const Polynomial &p5);// cmd == 4
// Main Function

int main() {
  Polynomial polynomial1; // the main polynomial for cmd 1, 2, 3
  while (true) {
    std::string cmd ;
    PrintTitle();
    std::getline(std::cin, cmd);
    printf("\n");
    SkipSpace(cmd);
    if (cmd == "0" || cmd.empty()) {
      return 0;
    } else if (cmd == "1") {
      polynomial1.ResetReduced();
      std::cout << "Inpu qt one polynomial...\n";
      std::cout << "Input each term as a pair of coefficient and power.\n";
      polynomial1.Reduced();
      polynomial1.ResetInput();
    } else if (cmd == "2") {
      if (polynomial1.GetReducedHead() == nullptr) {
        std::cout << "Command 1 first!\n\n";
        continue;
      }
      Polynomial polynomial2;
      std::cout << "Input the addend...\n";
      std::cout << "Input each term as a pair of coefficient and power.\n";
      polynomial2.Reduced();
      polynomial1.Addition(polynomial2);
    } else if (cmd == "3") {
      if (polynomial1.GetReducedHead() == nullptr) {
        std::cout << "Command 1 first!\n\n";
        continue;
      }
      Polynomial polynomial3;
      std::cout << "Input the subtrahend...\n";
      std::cout << "Input each term as a pair of coefficient and power.\n";
      polynomial3.Reduced();
      polynomial1.Subtraction(polynomial3);
      
    } else if (cmd == "4") {
      Polynomial polynomial4;
      Polynomial polynomial5;
      std::cout << "Input the 1st polynomial...\n";
      std::cout << "Input each term as a pair of coefficient and power.\n";
      polynomial4.Reduced();
      printf("\n");
      std::cout << "Input the 2nd polynomial...\n";
      std::cout << "Input each term as a pair of coefficient and power.\n";
      polynomial5.Reduced();
      Multiplication(polynomial4, polynomial5);
    } else {
      std::cout << "Command does not exist!\n";
    }
    printf("\n");  
  }

}
bool isNonNegInt (std::string s) {
  bool hasDigit = false;
  int i = 0;
  if (s[i] == '+') {
    i++;
    if (s.size() == 1) return false; // 單獨一個符號不行
  }
  for (; i < s.size(); i++) {
    if (s[i] >= '0' && s[i] <= '9') {
       hasDigit = true; // 至少有一個數字
    }  else return false; // 任何其他字元都不允許
        
  }
    // 最後要至少有一個數字
  return hasDigit;
}

bool isDouble(std::string s) {
  
    int i = 0;
    bool hasDot = false;
    bool hasDigit = false;
    if (s[i] == '+' || s[i] == '-') {
     i++;
     if (s.size() == 1) return false; // 單獨一個符號不行
    }

    for (; i < s.size(); i++) {
      if (s[i] >= '0' && s[i] <= '9') {
        hasDigit = true; // at least one num
      } else if (s[i] == '.') {
        if (hasDot) return false; // repeated dot
          hasDot = true;

          // 小數點不能at end 
        if (i == s.size() - 1) return false;
        } else return false; // other chars not allowed 
    }

    // at least有一個數字
    return hasDigit;
}
void PrintPoly(Term *head){
  bool empty = true;
    Term *cur = head; 
    int count = 0; // count how many I have printed
    while (cur != nullptr) {
      if (cur->coefficient == 0) {
        cur = cur->next;
        continue;
      }
      empty = false;
      std::cout << '\t';
      if (count != 0 && cur->coefficient > 0) {
        std::cout << '+';
      }
      if (cur->power == 0) {
        std::cout <<  cur->coefficient;
      } else if (cur->power == 1 && cur->coefficient == 1) {
        std::cout << "x";
      } else if (cur->power == 1 && cur->coefficient == -1) {
        std::cout << "-x";
      } else if (cur->power == 1) {
        std::cout <<  cur->coefficient << "x";
      } else if (cur->coefficient == 1) {
        std::cout << "x^";
        std::cout << cur->power;
      } else if (cur->coefficient == -1) {
        std::cout << "-x^";
        std::cout << cur->power;
      } else {
        std::cout <<  cur->coefficient << "x^";
        std::cout << cur->power;
      }
      cur = cur->next;
      count++;
    }

    if (empty) {
      printf("\t0\n");
      return;
    }
    printf("\t");
    printf("\n");
}
void Multiplication(const Polynomial &p4, const Polynomial &p5) { // cmd == 4
  p4.PrintReducedPoly();
  std::cout << "[*]";
  p5.PrintReducedPoly();
  std::cout << "------------------------------\n";
  Term *cur4 = p4.GetReducedHead();
  
  Polynomial result_p;
  Term *res_head = result_p.GetReducedHead();
  Term * prev = nullptr;
  while (cur4 != nullptr) {
    Term *cur5 = p5.GetReducedHead();
    while (cur5 != nullptr) {
      Term *answer = GetNewTerm(cur4->coefficient*cur5->coefficient, cur4->power+cur5->power);
      double c = cur4->coefficient*cur5->coefficient;
      int p = cur4->power+cur5->power;
      result_p.StoreToReduced(c, p);
      cur5 = cur5->next;
    }


    cur4 = cur4->next;                                                 
  }
  result_p.PrintReducedPoly();
}
Term *GetNewTerm (double coefficient, int power) {
  Term *new_term = new Term;
  new_term->coefficient = coefficient;
  new_term->power = power;
  return new_term;
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
  std::cout << "***** Polynomial Calculation *****\n";
  std::cout << "* 0. Quit                        *\n";
  std::cout << "* 1. Create a reduced polynomial *\n";
  std::cout << "* 2. Polynomial addition         *\n";
  std::cout << "* 3. Polynomial subtraction      *\n";    
  std::cout << "* 4. Polynomial multiplication   *\n";
  std::cout << "**********************************\n";
  std::cout << "Input a command: ";
} 
