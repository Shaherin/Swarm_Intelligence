#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>
#include <chrono>
#include <stdlib.h>
#include "Random/randutils.hpp"

//#inculde <atomic> //atomic type variables synchronise reading and writing under the hood

#include "Optimization/Swarm_Search.h"
#include "Optimization/Bees_Algorithm.h"
#include "Process/Process.h"

using namespace std;

int main()
{
    //cout<<"Swarm Intelligence Test"<<endl;

    //start time
    auto start = chrono::high_resolution_clock::now();

    Swarm_Search s_search(SEARCH_TYPE::BEES); //default bees

    s_search.printStrategy();
    s_search.initialise();
    static_cast<Bees_Algorithm*>(s_search.getStrategy())->setNumIterations(2);

    for(int k = 0; k<10; k++)
    {
        s_search.search();
    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> elapsed_time = end-start;
    cout<<endl<<"Elapsed Time: " <<elapsed_time.count()<<" ms"<<endl;

    //use default rng generator
    /*randutils::default_rng generator;
    generator.seed(); //non-deterministic

    int y = generator.uniform(0,10);
    int x = generator.variate<double, std::normal_distribution>(0.0,5);

    vector<double> vec(10);
    generator.generate<std::normal_distribution>(vec, 5.0, 1.0);*/

    //cout<<x<<endl;
    //cout<<y<<endl;

    /*for(int i : vec)
        cout<<i<<endl;//*/

    return 0;
}
