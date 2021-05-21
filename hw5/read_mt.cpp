#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;

// take from http://stackoverflow.com/a/236803/248823
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

int main() {
    std::ifstream infile ("hw5_data.txt");

    std::string line;



    while (std::getline(infile, line))
    {
        vector<string> row_values;

        split(line, '\t', row_values);

        for (auto v: row_values)
            cout << v << ' ';
        //cout << row_values.size();

        cout << endl;
     }

    cout << "hello " << endl;
    return 0;
}
