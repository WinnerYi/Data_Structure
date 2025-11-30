//11327217 蔡易勳     11327222 林瑋倫 
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono> // count time
void PrintTitle();
void SkipSpace(std::string &str);
std::string ReadInput();
bool isNonNegInt (std::string s);
/*
class Queue {
 private: 
  struct QueueNode {
   Coordinate coordinate_item;
   QueueNode *next;
  }; // end QueueNode
  QueueNode *backPtr;
  QueueNode *frontPtr;
 public: 
  Queue(); // Constructors and destructor
  Queue(const Queue& Q);
  ~Queue();
  bool isEmpty() const; // Queue operations
  void enqueue(const Coordinate& newItem);
  void dequeue();
  void dequeue(Coordinate& queueFront);
  void getFront(Coordinate& queueFront) const;
}; */

class Order {
 private:
  int oid;
  int arrival;
  int duration;
  int timeout;
public:
 Order() {}
 ~Order() {}
 void setOrder(int oid, int arrival, int duration, int timeout) {
   this->oid = oid;
   this->arrival = arrival;
   this->duration = duration;
   this->timeout = timeout; 
 }
int getOid() {
   return oid;
 }
 int getArrival() {
   return arrival;
 }
 int getDuration() {
   return duration;
 }
 int getTimeout() {
   return timeout;
 }

 void print() {
    std::cout << "\t" << oid
              << "\t" << arrival
              << "\t" << duration
              << "\t" << timeout << std::endl;
    }
};
class AbortTimeOut {
 private:
  std::vector<Order> sorted_orders;
  std::string filNum;
  std::string title;
  int total_order = 0;
  long long writing_data_time = 0;
  long long sorting_data_time = 0;
  long long reading_data_time = 0;
 public:
  AbortTimeOut() {}
  ~AbortTimeOut () {}
  bool fetchFile() {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream in;
    std:: cout << "Input a file number (e.g., 401, 402, 403, ...): ";
    std::string file_num = ReadInput();
    std::string txt_path = "sorted" + file_num + ".txt";
    in.open(txt_path);
    if(in.fail()){ 
      std::cout << std::endl << txt_path + " does not exist!\n" << std::endl;
      return false; 
    }
    filNum = file_num;
    std::getline(in, title);
    int oid, arrival, duration, timeout;
    while (in >> oid >> arrival >> duration >> timeout) {
      Order o;
      o.setOrder(oid, arrival, duration, timeout);
      sorted_orders.push_back(o);
      total_order++;
    }       
    in.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    reading_data_time = time.count();
    return true;
  }

};

class Goods {
 private:
  std::vector<Order> orders;
  std::string title;
  int total_order = 0;
  
  std::string filNum;
  long long writing_data_time = 0;
  long long sorting_data_time = 0;
  long long reading_data_time = 0;
 public:
  Goods(){};
  ~Goods(){};
  void ResetGoods() {
    total_order = 0;
    orders.clear();
    orders.shrink_to_fit();

  }
  void addOrder(const Order& o) {
    orders.push_back(o);
    total_order += 1;
  }

  bool fetchFile() {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream in;
    std:: cout << "Input a file number (e.g., 401, 402, 403, ...): ";
    std::string file_num = ReadInput();
    std::string txt_path = "input" + file_num + ".txt";
    in.open(txt_path);
    if(in.fail()){ 
      std::cout << std::endl << txt_path + " does not exist!\n" << std::endl;
      return false; 
    }
    filNum = file_num;
    std::getline(in, title);
    int oid, arrival, duration, timeout;
    while (in >> oid >> arrival >> duration >> timeout) {
      Order o;
      o.setOrder(oid, arrival, duration, timeout);
      orders.push_back(o);
      total_order++;
    }       
    in.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    reading_data_time = time.count();
    return true;
  }

