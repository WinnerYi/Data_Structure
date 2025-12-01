//11327217 蔡易勳     11327222 林瑋倫 
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
bool isNonNegInt (std::string s);



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

class AbortOrder {
 private:
  int oid;
  int cid = 0;
  int delay;
  int abort;
public:
 AbortOrder(int oid, int cid, int delay, int abort) {
   this->oid = oid;
   this->cid = cid;
   this->delay = delay;
   this->abort = abort;
 }
 ~AbortOrder() {}
 void setOrder(int oid, int cid, int delay, int abort) {
   this->oid = oid;
   this->cid = cid;
   this->delay = delay;
   this->abort = abort; 
 }
int getOid() {
   return oid;
 }
 int getCid() {
   return cid;
 }
 int getDelay() {
   return delay;
 }
 int getAbort() {
   return abort;
 }
};

class TimeOutOrder {
 private:
  int oid;
  int cid = 0;
  int delay;
  int departure;
public:
 TimeOutOrder(int oid, int cid, int delay, int departure) {
    this->oid = oid;
    this->cid = cid;
    this->delay = delay;
    this->departure = departure;
 }

 ~TimeOutOrder() {}
 void setOrder(int oid, int cid, int delay, int departure) {
   this->oid = oid;
   this->cid = cid;
   this->delay = delay;
   this->departure = departure; 
 }
int getOid() {
   return oid;
 }
 int getCid() {
   return cid;
 }
 int getDelay() {
   return delay;
 }
 int getDeparture() {
   return departure;
 }
};

class Queue {
 private: 
  struct QueueNode {
   Order order_item;
   QueueNode *next;
  }; // end QueueNode
  QueueNode *backPtr;
  QueueNode *frontPtr;
 public: 
  Queue(); // Constructors and destructor
  Queue(const Queue& Q);
  ~Queue();
  bool isEmpty() const; // Queue operations
  void enqueue(const Order& newItem);
  void dequeue();
  void dequeue(Order& queueFront);
  void getFront(Order& queueFront) const;
  int size();
}; 

struct Chef {
    Queue q;
    int cid;
    int idle_time = 0;
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
      std::cout << std::endl << "### " << txt_path + " does not exist! ###" << std::endl;
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

