/*
    **References**
    (1)lecture slide ch24.
    (2)Blog: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
    (3)Blog: https://www.tutorialspoint.com/cplusplus-program-for-dijkstra-s-shortest-path-algorithm
    (4)stack overflow: https://stackoverflow.com/questions/9445327/fast-way-to-implement-pop-front-to-a-stdvector
    (5)Book: Thomas H. (2009). Introduction to Algorithm 3rd edition, pp 690.
    (5)cplusplus: https://www.cplusplus.com/reference/queue/priority_queue/priority_queue/
    (6)blog: https://www.geeksforgeeks.org/floyd-warshall-algorithm-dp-16/
    (7)blog: https://kpumangyou.tistory.com/21

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <limits.h>
#include <queue>
#include <ctime>
#include <random>
#include <cstdlib>
#include <iomanip>

using namespace std;
#define INF 99999
#define NIL -1

void dijkstra(vector<string> cities, vector<int> adj[], vector<int> list[],vector <pair<int,int>> matrix[],int src);
void InitializeMatrix(vector<int> adj[]); //It will be developed soon
void Floyd(vector<int> adj[], vector<int> list[], vector<int> pred_list[],int size);
double roundToThousandths(double x);
void DisplayMatix(vector<int>list [], int size, vector<string> &cities);

int main(){
  #if 0 // no clock version
  ifstream fin ("hw6_data.txt");
  string line;
  getline(fin, line);
  string temp="";
  vector <string> cities;
  //store cities
  for(int i=0;i<line.length();i++){
    if(line[i] == ' '||line[i] == '\t'||i==line.length()-1){
      if(temp != "")
        cities.push_back(temp);
      temp = "";
    }
    else{
      temp = temp+ line[i];
    }
  }
  //check and print cities
  for(int j=0;j<cities.size();j++){
    cout<< cities[j] << "  ";
  }
  cout << endl;
  int size = cities.size();
  vector<string> adj_s[size];
  vector<int> adj[size];
  temp="";
  for(int j=0;j<size;j++){ // total 10 cities
    getline(fin, line); //first, busan
    for(int i=0;i<line.length();i++){ //that line include city name
      if(line[i] == ' '||line[i] == '\t'||i==line.length()-1){
        if(temp != "")
          adj_s[j].push_back(temp);
        temp = "";
      }
      else{
        temp = temp+ line[i];
      }
    }
    assert(!adj_s[j].empty()); 
    adj_s[j].erase(adj_s[j].begin()); //delete city name
  }
  for(int j=0;j<size;j++){
    for(int i=0;i<adj_s[j].size();i++){
      if(adj_s[j][i]=="INF")
        adj_s[j][i] = "99999";
      adj[j].push_back(stoi(adj_s[j][i]));
    }
  }
  //test-> success
  /*
  for(int j=0;j<size;j++){
    for(int i=0;i<adj_s[j].size();i++){
      cout << adj[j][i] << " ";
    }
    cout << endl;
  }
  */
 //<indicate next node, distance>
  vector <pair<int,int>> matrix[size];
  for(int j=0;j<size;j++){
    for(int i=0;i<adj[j].size();i++)
      matrix[j].push_back(make_pair(i, adj[j][i])); 
  }
  //test
  /*
  for(int j=0;j<size;j++){
    for(int i=0;i<adj[j].size();i++)
      cout << matrix[j][i].first << " " <<matrix[j][i].second << " " ;
    cout << endl; 
  }
  */
  for(int i=0;i<size;i++)
    dijkstra(cities, adj, matrix, i);
  cout << "\n\n";
  Floyd(adj, size);
  fin.close();
  cities.clear();

  
  #else // This option will be work
  //open file
  ifstream fin ("hw6_data.txt");
  srand((unsigned int)time(NULL));
  clock_t dij_start, dij_end, flo_start, flo_end;
  double dij_sec, flo_sec;
  string line;
  getline(fin, line);
  string temp="";
  vector <string> cities;

  //store cities
  for(int i=0;i<line.length();i++){
    if(line[i] == ' '||line[i] == '\t'||i==line.length()-1){
      if(temp != "")
        cities.push_back(temp);
      temp = "";
    }
    else{
      temp = temp+ line[i];
    }
  }

  int size = cities.size();
  vector<string> adj_s[size];
  vector<int> adj[size];
  vector <pair<int,int>> matrix[size];
  temp="";
  
  //fill the matrix
  for(int j=0;j<size;j++){ 
    getline(fin, line); 
    for(int i=0;i<line.length();i++){ //that line include city name
      if(line[i] == ' '||line[i] == '\t'||i==line.length()-1){
        if(temp != "")
          adj_s[j].push_back(temp);
        temp = "";
      }
      else{
        temp = temp+ line[i];
      }
    }
    assert(!adj_s[j].empty()); 
    adj_s[j].erase(adj_s[j].begin()); //delete city name
  }

  //finish reading file
  fin.close();

  //initialize matrix
  for(int j=0;j<size;j++){
    for(int i=0;i<adj_s[j].size();i++){
      if(adj_s[j][i]=="INF")
        adj_s[j][i] = "99999";
      adj[j].push_back(stoi(adj_s[j][i]));
    }
  }

  //<indicate next node, distance>
  for(int j=0;j<size;j++){
    for(int i=0;i<adj[j].size();i++)
      matrix[j].push_back(make_pair(i, adj[j][i])); 
  }

  //display result
  vector<int> dij_list[size];
  vector<int> flo_list[size];
  vector<int> pred_list[size];

  //Call dijkstra Function
  dij_start = clock();
  for(int i=0;i<size;i++){
    dijkstra(cities, adj, dij_list, matrix, i);
  }
  dij_end = clock();
  //dij_sec = 1000.0* (dij_end- dij_start)/CLOCKS_PER_SEC;
  dij_sec =(dij_end- dij_start)/1000.0;
  //dij_sec = roundToThousandths(dij_ms);
  cout << "It took "<< dij_sec <<" seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n\n";
  DisplayMatix(dij_list, size, cities);
  
  //Call Floyd Function
  flo_start = clock();
  Floyd(adj, flo_list, pred_list, size);
  flo_end = clock();
  //flo_sec = 1000.0* (flo_end- flo_start)/CLOCKS_PER_SEC;
  flo_sec = (flo_end- flo_start)/1000.0;
  //flo_sec = roundToThousandths(flo_ms);
  cout << "It took "<< flo_sec <<" seconds to compute shortest path between cities with Floyd's algorithm as follows.\n\n";
  DisplayMatix(flo_list, size, cities);
  cout << "Predecessor list\n"<< endl;
  DisplayMatix(pred_list, size, cities);

  //Clear All
  cities.clear();
  for(int i=0;i<size;i++){
    adj[i].clear();
    adj_s[i].clear();
    dij_list[i].clear();
    flo_list[i].clear();
    pred_list[i].clear();
  }
  matrix->clear();
  #endif
}

