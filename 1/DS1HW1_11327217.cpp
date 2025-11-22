// 11327217 蔡易勳
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <climits>
#include <iomanip>
int g_permutation = 0; // global 
int g_combination = 0;
int count_inner_recursion = 0;
// Function Declaration
std::vector<unsigned long long> CalculateFibonacci_loop(int number); // long long 型態為64位元，能儲存非常大的整數，比int（通常是32位元)big
void CalculateFibonacci_recursion(std::vector<unsigned long long> &fib, int &now_index, int number);
void PrintTitle();
void RecursionPermutation (int temps[10], int total, int number, bool has_used[10]); // total means 找到了多少數字
void RecursionCombination(int temps[10], int total, int number, int start);
long long InnerRecursionFactor (long long sqrt_value, long long fib, long long &big_factor);
// Class Declaration
class Operations { 
 private:
 public:
  Operations() {}
  ~Operations() {}
  void IterativeFibonacci() { // cmd == 1
    while (true) {
      int number = 0;
      std::cout << "Input a number: ";
      std::cin >> number;
      if (std::cin.fail()) { // **判斷是否輸入非數字
        std::cin.clear();// 清除錯誤狀態  
        std::cin.ignore(10000, '\n'); // 丟棄輸入緩衝區裡的資料，直到遇到換行字元 \n 或最多丟棄 10000 個字元。
        std::cout << std::endl;
        continue;
      }
      if (number > 999999 || number == 0) {
        std::cout << "### The number must be in [1,999999] ###\n\n";
        continue;
      } else if (number < 0) {
        std::cout << std::endl;
        continue;
      }
      std::vector<unsigned long long> fib = CalculateFibonacci_loop(number);
      for (int i = 0; i < fib.size(); i++) {  
        long long sqrtfib = (long long)(std::sqrt(fib[i])); // sqrt返還值為double, 要轉成int值
        long count_inner_loops = 1;
        long long factor = 0;
        while (1) {
          if (fib[i] % sqrtfib == 0) {
            factor =  fib[i] / sqrtfib;
            break;
          }
          if (fib[i] % 2 == 1 && sqrtfib % 2 == 1) { // 奇數
            sqrtfib -= 2;
          } else {
            sqrtfib -= 1;
          }
         
          count_inner_loops++;
        }
        // 16 24 32 40 48 
        std::cout << "[" << std::setw(3) << i+1 << "] " << fib[i] << " = " << sqrtfib << " * " << factor << "\t";
        std::cout << "(Inner loop:          " << count_inner_loops << " times)\n";      
        if (fib[i] > LLONG_MAX) {
          std::cout << "Stop here due to ULLONG overflow\n";
          break;
        }
      }
      int count_outer_loops = fib.size()-1;
      std::cout << "<Outer loop:" << std::setw(4)  << count_outer_loops << " times>\n";
      break;
    }
  }
  void RecursiveFibonacci() { // cmd == 2
    while (true) {
      int number = 0;
      std::cout << "Input a number: ";
      std::cin >> number;
      if (std::cin.fail()) { // **判斷是否輸入非數字
        std::cin.clear();// 清除錯誤狀態
        std::cin.ignore(10000, '\n'); // 丟棄輸入緩衝區裡的資料，直到遇到換行字元 \n 或最多丟棄 10000 個字元。
        std::cout << std::endl;
        continue;
      }
      if (number > 999999 || number == 0) {
        std::cout << "### The number must be in [1,999999] ###\n\n";
        continue;
      } else if (number < 0) {
        std::cout << std::endl;
        continue;
      }
      std::vector<unsigned long long> fib ;
      fib.push_back(1);
      fib.push_back(2);
      int now_index = 2;
      CalculateFibonacci_recursion(fib, now_index, number);
      for (int i = 0; i < now_index; i++) {
         std::cout << "[" << std::setw(3) << i+1 << "] " << fib[i] << " = ";
        long long big_factor = 0;
        long long small_factor = InnerRecursionFactor ((long long)sqrt(fib[i]), fib[i], big_factor);
        std::cout << small_factor << " * " << big_factor << "\t";
        std::cout << "(Inner recursion:          " << count_inner_recursion << " times)\n";
        
      }
      if (number > now_index) {
        std::cout << "Stop here due to ULLONG overflow\n";
      } else {
        std::cout << "<Outer recursion:" << std::setw(4)  << now_index << " times>\n";
      }
      break;
    }
  }
  void Combination() { // cmd == 3
    while (true) {
      int number = 0;
      std::cout << "Input a number: ";
      std::cin >> number;
      if (std::cin.fail()) { // **判斷是否輸入非數字
        std::cin.clear();// 清除錯誤狀態
        std::cin.ignore(10000, '\n'); // 丟棄輸入緩衝區裡的資料，直到遇到換行字元 \n 或最多丟棄 10000 個字元。
        std::cout << std::endl;
        continue;
      }
      if (number == 0 || number > 10) {
        std::cout << "### The number must be in [1,10] ###\n\n";
        continue;
      } else if (number < 0){
        printf("\n");
        continue;
      }
      int temps[10] = {0};
      RecursionCombination(temps, 0, number, 0);
      std::cout << "Mission 3: " << g_combination << " combinations\n";
      break;
      
    }
  }
  void Permutation() { // cmd == 4
     while (true) {
      int number = 0;
      std::cout << "Input a number: ";
      std::cin >> number;
      if (std::cin.fail()) { // **判斷是否輸入非數字
        std::cin.clear();// 清除錯誤狀態
        std::cin.ignore(10000, '\n'); // 丟棄輸入緩衝區裡的資料，直到遇到換行字元 \n 或最多丟棄 10000 個字元。
        std::cout << std::endl;
        continue;
      }
      if (number == 0 || number > 10) {
        std::cout << "### The number must be in [1,10] ###\n\n";
        continue;
      } else if (number < 0){
        printf("\n");
        continue;
      }
      int temps[10] = {0}; // 暫存數字，最多10個數字
      bool has_used[10] = {false}; // 看0~9數字有沒有用過
      RecursionPermutation(temps, 0, number, has_used);
      std::cout << "Mission 4: " << g_permutation << " permutations\n";
      break;
     
    }

  }

};