  void ShellSort() {// 希爾
    auto start = std::chrono::high_resolution_clock::now();
    int gap = total_order / 2;

    for (; gap > 0; gap /= 2) {
        // 插入排序法（gap 間隔）
        for (int i = gap; i < total_order; i++) {
            Order tmp = orders[i];
            int j = i;

            while (j >= gap && tmp.getArrival() < orders[j - gap].getArrival()) {
                orders[j] = orders[j - gap];
                j -= gap;
            }

            orders[j] = tmp;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    sorting_data_time = time.count();
}


  void OutputFile() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string sorting_path = "sorted" + filNum + ".txt";
     std::ofstream outputFile(sorting_path);
     outputFile << title << "\n";
     for (int i = 0; i < total_order; i++) {
       outputFile << orders[i].getOid() << " " ;
       outputFile << orders[i].getArrival() << " " ;
       outputFile << orders[i].getDuration() << " " ;
       outputFile << orders[i].getTimeout() << "\n";
     }
     auto end = std::chrono::high_resolution_clock::now();
     auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
     writing_data_time = time.count();

  }

  void printOrders() {
        std::cout << "\t" <<title << std::endl;
        for (int i = 0; i < total_order; i++) {
          std::cout << "(" << i+1 << ")";
          orders[i].print();
        }
  }

  void taskOne() {
    printf("\n");
    printOrders();
    printf("\n");
    ShellSort();
    OutputFile();
    std::cout << "Reading data: " << reading_data_time << " us\n";
    printf("\n");
    std::cout << "Sorting data: " << sorting_data_time << " us\n";
    printf("\n");
    std::cout << "Writing data: " << writing_data_time << " us\n";





  }

};


int main() {
  Goods goods;
  while (true) {
    PrintTitle();
    std::string cmd = ReadInput();
    
    if (cmd == "0") {
      return 0;
    } else if (cmd == "1") {
      printf("\n");
      goods.ResetGoods();
      if (goods.fetchFile()) {
        goods.taskOne();
      }
      
    } else if (cmd == "2") {
  
    } else if (cmd == "3") {
     
    } else if (cmd == "4") {
      
    } else {
      printf("\n");
      std::cout << "Command does not exist!\n";
    }
    printf("\n");  
  }
}
/*

Queue::Queue() {
  backPtr = NULL;   
  frontPtr = NULL;  
}

Queue::~Queue() {
  while (!isEmpty()) { 
    dequeue();
  }
}

bool Queue::isEmpty() const{
   
  if (backPtr == NULL)
    return true;
  else
   return false;
}


void Queue::enqueue(const Coordinate& newItem){
    QueueNode* newPtr = new QueueNode;  
    newPtr->coordinate_item = newItem;              
    newPtr->next = NULL;                 

    if (isEmpty()) {
      
        frontPtr = newPtr;
    } 
    else {
       
        backPtr->next = newPtr;
    }
    backPtr = newPtr; 
}

void Queue::dequeue() {
    if (isEmpty())
        throw std::runtime_error("Queue is empty, cannot dequeue.");
    else {
        QueueNode* tempPtr = frontPtr;   // 
        //  如果佇列裡只有一個節點
        if (frontPtr == backPtr) {
            frontPtr = NULL;  
            backPtr  = NULL;  
        }
        else {
            // 如果有多個節點，就讓前端指向下一個節點
            frontPtr = frontPtr->next;   
        }
        // 安全起見，斷開要刪除節點的連結
        tempPtr->next = NULL;             
        // 釋放被刪除的節點記憶體
        delete tempPtr;                   
    }
}

void Queue::getFront(Coordinate& queueFront) const {
    if (isEmpty())
        throw std::runtime_error("Queue is empty, cannot dequeue.");
    // 否則，把前端節點的資料取出放進參數
    queueFront = frontPtr->coordinate_item;
}

void Queue::dequeue(Coordinate& queueFront){
    if (isEmpty())
        throw std::runtime_error("Queue is empty, cannot dequeue.");
    queueFront = frontPtr->coordinate_item;
    dequeue();
} 
*/
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
  std::cout << "** Simulate FIFO Queues by SQF ***\n";
  std::cout << "* 0. Quit                        *\n";
  std::cout << "* 1. Sort a file                 *\n";
  std::cout << "* 2. Simulate one FIFO queue     *\n";
  std::cout << "* 3. Simulate two queues by SQF  *\n";    
  std::cout << "* 4. Simulate some queues by SQF *\n";
  std::cout << "**********************************\n";
  std::cout << "Input a command(0, 1, 2, 3, 4): ";
} 


