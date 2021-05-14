/*
* References
- file write : https://jhnyang.tistory.com/363
- DP : ch 6 knapsack ppt 슬라이드
<blog>
- Greedy: https://stackoverflow.com/questions/42536798/greedy-algorithm-implemented-in-fractional-knapsack
        https://dojinkimm.github.io/algorithm/2019/10/19/dp-2.html
- B&B: http://yimoyimo.tk/Dynamic,Branch-and-bound-Knapsack/
- Vector: https://cocoon1787.tistory.com/244
        https://blockdmask.tistory.com/70
- time : http://cplusplus.com/forum/beginner/195167/

* My program works for greedy and DP solution only.”

*/

#include <iostream>
#include <ctime>
#include <random>
#include <cstdlib>
#include <cassert>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
/*
int DynamicProgramming(int *val, int *w, int n, int capa);
void Greedy(int *v, int *w, int n, int cap);
void Branch_Bound(int *v, int *w, int n, int cap);
*/

double DynamicProgramming(vector <int>& v, vector <int>& w, int n, int cap);
double Greedy(vector <int>& v, vector <int>& w, int n, int cap);
double Branch_Bound(vector <int>& v, vector <int>& w, int n, int cap);
void sorting(vector <int>& v, vector <int>& w, struct Element *);
float Bound(int cap, int n, struct TreeNode *curr, struct Element *);
double roundToThousandths(double x);

int main(int argc, char *argv[])
{
    /*
    //max capacity: items * 40
    //value:1~300/ weight:1~ 100
    //# of items : 100, 1000, 2000, 4000, 6000, 8000, 10000, 12000
    - set input items and W
    - make value and weight
     */

    #if 0 // test version 4 value and 5 weight 
    ofstream fout("output.txt");
    srand((unsigned int)time(NULL));
    int items = 4; 
    int W = 5;
    int res;
    vector<int> value;
    vector<int> weight;

    // fill the values and weights
    for (int i = 0; i < items; i++)
        value.push_back(rand() % 300 + 1);

    for (int i = 0; i < items; i++)
        //weight[i] = rand() % 100 + 1;
        weight.push_back(rand() % 5 + 1);//use when test
    
    //test
    for(int i=0;i<value.size();i++)
        cout << i<<": " << value[i] << "\t";
    for(int i=0;i<weight.size();i++)
        cout << i<<": " << weight[i] << "\t";
    // Dynamic programming
    //cout << "By using dynamic programming algorithm" << endl;
    //res = DynamicProgramming(value, weight, items, W);
    //greedy
    //cout << "By using Greedy algorithm" << endl;
    //res = Greedy(value , weight, items, W);
    //branch and bound
    res = Branch_Bound(value, weight, items, W);
    
    if(res){
        value.clear();
        weight.clear();
    }

    #else // for input 100, 1000, ...
    srand((unsigned int)time(NULL));
    //int items = 100; //100, 1000, 2000, 4000, 6000, 8000, 10000, 12000
    int items[8] = {100, 1000, 2000, 4000, 6000, 8000, 10000, 12000}; 
    int num = sizeof(items)/sizeof(items[0]);
    //int *W = new int[num];
    clock_t dp_start, dp_end, g_start, g_end, bb_start, bb_end;
    double dp_ms, g_ms, bb_ms;
    int res1, res2, res3;
    int W;
    for(int i=0;i<num;i++){
        W = items[i] * 40;
         vector<int> value;
         vector<int> weight;
        // fill the values and weights
        for (int j = 0; j < items[i]; j++)
            //value[i] = rand() % 300 + 1;
            value.push_back(rand() % 300 + 1);

        for (int k = 0; k < items[i]; k++)
            weight.push_back(rand() % 100 + 1);
            //weight[i] = rand() % 5 + 1; //use when test
        ofstream fout("output.txt", ios::app);
        fout << "Input\t\t\t\tDP(value/time)\t\t\t\tGreedy(value/time)\t\t\t\tB.B(value/time)" <<endl;
        fout << items[i] << "\t\t\t\t";
        // Dynamic programming
        dp_start = clock();
        cout << "By using dynamic programming algorithm" << endl;
        res1 = DynamicProgramming(value, weight, items[i], W);
        dp_end = clock();
        dp_ms = 1000.0* (dp_end - dp_start)/CLOCKS_PER_SEC;
        dp_ms = roundToThousandths(dp_ms);
        //greedy
        g_start = clock();
        cout << "By using Greedy algorithm" << endl;
        res2 = Greedy(value , weight, items[i], W);
        g_end = clock();
        g_ms = 1000.0* (g_end - g_start)/CLOCKS_PER_SEC;
        g_ms = roundToThousandths(g_ms);
        //branch and bound
        
        bb_start = clock();
        cout << "By using BB algorithm" << endl;
        res3 = Branch_Bound(value, weight, items[i], W);
        bb_end = clock();
        bb_ms = 1000.0* (bb_end- bb_start)/CLOCKS_PER_SEC;
        bb_ms = roundToThousandths(bb_ms);
        
        //Only for DP and Greedy
        //fout << res1 <<"/"<<dp_ms <<"ms\t\t\t\t" << res2 <<"/"<<g_ms <<"ms\t\t\t\t" << endl;
        //Write DP, Greedy, and BB
        fout << res1 <<"/"<<dp_ms <<"ms\t\t\t\t" << res2 <<"/"<<g_ms <<"ms\t\t\t\t" << res3 <<"/"<<bb_ms <<"ms\t\t\t\t" << endl;
        value.clear();
        weight.clear(); 
        fout.close();
        
    }
   
    
    #endif
}
double roundToThousandths(double x)
{
   return floor(x * 1000 + 0.5f) / 1000;
}

