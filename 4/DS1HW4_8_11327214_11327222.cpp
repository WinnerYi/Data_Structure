//11327217 蔡易勳     11327222 林瑋倫 
#include <iostream>
#include <string>
#include <fstream>
//hahaha
void PrintTitle();

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
}; 

int main() {
  Maze maze1; // for task1, 2, 3
  Maze maze4; // for task4
  bool maze1_is_empty = true;
  while (true) {
    PrintTitle();
    std::string cmd = ReadInput();
    
    if (cmd == "0") {
      return 0;
    } else if (cmd == "1") {
      std::cout << std::endl;
      if (!maze1_is_empty) {
        maze1.deleteMaze();
        maze1_is_empty = true;
      }
      if (maze1.fetchFile()) {
        maze1_is_empty = false;
        maze1.taskOne();
      }
      continue;
    } else if (cmd == "2") {
      std::cout << std::endl;
      if (!maze1_is_empty) {
        maze1.taskTwo();
      } else {
        std::cout << "### Execute command 1 to load a maze! ###\n";
      }
      std::cout << std::endl;
      continue;
    } else if (cmd == "3") {
      if (!maze1_is_empty) {
        maze1.taskThree();
        continue;
      } else {
        std::cout << "\n### Execute command 1 to load a maze! ###\n";
      }
    } else if (cmd == "4") {
      std::cout << std::endl;
      if (maze4.fetchFile()) {
        maze4.taskFour();
        maze4.deleteMaze();
      } 
      continue;
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


