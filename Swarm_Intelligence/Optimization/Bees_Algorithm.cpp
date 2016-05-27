#include "Bees_Algorithm.h"

void Bees_Algorithm::initialise()
{
    loadData(sch10);
    initialiseBees();
};

void Bees_Algorithm::initialiseBees()
{
    //initialise population
    if(total_processes<100)
        total_bees = 2 * total_processes;
    else
        total_bees = 400;

    //initialise bee objects in the hive
    for(int bee_ID = 0; bee_ID<total_bees; bee_ID++)
    {
        hive.push_back(Bee(bee_ID));
    }
};

void Bees_Algorithm::search()
{
    cout<<"Searching..."<<endl;

    /** NOTE:: HOW MANY SCOUT BEES?*/
    ///1. Place scout bees randomly
    num_scoutBees = round(total_bees/10); //10% of initial bees are scouts

    random_device rd;
    default_random_engine generator(rd());
    std::uniform_int_distribution<int> uniform_dist(1, total_processes); //random number betwen 1 and total processes

    for(int i = 0; i<num_scoutBees ; i++)
    {
        hive[i].type = SCOUT;
        hive[i].location = uniform_dist(generator);

        scout_bees.push_back(&hive[i]);
    }

    ///2. Calculate fitness of sites visited by scouts
    //TODO : figure out multithreading for this( determine if this is necessary of even helpful )
    evaluateBeeFitness(scout_bees); //writes fitness to bee

    ///>WHILE(STOPPING CRITERION)
    //TODO : What is the stopping criterion? Quality of solution? number of iterations of search? completion of search space?
    bool stop_criterion = false;
    while(!stop_criterion)
    {
        ///3. Choose scout bees with highest fitness and choose their sites for a neighbourhood search
        //TODO : How many bees do you choose? Above what threshold of fitness/what number?
        //scout_bees.sort(scout_bees.begin(), scout_bees.end()); //sorts using default < operator(overloaded in Bee)
        sort(hive.begin(), hive.end()); //sorts using default < operator(overloaded in Bee)

        //get threshold somehow (this is a placeholder)
        auto threshold = hive.begin() + 9; //10 bees are chosen arbitrarily

        ///4. Recruit bees to send to selected sites in 3. (more bees for selected sites)
        //TODO: How to determine how many bees are recruited for each site?

        for(auto hive_iter = hive.begin(); hive_iter<threshold; hive_iter++)
        {
            //for each chosen scout, assign some number of remaining bees to its location +- rand(1, patch size)
        };

        ///5. Select the fittest bee from each patch searched
        //

        ///6. Assign remaining bees(new scouts) to search randomly and evaluate fitness (search for new areas)
        //

    }///END WHILE
};

void Bees_Algorithm::evaluateBeeFitness(vector<Bee*> bees)
{
    /*for(Bee* bee : bees)
    {
        bee->fitness = Processes[bee->location].calculateFitness();
    };*/
};

void Bees_Algorithm::evaluateSolutionFitness()
{


};
