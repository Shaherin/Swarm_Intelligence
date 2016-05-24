#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>
#include <chrono>
//#inculde <atomic> //atomic type variables synchronise reading and writing under the hood

#include "Swarm_Search.h"
#include "Bees_Algorithm.h"

using namespace std;

int main()
{
    //cout<<"Swarm Intelligence Test"<<endl;

    //start time
    auto start = chrono::high_resolution_clock::now();

    Swarm_Search s_search(SEARCH_TYPE::BEES); //default bees

    s_search.printStrategy();
    s_search.initialise();
    s_search.search();

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed_time = end-start;
    cout<<endl<<"Elapsed Time: " <<elapsed_time.count()<<" ms"<<endl;

    return 0;
}