double DynamicProgramming(vector <int>& v, vector <int>& w, int n, int cap){
    #if 0 // using 2 dimensional array(It doesn't work)
    int i;
    clock_t start, end;
    ofstream fout("output.txt", ios::app);
    start = clock();
    int item = n+1;
    int maxW = cap +1;
    int** benefit = new int*[item];
    for(i=0;i<maxW;i++)
        benefit[i] = new int[maxW]; // 
    
    //fill in 0
    for(i=0;i<item;i++) 
        benefit[i][0] = 0; //[0,0], [1,0], ...
    for(i=0;i<maxW;i++)
        benefit[0][i] = 0;
    //고려되고 있는 item => n개의 item들이 있다. 첫번째 item: weight[0], value[0]...weight[i], value[i]
    for(i=1;i<item;i++){ //1 2 3 4
        for(int j=1;j<maxW;j++){ // 1 2 3 4 5
            //item i can be part of the solution
            if(w[i]<=j){ // 현재 item의 weight이 고려되고 있는 weight
                if(benefit[i-1][j-w[i]] +v[i] > benefit[i-1][j])
                    benefit[i][j] = benefit[i-1][j-w[i]] +v[i];
                else 
                    benefit[i][j] = benefit[i-1][j];
            }
            else //benefit[i][j]> cap
                benefit[i][j] = benefit[i-1][j];
        }
    }
    end = (double)clock() - start; /// (double)(CLOCKS_PER_SEC / 1000);
    //fout << "By using dynamic programming algorithm" << endl;
    fout << "Input "<< n <<"'s result : " << benefit[n][cap] <<"\t\tTime: " << end  <<"ms"<<endl;
    //test when input 4, 5
    /*
    fout << cap << " "<< n <<endl; 
    for(int z=0;z<cap+1;z++){ //0 1 2 3 4 5
        for(int k=0;k<n+1;k++) //0 1 2 3 4 
            fout << benefit[k][z] << " ";
        fout << endl;
    }
    */
    fout.close();
    for(i=0;i<maxW;i++)
        delete[] benefit[i];
    delete[] benefit;

    benefit = 0;

    return 1;
    #else
    int i;
    double result;
    //clock_t start, end;
    //ofstream fout("output.txt", ios::app);
    //start = clock();
    int item = n+1;
    int maxW = cap +1;
    vector<vector<int> >benefit(item, vector<int>(maxW,0));
    //fill in 0
    for(i=0;i<item;i++) 
        benefit[i][0] = 0; //[0,0], [1,0], ...
    for(i=0;i<maxW;i++)
        benefit[0][i] = 0;
    //고려되고 있는 item => n개의 item들이 있다. 첫번째 item: weight[0], value[0]...weight[i], value[i]
    for(i=1;i<item;i++){ //1 2 3 4
        for(int j=1;j<maxW;j++){ // 1 2 3 4 5
            //item i can be part of the solution
            if(w[i]<=j){ // 현재 item의 weight이 고려되고 있는 weight
                if(benefit[i-1][j-w[i]] +v[i] > benefit[i-1][j])
                    benefit[i][j] = benefit[i-1][j-w[i]] +v[i];
                else 
                    benefit[i][j] = benefit[i-1][j];
            }
            else //benefit[i][j]> cap
                benefit[i][j] = benefit[i-1][j];
        }
    }
    //end = (double)clock() - start; /// (double)(CLOCKS_PER_SEC / 1000);
    //fout << "DP: Input "<< n <<"'s result : " << benefit[n][cap] <<"\t\t\tTime: " << end  <<"ms"<<endl;
    result = benefit[n][cap];
    
    //test when input 4, 5
    /*
    fout << cap << " "<< n <<endl; 
    for(int z=0;z<cap+1;z++){ //0 1 2 3 4 5
        for(int k=0;k<n+1;k++) //0 1 2 3 4 
            fout << benefit[k][z] << " ";
        fout << endl;
    }
    */
    //fout.close();
    for(i=0;i<(int)benefit.size();i++)
       benefit[i].clear();
    benefit.clear();

    return result;
    #endif

}

