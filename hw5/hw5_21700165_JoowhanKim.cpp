/*
**References
(1)lecture slide ch22.
(2)Blog: https://www.geeksforgeeks.org/transpose-graph/
(3)Book: Thomas H. (2009). Introduction to Algorithm 3rd edition, pp 592, exercise 22.1-3

- It works only graph which is represented as matrix.
  For example, 

        A	B	C	D	E	F	G	H
    A   0	0	1	1	0	1	0	1
    B	0	0	1	1	1  	0	0	0  
    C  	0	0	0	0	1  	0	0	1
    D	1	0  	1	0	0	1	0	0
    E	0	0	1	0	0	0	1  	0
    F	1	0	1  	0	0 	0	0	1
    G	0	0	0	1  	1	0	0	0
    H	0	1	0	0	0	1	0	0

*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>    
using namespace std;

void Display_AdjList(char * node, vector<char> adj[], int size);
void Transpose_graph(char * node, vector<char> adj[], vector<char> t_adj[], int size);
void Initialize(char * node, vector<char> adj[], vector<char> t_adj[], int size);

int main(){
    /*
    ** My Logic ** 
    1. Read file and make a list using vector
        - read first line and check the size
        - node sort and adj[i].front() <- same with sorting node
    2. 만들어진 linked list를 transpose한다. 
    3. 결과 값을 출력한다. 

    ** function
    - Display_list
    - Transpose_adjList
    */

    //ifstream fin ("hw5_.data2.txt");
    ifstream fin ("hw5_data.txt");
    string line;
    string val;
    char link;
    //istringstream ss(line);
    int i =0;
    getline(fin, line);
    line.erase(remove(line.begin(), line.end(), '\t'), line.end());
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    //cout << line.size() <<endl;
    int size = line.size()-1;
    char * node = new char[size];
    for(int i=0;i<size;i++){
        node[i] = 'A' + i;
        //cout << node[i] << endl;
        //cout << line[i] <<endl;
    }
    vector<char> adj[size];
    vector<char> t_adj[size];
    int vertex_id = 0; //indicate which node

    while(std::getline(fin, line)){
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        //input sorting algorithm here.
        for(int j=1;j<line.size();j++){ // except node, input the linked vertexs
            link = (char)line[j]; // ex) B 1 0 1 0 0 1
            if(link=='1')
                adj[vertex_id].push_back('A'+j-1);
        }
        vertex_id++; 
    }
    cout << "Adjacency list of graph" << endl;
    Display_AdjList(node, adj, size);
    cout << "Adjacency list of transpose graph" << endl;
    Transpose_graph(node, adj, t_adj, size);
    Display_AdjList(node, t_adj, size);
    fin.close();
    Initialize(node, adj,t_adj, size);
}

void Initialize(char * node, vector<char> adj[], vector<char> t_adj[], int size){
    delete[] node;
    for(int i=0;i<size;i++){
        adj[i].clear();
        t_adj[i].clear();
    }
}

void Display_AdjList(char * node, vector<char> adj[], int size){
    for(int i=0;i<size;i++){
        cout << "\t" <<node[i] << "-->";
        for(int j=0;j<adj[i].size();j++){
            cout << adj[i][j] ;
            if(j!=adj[i].size()-1)
                cout<< "->";
        }
        cout <<endl;
    }
    cout << "\n"<<endl;
}
void Transpose_graph(char * node, vector<char> adj[], vector<char> t_adj[], int size){
    int curr=0;
    for(int i=0;i<size;i++){
        for(int j=0; j<adj[i].size();j++){
            curr = adj[i][j]-'A';
            t_adj[curr].push_back(node[i]);
        }
    }

}