double roundToThousandths(double x){
   return floor(x * 1000 + 0.5f) / 1000;
}

void dijkstra(vector<string> cities, vector<int> adj[], vector<int> list[], vector <pair<int,int>> matrix[], int src){
  int size = cities.size();
  int *dist = new int[size+1];
  bool *visit = new bool[size];
  priority_queue<pair<int,int>> pq;
  //Initialize
  for(int i=0;i<size;i++){
    dist[i] = INF;
  }
  dist[src] = 0;
  int node;
  int next, new_val, prev;
  pq.push(make_pair(0, src));
  while(!pq.empty()){
    node = pq.top().second;
    pq.pop();
    for(int i=0;i<matrix[node].size();i++){
      next = matrix[node][i].first;
      new_val = matrix[node][i].second + dist[node];
      prev = dist[next];
      if(new_val < prev){
        dist[next] = new_val;
        pq.push(make_pair(-1*new_val,next));
      }
    } 
  }
  
  for(int i =0 ;i<size;i++){
    //cout.setf(ios::left);
    //cout << setw(10) << dist[i] << " ";
    list[src].push_back(dist[i]);
  }
  //cout << endl;
  delete[] dist;
}

void Floyd(vector<int> adj[], vector<int> list[], vector<int> pred_list[], int size){
  vector <pair<int,int>> matrix[size];

  //Initialize
  for(int j=0;j<size;j++){
    for(int i=0;i<size;i++){
      matrix[j].push_back(make_pair(INF, NIL));
      if(j==i) matrix[j][i].first = 0;
    }
  }
  for(int j=0;j<size;j++){
    for(int i=0;i<size;i++){
      matrix[j][i].first = adj[j][i];
      if(matrix[j][i].first != INF)
        matrix[j][i].second = j;
    }
  }

  for(int k=0;k<size;k++){
    for(int i=0;i<size;i++){ //from i
      for(int j=0;j<size;j++){ // to j
        if(matrix[i][j].first > matrix[i][k].first + matrix[k][j].first){
          matrix[i][j].first = matrix[i][k].first + matrix[k][j].first;
          matrix[i][j].second = matrix[k][i].second;
        }
      }
    }
  }

  for(int j=0;j<size;j++){
    for(int i=0;i<size;i++){
      list[j].push_back(matrix[j][i].first);
    }
    //cout << endl;
  }
  for(int j=0;j<size;j++){
    for(int i=0;i<size;i++){
      //cout.setf(ios::left);
      //cout << setw(7) << matrix[j][i].second << " ";
      pred_list[j].push_back(matrix[j][i].second);
    }
    //cout << endl;
  }
}

void DisplayMatix(vector<int>list [], int size, vector<string> &cities){
  cout.setf(ios::left);
  cout << "\t ";
  for(int i=0;i<size;i++){
    cout.setf(ios::left);
    cout << setw(10) << cities[i] << " ";
  }
  cout << endl;

  for(int i=0;i<size;i++){
    cout.setf(ios::left);
    cout << setw(10) << cities[i] << " ";
    for(int j=0;j<size;j++){
      cout.setf(ios::left);
      cout << setw(10) << list[i][j] << " ";
    }
    cout << endl;
  }
  cout<<endl;
}