double Greedy(vector <int>& v, vector <int>& w, int n, int cap){
    //clock_t start, end;
    //start = clock();
    //ofstream fout("output.txt", ios::app);
    /*
    1. save 'value/weight'(rate) and vector and calculate all rates
    2. sorting value, weight, and rate by decending order 
    3. Weight less than curr_cap, contain the item and adjust the weight.
    4. else, split the price and put it in and out of the loop.
    */ 
    //int num = n;
    int num = v.size();
    int curr_cap = cap;
    double total = 0;
    double fraction;
    vector <double> rate(num);

    for(int i=0;i<num;i++)
        rate[i] = (double)v[i]/w[i];
    
    //sort(rate.begin(), rate.end(), greater<float>());
    //sorting by decending order
    for(int i=0;i<num-1;i++){
        for(int j=i+1;j<num;j++){
            float ftemp;
            int temp;
            if(rate[i]<rate[j]){
                //change rate
                temp = rate[i];
                rate[i] = rate[j];
                rate[j] = temp;
                //change value
                temp = v[i];
                v[i] = v[j];
                v[j] = temp;
                //change weight
                temp = w[i];
                w[i] = w[j];
                w[j] = temp;
            }
        } 
    }
    for(int i=0;i<num;i++){
            //Weight less than curr_cap, contain the item and adjust the weight.
            if(w[i] <= curr_cap){
                curr_cap = curr_cap - w[i];
                total += v[i];
                if(curr_cap == 0) break;
            }
            else{
                //fraction = (double)(cap/w[i]);
                //total +=fraction * v[i];
                total += rate[i] * curr_cap;
                break;
            }
    }

    //end = (double)clock() - start;
    //fout << "Greedy: Input "<< n <<"'s result : " << total <<"\t\tTime: " << end  <<"ms"<<endl;

    //fout.close();
    rate.clear();
    return total;
}
/*
In the node, there are information about profit, weight, level, and bound
*/
struct TreeNode{
    int profit;
    int weight;
    int level;
    float bound;
};
struct Element{
    //vector<int> val;
    //vector<int> wei;
    //vector<double> ratio;
    int val;
    int wei;
    double ratio;
};
struct cmp{
    bool operator()(TreeNode x, TreeNode y){
        return y.bound >x.bound;
    }
};