// Main Function
int main() {
  Operations Operations;
  while (true) {
    g_permutation = 0;
    g_combination = 0;
    int cmd = 0;
    PrintTitle();
    double START, END;
    std::cin >> cmd;
    printf("\n");
    if (cmd == 0) {
      return 0;
    }
    else if (cmd == 1) {
      Operations.IterativeFibonacci();
    } else if (cmd == 2) {
      Operations.RecursiveFibonacci();
      
    } else if (cmd == 3) {
      START = clock();
      Operations.Combination();
      END = clock();
      std::cout << "T = " <<  (double)clock()/1000 << " ms\n";
    } else if (cmd == 4) {
      START = clock();
      Operations.Permutation();
      END = clock();
      std::cout << "T = " <<  (double)clock()/1000 << " ms\n";
      
    } else {
      std::cout << "Command does not exist!\n";
    }
    printf("\n"); 
  }

}
void PrintTitle () {
  std::cout << "*** (^_^) Data Structure (^o^) ***\n";
  std::cout << "*** Fibonacci Series Generator ***\n";
  std::cout << "* 0. Quit                        *\n";
  std::cout << "* 1. Iterative Fib. generation   *\n";
  std::cout << "* 2. Recursive Fib. generation   *\n";
  std::cout << "* 3. M-digit Combination in 0..9 *\n";    
  std::cout << "* 4. M-digit Permutation in 0..9 *\n";
  std::cout << "**********************************\n";
  std::cout << "Input a command(0, 1, 2, 3, 4): ";
} 

std::vector<unsigned long long> CalculateFibonacci_loop(int number) { // outerloop
  std::vector<unsigned long long> fib;
  fib.push_back(1);
  if (number == 1) {
    return fib;
  }
  fib.push_back(2);
  for (int i = 2; i < number; i++) {
    
    fib.push_back(fib[i-1] + fib[i-2]);
    if (fib[fib.size()-1] > LLONG_MAX) break;
  }
  return fib; 
}

void CalculateFibonacci_recursion(std::vector<unsigned long long> &fib, int &now_index, int number){ // outerrecursion
  if (now_index >= number) {
    now_index = number;
    return;
  }  
  fib.push_back(fib[now_index-1] + fib[now_index-2]);
  if (fib[fib.size()-1] > LLONG_MAX) return;
  now_index++;
  CalculateFibonacci_recursion(fib, now_index, number);
}

void RecursionPermutation (int temps[10], int total, int number, bool has_used[10]){
  if (total == number) {
    g_permutation++;
    std::cout << "[" << g_permutation << "] " ;
    for (int i = 0; i < total; i++) {
      std::cout << temps[i];
      if (i != total - 1) std::cout << " ";
    }
    printf("\n");
    return;
  }
  for (int i = 0; i <= 9; i++) {
    if (has_used[i] == false) { //  這數字沒被用過的話
      temps[total] = i;
      has_used[i] = true;
      RecursionPermutation(temps, total + 1, number, has_used);
      has_used[i] = false; // 回朔
    } 
  }
}
void RecursionCombination(int temps[10], int total, int number, int start) {
    if (total == number) {
        g_combination++;
        std::cout << "[" << g_combination << "] ";
        for (int i = 0; i < total; i++) {
            std::cout << temps[i];
            if (i != total - 1) std::cout << " ";
        }
        printf("\n");
        return; 
    }
    for (int i = start; i <= 9; i++) {
        temps[total] = i;
        RecursionCombination(temps, total + 1, number, i + 1);
    }
}

long long InnerRecursionFactor (long long sqrt_value, long long fib, long long &big_factor) {
  if (fib % sqrt_value == 0) {
    big_factor = fib / sqrt_value;
    return sqrt_value;
  }
  count_inner_recursion ++;
  if (fib % 2 == 1 && sqrt_value % 2 == 1) {
    return InnerRecursionFactor (sqrt_value-2, fib, big_factor);
  } else {
    return InnerRecursionFactor (sqrt_value-1, fib, big_factor);
  }

}