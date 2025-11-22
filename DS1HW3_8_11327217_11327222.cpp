//11327217 蔡易勳     11327222 林瑋倫 
#include <iostream>
#include <string>
#include <fstream>

/*
原始迷宮所採用的字符定義如下：
1. 字符'O'表示障礙物 (Obstacle)
2. 字符'E'表示空格 (Empty cell)
3. 字符'G'表示目標 (Goal)，抵達這個位置後，也視同一個可以行走的空格。
   程式輸出所採用的字符定義如下：
4. 字符'V'表示曾經走過的位置 (Visited)
5. 字符'R'表示成功可行的路徑 (Route)
*/
struct Coordinate {
  int y, x;  // 往下是正, 往右是正
};

void PrintTitle();
void SkipSpace(std::string &str);
std::string ReadInput();
bool isNonNegInt (std::string s);


// Class Declaration

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

class Stack{
 private:
  struct StackNode{ 
    Coordinate coordinate_item; // a coordinate(x, y)is an coordinate_item
    StackNode *next;
  }; // end StackNode
  StackNode *topPtr;

 public:
  Stack();
  Stack(const Stack& aStack);
  
  ~Stack();
  bool isEmpty();
  void push(const Coordinate& newcoordinate_Item);
  void pop();
  void getTop(Coordinate& stackTop);
  void pop(Coordinate& stackTop);
}; // end Stack
class Maze {
  private:
    char **maze_grid; // 採用 C++標準函式 new 動態配置陣列以儲存迷宮。
    char **visited_grid;
    char **route_grid;
    bool can_go_to_goal = false;
    int maze_rows;
    int maze_columns;
  public:
    Maze() {}
    ~Maze() {
      // deleteMaze();
    }
    void deleteMaze() {
      for (int i = 0; i < maze_rows; i++) {
        delete[] maze_grid[i];
        delete[] visited_grid[i];
        delete[] route_grid[i];
      }
      delete[] maze_grid;
      delete[] visited_grid;
      delete[] route_grid;
      can_go_to_goal = false;
    }
    void resetVisitRoutine() {
      for (int i = 0; i < maze_rows; i++) {
        delete[] visited_grid[i];
        delete[] route_grid[i];
      }
      delete[] visited_grid;
      delete[] route_grid;
      visited_grid = new char*[maze_rows];
      route_grid = new char*[maze_rows];
      for (int i = 0; i < maze_rows; i++) {
        visited_grid[i] = new char[maze_columns];
        route_grid[i] = new char[maze_columns];
        for (int j = 0; j < maze_columns; j++) {
          visited_grid[i][j] = maze_grid[i][j];
          route_grid[i][j] = maze_grid[i][j];
        }
      } 
    }
    bool fetchFile() {
      std::ifstream in;
      std:: cout << "Input a file number: ";
      std::string file_num = ReadInput();
      std::string txt_path = "input" + file_num + ".txt";
      in.open(txt_path);
      if(in.fail()){ 
        std::cout << std::endl << txt_path + " does not exist!\n" << std::endl;
        return false; 
      }
      in >>  maze_columns >> maze_rows;
      maze_grid = new char*[maze_rows];
      visited_grid = new char*[maze_rows];
      route_grid = new char*[maze_rows];
      for (int i = 0; i < maze_rows; i++) {
        maze_grid[i] = new char[maze_columns];
        visited_grid[i] = new char[maze_columns];
        route_grid[i] = new char[maze_columns];
        for (int j = 0; j < maze_columns; j++) {
          in >> maze_grid[i][j];
          visited_grid[i][j] = maze_grid[i][j];
          route_grid[i][j] = maze_grid[i][j];
        }
      } 
      in.close();
      return true;
    }
    void bestRoutine(Stack path) {
      Coordinate temp_coor;
      while (!path.isEmpty()) {
        path.getTop(temp_coor);
        if (route_grid[temp_coor.y][temp_coor.x]!= 'G') route_grid[temp_coor.y][temp_coor.x] = 'R';
        if (maze_grid[temp_coor.y][temp_coor.x]== 'G') {
          route_grid[temp_coor.y][temp_coor.x] = 'G';
          visited_grid[temp_coor.y][temp_coor.x]= 'G';
        }
        path.pop();
      }
    }
    void Dfs() { // for task 1
      Stack path;
      Coordinate start;
      start.y = 0;
      start.x = 0;// y first, then x    
      path.push(start);  
      int dx[4] = {1, 0, -1, 0}; // 右下左上
      int dy[4] = {0, 1, 0, -1};  

      int dir = 0; //右邊開始
    
      while (!path.isEmpty()) {
        Coordinate cur;
        path.getTop(cur);
        
        if (visited_grid[cur.y][cur.x] == 'G') {
          can_go_to_goal = true;
          if (!path.isEmpty())  {
            path.pop();
          }
          bestRoutine(path);
          
          return;
        }
        visited_grid[cur.y][cur.x] = 'V';
        bool moved = false;
        for (int i = 0; i < 4; i++) {
          int ndir = (dir + i) % 4;
          
          int nx = cur.x + dx[ndir];
          int ny = cur.y + dy[ndir];
          
          if ((0 <= nx && nx < maze_columns) && (0 <= ny && ny < maze_rows) && (visited_grid[ny][nx] != 'V') && (visited_grid[ny][nx] != 'O')) {
            Coordinate temp;
            temp.y = ny;
            temp.x = nx;
            dir = ndir;
            if (visited_grid[temp.y][temp.x] == 'G') {
              can_go_to_goal = true;
              bestRoutine(path);
              return;
            } else {
              visited_grid[temp.y][temp.x] = 'V';
            }
       
            path.push(temp);
            moved = true;
            break;
          }
        }
        if (!moved) {
          path.pop();
        }
      }
      can_go_to_goal = false;
    }
    void Dfs2(int int_input_goals, bool &success) { // for task 2
      bool has_visited_goal[maze_rows][maze_columns];
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          has_visited_goal[i][j] = false;
        }
      }
      resetVisitRoutine();
      int count_goal = 0;
      Stack path;
      Coordinate start;
      start.y = 0;
      start.x = 0;// y first, then x    
      path.push(start);  
      int dx[4] = {1, 0, -1, 0}; // 右下左上
      int dy[4] = {0, 1, 0, -1};  

      int dir = 0; //右邊開始
    
      while (!path.isEmpty()) {
        Coordinate cur;
        path.getTop(cur);
        
        if (visited_grid[cur.y][cur.x] == 'G' && !has_visited_goal[cur.y][cur.x]) {
          count_goal++;
          has_visited_goal[cur.y][cur.x] = true;
          if (count_goal == int_input_goals) {
            success = true;
            if (!path.isEmpty())  {
              path.pop();
            }
            bestRoutine(path);
            return;
          }
        }
        visited_grid[cur.y][cur.x] = 'V';
        bool moved = false;
        for (int i = 0; i < 4; i++) {
          int ndir = (dir + i) % 4;
          
          int nx = cur.x + dx[ndir];
          int ny = cur.y + dy[ndir];
          
          if ((0 <= nx && nx < maze_columns) && (0 <= ny && ny < maze_rows) && (visited_grid[ny][nx] != 'V') &&(visited_grid[ny][nx] != 'O')) {
            Coordinate temp;
            temp.y = ny;
            temp.x = nx;
            dir = ndir;
            if (visited_grid[temp.y][temp.x] == 'G' && !has_visited_goal[temp.y][temp.x]) {
              count_goal++;
              has_visited_goal[temp.y][temp.x] = true;
              if (count_goal == int_input_goals) {
                success = true;
                bestRoutine(path);
                return;
              }
            } else {
              visited_grid[temp.y][temp.x]= 'V';
            }
       
            path.push(temp);
            moved = true;
            break;
          }
        }
        if (!moved) {
          path.pop();
        }
      }
      success = false;
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          if (maze_grid[i][j] == 'G') visited_grid[i][j] = 'G';
        }
      }
    }
    void Dfs3(int &total_goal) { // for task 3
      resetVisitRoutine();
      Stack path;
      Coordinate start;
      start.y = 0;
      start.x = 0;// y first, then x    
      path.push(start);  
      int dx[4] = {1, 0, -1, 0}; // 右下左上
      int dy[4] = {0, 1, 0, -1};  

      int dir = 0; //右邊開始
    
      while (!path.isEmpty()) {
        Coordinate cur;
        path.getTop(cur);
        
        if (visited_grid[cur.y][cur.x] == 'G') {
          total_goal++;
        }
        visited_grid[cur.y][cur.x] = 'V';
        bool moved = false;
        for (int i = 0; i < 4; i++) {
          int ndir = (dir + i) % 4;
          
          int nx = cur.x + dx[ndir];
          int ny = cur.y + dy[ndir];
          
          if ((0 <= nx && nx < maze_columns) && (0 <= ny && ny < maze_rows) && (visited_grid[ny][nx] != 'V') &&(visited_grid[ny][nx] != 'O')) {
            Coordinate temp;
            temp.y = ny;
            temp.x = nx;
            dir = ndir;
            if (visited_grid[temp.y][temp.x] == 'G') {
              total_goal++;
            } 
            visited_grid[temp.y][temp.x]= 'V';
            path.push(temp);
            moved = true;
            break;
          }
        }
        if (!moved) {
          path.pop();
        }
      }
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          if (maze_grid[i][j] == 'G') visited_grid[i][j] = 'G';
        }
      }
    }





    void Dfs4() { // for task 4 (test) + backtracking
     int count_path = 0;
      Stack path;
      Coordinate start;
      start.y = 0;
      start.x = 0;// y first, then x    
      path.push(start);  
      bool visited[maze_rows][maze_columns];
      int dx[4] = {1, 0, -1, 0}; // 右下左上
      int dy[4] = {0, 1, 0, -1};  
      int dir = 0; //右邊開始
      int dist[maze_rows][maze_columns];
      Coordinate prev[maze_rows][maze_columns];
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          dist[i][j] = 0;
          prev[i][j].y = -1;
          prev[i][j].x = -1;
          visited[i][j] = false;
        }
      }
      dist[start.y][start.x] = 1;
      visited_grid[0][0] = 'V';
      visited[0][0] = true;
      Coordinate cur; 
      
      while (!path.isEmpty()) {
        path.getTop(cur);
        bool moved = false;
        for (int i = 0; i < 4; i++) {
          int ndir = (dir + i) % 4;
          int nx = cur.x + dx[ndir];
          int ny = cur.y + dy[ndir];
          if ((0 <= nx && nx < maze_columns) && (0 <= ny && ny < maze_rows) && (visited_grid[ny][nx] == 'E' || visited_grid[ny][nx] == 'G')) {
            Coordinate next;
            next.y = ny;
            next.x = nx;
            visited[ny][nx] = true;
            dir = ndir;
            prev[ny][nx] = cur;
            dist[ny][nx] = dist[cur.y][cur.x] + 1;
            count_path++;

            if (visited_grid[next.y][next.x] == 'G') {
              can_go_to_goal = true;
              break;
              
            } else {
              if (visited_grid[ny][nx] != 'G'){
                visited_grid[ny][nx] = 'V';
              }
            }
            path.push(next);
            moved = true;
            break;
          }
        }
        if (can_go_to_goal) break;
        if (!moved) {
          path.pop();
        }
      }

      int dist_temp[maze_rows][maze_columns];
      for (int i = 0; i < maze_rows; i++) {
          for (int j = 0; j < maze_columns; j++) {
            dist_temp[i][j] =  dist[i][j];
          }
      }

      dir = 0;
      while (!path.isEmpty()) { 
        /*for (int i = 0; i < maze_rows; i++) {
          for (int j = 0; j < maze_columns; j++) {
            if (dist_temp[i][j] != 0)  dist[i][j] = dist_temp[i][j];
          }
        }*/
        /*for (int i = 0; i < maze_rows; i++) {
          for (int j = 0; j < maze_columns; j++) {
            std::cout << dist[i][j] << "    ";
          }
          printf("\n");
        }*/
       
        bool moved = false;   
        path.getTop(cur);
        if (dist[cur.y][cur.x] >= count_path) {
          path.pop();
          continue;
        }
        
        for (int i = 0; i < 4; i++) {
          int ndir = (dir + i) % 4;
          int nx = cur.x + dx[ndir];
          int ny = cur.y + dy[ndir];
          

          if ((0 <= nx && nx < maze_columns) && (0 <= ny && ny < maze_rows) &&
          ((visited_grid[ny][nx] == 'E' || visited_grid[ny][nx] == 'G') ||
         (visited_grid[ny][nx] == 'V' && 
        ((dist[ny][nx] - dist[cur.y][cur.x]) != 1 && (dist[ny][nx] - dist[cur.y][cur.x]) != -1 && dist_temp[ny][nx] == 0)))) {

            Coordinate next;
            next.y = ny;
            next.x = nx;
            dir = ndir;
            dist[ny][nx] = dist[cur.y][cur.x] + 1;
         

            if (visited_grid[next.y][next.x] == 'G' ) {
              if (dist[cur.y][cur.x] < count_path) {
                count_path = dist[cur.y][cur.x];
              }
              moved = true;
            } else {
              
              visited_grid[ny][nx] = 'V';
              
            }
            if (dist[ny][nx] < count_path) {
               moved = true;
               path.push(next);
               break;
            }
          }
        }
        if (!moved) {
          path.pop();
        }

     }
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          std::cout << visited_grid[i][j];
        }
        printf("\n");
      }
    }
    void assist_Dfs4 () {




    }





    void Bfs() { // for task4
      Queue q;
      Coordinate start;
      start.y = 0;
      start.x = 0;// y first, then x  
      q.enqueue(start);

      int dx[4] = {-1, 0, 1, 0}; 
      int dy[4] = {0, -1, 0, 1};

      int dist[maze_rows][maze_columns];
      Coordinate prev[maze_rows][maze_columns];
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          dist[i][j] = -1;
          prev[i][j].y = -1;
          prev[i][j].x = -1;
        }
      }
      dist[start.y][start.x] = 1;
      visited_grid[start.y][start.x] = 'V';
      Coordinate goal;
      while (!q.isEmpty()) {
        Coordinate cur;
        q.dequeue(cur);

        if (visited_grid[cur.y][cur.x] == 'G') {
          can_go_to_goal = true;
          goal = cur;
          break;
        }

        for (int k = 0; k < 4; k++) {
          int ny = cur.y + dy[k];
          int nx = cur.x + dx[k];
          if (ny >= 0 && ny < maze_rows && nx >= 0 && nx < maze_columns &&
              (visited_grid[ny][nx] == 'E' || visited_grid[ny][nx] == 'G') &&
              dist[ny][nx] == -1) {
            dist[ny][nx] = dist[cur.y][cur.x] + 1;
            prev[ny][nx] = cur;
            Coordinate temp;
            temp.y = ny;
            temp.x = nx;
            q.enqueue(temp);
            if (visited_grid[ny][nx] != 'G')
              visited_grid[ny][nx] = 'V';
          }
        }
      }
  
      if (can_go_to_goal) {
        printf("\n");
        Coordinate cur = goal;
        while (!(cur.y == -1 && cur.x == -1)) {
          if (visited_grid[cur.y][cur.x] != 'G')
            route_grid[cur.y][cur.x] = 'R';
          cur = prev[cur.y][cur.x];
        }
        for (int i = 0; i < maze_rows; i++) {
          for (int j = 0; j < maze_columns; j++) {
            std::cout << route_grid[i][j];
          }
          printf("\n");
        }
        printf("\n");
        std::cout << "Shortest path length = " << dist[goal.y][goal.x];
        printf("\n\n");
      } else {
        std::cout << "\n\n### There is no path to find a goal! ### \n" << std::endl;
      }
    }

    void taskOne() { // 從左上角出發(依照指定行走模式)走到目標 G 的一條路徑
      Dfs();
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          std::cout << visited_grid[i][j];
        }
        printf("\n");
      } 
      
      if (can_go_to_goal) {
        printf("\n");
        for (int i = 0; i < maze_rows; i++) {
          for (int j = 0; j < maze_columns; j++) {
            std::cout << route_grid[i][j];
          }
          printf("\n");
        } 
      }
      printf("\n\n");
    }

    void taskTwo() { // 從左上角出發(依照指定行走模式)走過 N 個目標的一條路徑
      int int_goal_input;
      bool success = false;
      std::string goal_input;
      while (1) {
        std::cout << "Number of G (goals): ";
        goal_input = ReadInput();
        
        if (isNonNegInt(goal_input)) {
          int_goal_input = std::stoi(goal_input);
          if (int_goal_input < 1 || int_goal_input > 100) {
            std::cout << "\n### The number must be in [1,100] ###\n\n";
            continue;
          }
          break;
        } else {
          printf("\n");
          continue;
        }
      }
      Dfs2(int_goal_input, success);
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          std::cout << visited_grid[i][j];
        }
        printf("\n");
      } 
      
      if (success) {
        printf("\n");
        for (int i = 0; i < maze_rows; i++) {
          for (int j = 0; j < maze_columns; j++) {
            std::cout << route_grid[i][j];
          }
          printf("\n");
        } 
      }
      printf("\n");
    }

    void taskThree() { // 從左上角出發(依照指定行走模式)走過所有目標 G 以計算總數
      int total_goal = 0;
      Dfs3(total_goal);
      for (int i = 0; i < maze_rows; i++) {
        for (int j = 0; j < maze_columns; j++) {
          std::cout << visited_grid[i][j];
        }
        printf("\n");
      } 
      std::cout << "\nThe maze has " << total_goal << " goal(s) in total.\n\n";
    }

    void taskFour() { // 從左上角出發走到目標 G 的一條最短路徑
      Dfs4();
      resetVisitRoutine();
      Bfs();
    }
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

