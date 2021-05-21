#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;

int main(){
    ifstream fin ("hw5_data.txt");
    /*char **A;
    for (int i = 0; i < 30; i++)
        for (int j = 0; j < 30; j++)
            fin >> A[i][j];*/
    string line;
    while(getline(fin, line)){
        cout <<line;
        //line.erase(remove(line.begin(), line.end(), '\t'), line.end());
        //cout <<line;
    }

    
}