/*
    **References**
    (1)lecture slide ch22.

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

using namespace std;
#define INF 99999
#define NIL -1

void FillAdjList(vector<int> adj[], int size); //It will be developed soon
void ReadAndStoreInVector(vector<string> &arr); //It will be developed soon
void dijkstra(vector<string> cities, vector<int> adj[], vector <pair<int,int>> matrix[],int src);
void Initialize(vector<int> adj[]); //It will be developed soon
void Floyd(vector<int> adj[], int size);

int main(){
  #if 1
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
  #else
  #endif
}

void dijkstra(vector<string> cities, vector<int> adj[], vector <pair<int,int>> matrix[], int src){
  int size = cities.size();
  int *dist = new int[size+1];
  bool *visit = new bool[size];
  priority_queue<pair<int,int>> pq;
  //Initialize
  for(int i=0;i<size;i++){
    dist[i] = INF;
    visit[i] = false;
  }
  dist[src] = 0;
  visit[src] = true;
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
    cout << dist[i] << " ";
  }
  cout << endl;
  delete[] dist;
  delete[] visit;
}

void Floyd(vector<int> adj[], int size){
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
      cout << matrix[j][i].first << " ";
    }
    cout << endl;
  }


}