  void ShellSort() { // 先 Arrival 再 OID
    auto start = std::chrono::high_resolution_clock::now();
    int gap = total_order / 2;

    for (; gap > 0; gap /= 2) {
        for (int i = gap; i < total_order; i++) {
            Order tmp = orders[i];
            int j = i;

            // 雙重比較：先 Arrival，再 OID
            while (j >= gap && 
              (tmp.getArrival() < orders[j - gap].getArrival() ||
              (tmp.getArrival() == orders[j - gap].getArrival() && tmp.getOid() < orders[j - gap].getOid()))) 
            {
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
     outputFile <<"OID\tArrival\tDuration\tTimeOut\n";
     for (int i = 0; i < total_order; i++) {
       outputFile << orders[i].getOid() << "\t" ;
       outputFile << orders[i].getArrival() << "\t" ;
       outputFile << orders[i].getDuration() << "\t" ;
       outputFile << orders[i].getTimeout() << "\n";
     }
     auto end = std::chrono::high_resolution_clock::now();
     auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
     writing_data_time = time.count();

  }

  void printOrders() {
        std::cout << "\tOID\tArrival\tDuration\tTimeOut" << std::endl;
        for (int i = 0; i < total_order; i++) {
          std::cout << "(" << i+1 << ") ";
          orders[i].print();
        }
  }

  void taskOne() {
    printf("\n");
    printOrders();
    printf("\n");
    ShellSort();
    OutputFile();
    std::cout << "Reading data: " << reading_data_time << " us.\n";
    printf("\n");
    std::cout << "Sorting data: " << sorting_data_time << " us.\n";
    printf("\n");
    std::cout << "Writing data: " << writing_data_time << " us.\n";

  }
};

class CancelList {
 private:
  std::vector<Order> sorted_orders;
  std::vector<AbortOrder> abort_orders; 
  std::vector<TimeOutOrder> timeOut_orders;
  std::string filNum;
  std::string title;
  int total_order = 0;
  int total_delay = 0;
 public:
  CancelList () {}
  ~CancelList () {
    total_order = 0;
    sorted_orders.clear();
    sorted_orders.shrink_to_fit();
  }
  void ResetSorted() {
    sorted_orders.clear();
    sorted_orders.shrink_to_fit();
    total_order = 0;

  }

  void ResetCancel() {
    total_delay = 0;
    abort_orders.clear();
    abort_orders.shrink_to_fit();

    timeOut_orders.clear();
    timeOut_orders.shrink_to_fit();

  }
  bool fetchFile() {
    std::ifstream in;
    std:: cout << "Input a file number (e.g., 401, 402, 403, ...): ";
    std::string file_num = ReadInput();
    std::string txt_path = "sorted" + file_num + ".txt";
    in.open(txt_path);
    if(in.fail()){ 
      std::cout << std::endl << "### " << txt_path + " does not exist! ###" << std::endl;
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
    return true;
  }

  void OutputFile(int chef_counts) {
    std::string sorting_path;
    if (chef_counts == 1) {
      sorting_path = "one" + filNum + ".txt";
    } else if (chef_counts == 2) {
      sorting_path = "two" + filNum + ".txt";
    } else {
      sorting_path = "any" + filNum + ".txt";
    }
     
     std::ofstream outputFile(sorting_path);
     outputFile << std::fixed << std::setprecision(2);
     outputFile <<  "\t[Abort List]\n";
     outputFile << "\tOID\tCID\tDelay\tAbort\n";
     for (int i = 0; i < abort_orders.size(); i++) {
       outputFile << "[" << i+1 << "]";
       outputFile << "\t" << abort_orders[i].getOid();
       outputFile << "\t" << abort_orders[i].getCid();
       outputFile << "\t" << abort_orders[i].getDelay();
       outputFile << "\t" << abort_orders[i].getAbort() << "\n";
     }
     outputFile <<  "\t[Timeout List]\n";
     outputFile << "\tOID\tCID\tDelay\tDeparture\n";
     for (int i = 0; i < timeOut_orders.size(); i++) {
       outputFile << "[" << i+1 << "]";
       outputFile << "\t" << timeOut_orders[i].getOid();
       outputFile << "\t" << timeOut_orders[i].getCid();
       outputFile << "\t" << timeOut_orders[i].getDelay();
       outputFile << "\t" << timeOut_orders[i].getDeparture() << "\n";
     }
     outputFile <<  "[Total Delay]\n";
     outputFile << total_delay << " min.\n";
     outputFile << "[Failure Percentage]\n";
  
     double percentage = 100 * (double)(abort_orders.size() + timeOut_orders.size()) / (double)sorted_orders.size() ;
     outputFile << percentage << " %" << std::endl;

  }

 

  void printOrders() {
        std::cout << "\t" <<title << std::endl;
        for (int i = 0; i < total_order; i++) {
          std::cout << "(" << i+1 << ") ";
          sorted_orders[i].print();
        }
  }

  void processOrderFromQueue(Queue &q, int &idle_time) {
    Order front;
    q.dequeue(front);
    int start = idle_time;
    // --- 取出時逾時 → Abort ---
    if (front.getTimeout() < start) {
        int abort_time = start;
        int delay = abort_time - front.getArrival();
        abort_orders.push_back(AbortOrder(front.getOid(), 1, delay, abort_time));
        total_delay += delay;
        return;
    }
    // 做菜後才逾時 → Timeout
    if (start + front.getDuration() > front.getTimeout()) {
        int delay = start - front.getArrival();
        idle_time += front.getDuration();
        timeOut_orders.push_back(TimeOutOrder(front.getOid(), 1, delay, idle_time));
        total_delay += delay;
        return;
    }

    // 正常完成 
    idle_time += front.getDuration();
}

void processOrderFromQueue2(Queue &q, int cid, int &idle_time) {
    Order front;
    q.dequeue(front);
    int start = idle_time;
    // --- 取出時逾時 → Abort ---
    if (front.getTimeout() < start) {
        int abort_time = start;
        int delay = abort_time - front.getArrival();
        abort_orders.push_back(AbortOrder(front.getOid(), cid, delay, abort_time));
        total_delay += delay;
        return;
    }
    // 做菜後才逾時 → Timeout
    if (start + front.getDuration() > front.getTimeout()) {
        int delay = start - front.getArrival();
        idle_time = start + front.getDuration();
        timeOut_orders.push_back(TimeOutOrder(front.getOid(), cid, delay, idle_time));
        total_delay += delay;
        return;
    }

    // 正常完成 
    idle_time += front.getDuration();
}
void taskTwo() {
    int idle_time = 0;
    Queue q;

    for (int i = 0; i < total_order; i++) {
        Order &cur = sorted_orders[i];

        // 不合理  直接取消 
        if (cur.getDuration() <= 0 || cur.getArrival() + cur.getDuration() > cur.getTimeout()) {
            sorted_orders.erase(sorted_orders.begin() + i);
            i--;
            total_order--;
            continue;
        }
        int arrival = cur.getArrival();

        //  若 queue 尚有舊訂單，且 idle_time 剛好等於 arrival → 先清舊單（耗時=0）
        
        while (!q.isEmpty() && idle_time <= arrival) {
            processOrderFromQueue(q, idle_time);
        }
            // 正常完成但耗時=0 → idle_time 不變
        

        // 若廚師空、未達 arrival → 跳到 arrival 
        if (q.isEmpty() && idle_time < arrival)
            idle_time = arrival;

        //  Queue 滿 -> abort
        if (q.size() >= 3 && arrival < idle_time) {
            abort_orders.push_back(AbortOrder(cur.getOid(), 0, 0, arrival));
            
        } else {
          q.enqueue(cur);
        }
    }

    // 處理 queue 中剩餘的訂單 
    while (!q.isEmpty()) {
        processOrderFromQueue(q, idle_time);
    }

    OutputFile(1);
}

void taskTwoMultiChef(int chef_count) {
    Chef chef[chef_count];
    for (int i = 0; i < chef_count; i++) {
      chef[i].cid = i+1;
    }

    for (int i = 0; i < total_order; i++) {
    
        Order &cur = sorted_orders[i];
        //  不合理 → 直接取消 
        if (cur.getDuration() <= 0 || cur.getArrival() + cur.getDuration() > cur.getTimeout()) {
            sorted_orders.erase(sorted_orders.begin() + i);
            i--;
            total_order--;
            continue;
        }
        int arrival = cur.getArrival();
        //  (Case 1 / Case 2) 找「閒置廚師」 idle <= arrival && queue empty

    
        for (int i = 0; i < chef_count; i++) {
          while (!chef[i].q.isEmpty() && chef[i].idle_time <= arrival) {
            processOrderFromQueue2(chef[i].q, chef[i].cid, chef[i].idle_time);
          }
        }

  // 若廚師空、未達 arrival → 跳到 arrival 
        int target = -1;

        // Case1 / Case2：找 idle_time ≤ arrival 且 queue 空的廚師
        std::vector<int> idle_list;
        for (int c = 0; c < chef_count; c++) {
            if (chef[c].idle_time <= arrival && chef[c].q.size() == 0) {
                idle_list.push_back(c);
            }
        }

        if (idle_list.size() == 1) {
            target = idle_list[0]; // 唯一閒置
        }
        else if (idle_list.size() > 1) {
            target = idle_list[0]; // 多個閒置 → 選編號最小
        }
        else {
            // Case3：沒有閒置廚師  選 queue 最短的（且 < 3）
            int best_len = INT_MAX;
            for (int c = 0; c < chef_count; c++) {
                int len = chef[c].q.size();
                if (len < best_len && len < 3) {
                    best_len = len;
                    target = c;
                }
            }

            // Case4：全部 queue 都滿 → abort
            if (target == -1) {
                abort_orders.push_back(AbortOrder(cur.getOid(), 0, 0, arrival));
                continue;
            }
        }

        // -------------------------------------------
        // Step 3：把訂單丟給 target 廚師
        // -------------------------------------------
        chef[target].q.enqueue(cur);

        // 若該廚師是空的而且 idle_time < arrival → 推進時間
        if (chef[target].q.size() == 1 && chef[target].idle_time < arrival) {
            chef[target].idle_time = arrival;
        }
    }
  
    // 處理 queue 中剩餘的訂單 
    
    bool processed;

do {
    // 建立索引陣列
    
    int idx[chef_count];
    // bubble sort idx，依 idle_time 遞增，idle_time 相同時按 cid 遞增
    if (chef_count >= 2) {
      
      for (int i = 0; i < chef_count; i++) idx[i] = i;
      for (int i = 0; i < chef_count - 1; i++) {
          for (int j = 0; j < chef_count - 1 - i; j++) {
              int a = idx[j], b = idx[j+1];
              if (chef[a].idle_time > chef[b].idle_time ||
                (chef[a].idle_time == chef[b].idle_time && chef[a].cid > chef[b].cid)) {
                  int temp = idx[j];
                  idx[j] = idx[j+1];
                  idx[j+1] = temp;
              }
          }
      }
    }
    // 找第一個可以處理的 chef（idle_time 最小且 queue 不空）
    processed = false;
    for (int i = 0; i < chef_count; i++) {
        int c = idx[i];
        // 如果需要考慮 arrival 時間，打開 idle_time <= arrival
        if (!chef[c].q.isEmpty()) {
            processOrderFromQueue2(chef[c].q, chef[c].cid, chef[c].idle_time);
            processed = true;
            break; // 做完一單後重新排序
        }
    }

} while (processed);
    
    OutputFile(chef_count);
}




};


int main() {
  Goods goods;
  CancelList list;
  bool has_command2 = false;
  while (true) {
    PrintTitle();
    int cmd;
    std::cin >> cmd;
    if (std::cin.fail()) { // 檢查輸入是否失敗
        std::cin.clear();              // 清除錯誤標誌
        std::cin.ignore(10000, '\n');  // 忽略錯誤輸入
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
        list.taskTwo();
        has_command2 = true;
      }
      
      list.ResetCancel();
 
  
    } else if (cmd == 3) {
   
      if (!has_command2) {
        std::cout << "\n### Execute command 2 first! ###\n\n";
        continue;
      }
      list.taskTwoMultiChef(2);
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
      if (num == 1) {
        list.taskTwo();
      } else {
        list.taskTwoMultiChef(num);
      }
      list.ResetCancel();
    } else {
      printf("\n");
      std::cout << "Command does not exist!\n";
    }
    printf("\n");  
  }
}

Queue::Queue() {
  backPtr = NULL;   
  frontPtr = NULL;  
}

Queue::Queue(const Queue& Q) : frontPtr(nullptr), backPtr(nullptr) {
    QueueNode* curr = Q.frontPtr;
    while (curr != nullptr) {
        enqueue(curr->order_item);
        curr = curr->next;
    }
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


void Queue::enqueue(const Order& newItem){
    QueueNode* newPtr = new QueueNode;  
    newPtr->order_item = newItem;              
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

void Queue::getFront(Order& queueFront) const {
    if (isEmpty())
        throw std::runtime_error("Queue is empty, cannot dequeue.");
    queueFront = frontPtr->order_item;
}

void Queue::dequeue(Order& queueFront){
    if (isEmpty())
        throw std::runtime_error("Queue is empty, cannot dequeue.");
    queueFront = frontPtr->order_item;
    dequeue();
} 

int Queue::size() {
  int count = 0;
  QueueNode* current = frontPtr;
  while (current != nullptr) {
      count++;
      current = current->next;
  }
  return count;
  
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