// QUENE 跟 Stack 都是來自老師的簡報

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
Stack::Stack(const Stack& aStack) {
  if (aStack.topPtr == NULL) {
    topPtr = NULL; // original list is empty
  } else {
    topPtr = new StackNode;
    topPtr->coordinate_item = aStack.topPtr->coordinate_item;
    StackNode *newPtr = topPtr;
    for (StackNode *origPtr = aStack.topPtr->next; origPtr != NULL; origPtr = origPtr->next) {
       newPtr->next = new StackNode;
       newPtr = newPtr->next;
       newPtr->coordinate_item = origPtr->coordinate_item;
    } // end for
    newPtr->next = NULL;
  } // end if-else
} // end copy construct

Stack::Stack() {
    topPtr = nullptr;
}

Stack::~Stack() {
  while (!isEmpty()) {
    pop();
  }
}
bool Stack::isEmpty() {
  return topPtr == NULL;
}
//bad_alloc 是 C++ 標準例外類別，當 new 失敗時會丟出這個例外。
  //用 catch (bad_alloc& e) 比 catch (bad_alloc e) 好，因為：
  //避免複製例外物件（效率更好）
  //確保多型行為正確
  //e.what() 會輸出錯誤訊息（通常是 "std::bad_alloc"）
void Stack::push(const Coordinate& newcoordinate_Item) {
  try { 
    StackNode *newPtr = new StackNode;
    newPtr->coordinate_item = newcoordinate_Item;
    newPtr->next = topPtr;
    topPtr = newPtr;
  } // end try
   catch (std::bad_alloc& e) {  // ← 用參考 (&)
     std::cerr << "記憶體配置失敗: " << e.what() << std::endl;
   } // end catch
}
void Stack::pop() {
  if (!isEmpty()){ 
    StackNode *temp = topPtr;
    topPtr = topPtr->next;
    temp->next = NULL;
    delete temp;
  } // end if
}

void Stack::getTop(Coordinate& stackTop) {
  if (!isEmpty()) {
    stackTop = topPtr->coordinate_item;
  }
}

void Stack:: pop(Coordinate& stackTop) {
  if (!isEmpty()) {
    stackTop = topPtr->coordinate_item;
    StackNode *temp = topPtr;
    topPtr = topPtr->next;
    temp->next = NULL;
    delete temp;
  } // end if
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
  std::cout << "*** Find the Goal(s) in a Maze ***\n";
  std::cout << "* 0. Quit                        *\n";
  std::cout << "* 1. Find one goal               *\n";
  std::cout << "* 2. Find goal(s) as requested   *\n";
  std::cout << "* 3. How many goals?             *\n";    
  std::cout << "* 4. Shortest path to one goal   *\n";
  std::cout << "**********************************\n";
  std::cout << "Input a command(0, 1, 2, 3, 4): ";
} 