int max_benefit;
double Branch_Bound(vector <int>& v, vector <int>& w, int n, int cap){
    //clock_t start, end;
    //start = clock();
    //ofstream fout("output.txt", ios::app);
    int num = v.size();
    double result;
    struct TreeNode curr, next, another;
    struct Element item[num];
    priority_queue <TreeNode, vector<TreeNode> , cmp > que;
    //sorting the rate of items in decending order. (do it again)
    sorting(v,w,item);
    //set the root node
    curr.weight = 0;
    curr.profit = 0;
    curr.level =0;
    curr.bound = Bound(cap, n, &curr, item);
    //store in queue
    max_benefit = 0;
    que.push(curr);
    //Branch and Bound
    /*
    1. dequeue, check next node(choose) which is promising
    2. next node check it is promising -> push
    3. set next node for another node(not choose) -> push
    */
    while(!que.empty()){
        curr = que.top(); //parent node
        que.pop(); 
        //check promising
        if(curr.bound > max_benefit){
            // store value in next node
            next.level = curr.level +1;
            next.profit = curr.profit + item[next.level-1].val;
            next.weight = curr.weight + item[next.level -1].wei;

            if(next.weight <= cap)
                max_benefit = max(next.profit, max_benefit);
            
            next.bound = Bound(cap, num, &next, item);
            
            //go for another second node (non choose)
            another.profit = curr.profit;
            another.weight = curr.weight;
            another.bound = Bound(cap, num, &another, item);

            //check promising
            /*
            if(next.bound > max_benefit && next.weight <= cap && another.bound > max_benefit && another.weight <= cap){
                if(next.bound > another.bound) que.push(next);
                else que.push(another);
            }
            else if(next.bound > max_benefit && next.weight <= cap) //promising? add
                que.push(next);
            
            else if(another.bound > max_benefit && another.weight <= cap){
                que.push(another);

            }
            */
            if(next.bound > max_benefit && next.weight <= cap) //promising? add
                que.push(next);
            
            if(another.bound > max_benefit && another.weight <= cap){
                que.push(another);

            }   
        } 
        
    }
    result = (double)max_benefit;
    //end = (double)(clock() - start);
    //fout.close();
    memset(&item, 0, sizeof(struct Element));
    memset(&curr, 0, sizeof(struct TreeNode));
    memset(&next, 0, sizeof(struct TreeNode));
    memset(&another, 0, sizeof(struct TreeNode));
    return max_benefit;
}
void sorting(vector <int>& v, vector <int>& w, struct Element *item){ 
    int num = v.size();
    vector <double> rate(num);
    for(int i=0;i<num;i++)
        rate[i] = (double)v[i]/w[i];
    
    for(int i=0;i<num-1;i++){
        for(int j=i+1;j<num;j++){
            float ftemp;
            int temp;
            if(rate[i]<rate[j]){
                //change rate
                temp = rate[i];
                rate[i] = rate[j];
                rate[j] = temp;
                //change value
                temp = v[i];
                v[i] = v[j];
                v[j] = temp;
                //change weight
                temp = w[i];
                w[i] = w[j];
                w[j] = temp;
            }
        } 
    }
    for(int i=0;i<num;i++){
        item[i].val = v[i];
        item[i].wei = w[i];
        item[i].ratio = rate[i];
    }

    
    rate.clear();
    
}
float Bound(int cap, int n, struct TreeNode *curr, struct Element *item){
    
    //bound = current benefits + future benefits  + fraction
    int benefit = curr->profit;
    int tot_weight = curr->weight;
    int i =0;
    float bound = 0;
    //앞으로 집어넣을 benefit과 weight
    //future benefits and weight
    for(i=curr->level+1;i<=n;i++){
        if(tot_weight+ item[i-1].wei > cap) break;
        benefit = benefit+ item[i-1].val; //store previous value
        tot_weight = tot_weight + item[i-1].wei; // store previous weight
    }
    //add fraction
    if(i<=n)
        bound = benefit + (cap - tot_weight)*(item[i-1].val/item[i-1].wei);
    return bound;
}

