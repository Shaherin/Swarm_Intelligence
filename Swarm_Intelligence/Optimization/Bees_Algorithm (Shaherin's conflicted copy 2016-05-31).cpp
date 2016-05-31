#include "Bees_Algorithm.h"

void Bees_Algorithm::initialise()
{
    loadData(sch10);

    initialiseBees();

    generateSolutionSpace(0); //generate solution space for set k(argument)

};

void Bees_Algorithm::initialiseBees()
{
    ///initialise population (we now do this statically)
    /*if(num_processes<100)
        total_bees = 2 * num_processes;
    else
        total_bees = 400;*/

    ///initialise bee objects in the hive
    for(int bee_ID = 0; bee_ID<total_bees; bee_ID++)
    {
        hive.push_back(Bee(bee_ID));
    }

    //cout<<"init" <<Solution_Space[0].calculateFitness()<<endl;
    //cout<<"fe " <<Process_Sets[0].calculateFitness()<<endl;

};

void Bees_Algorithm::search()
{
    cout<<"Searching..."<<endl;
    ///Note: gcc on windows does not support non-deterministic random_device so an alternative had to be found
    //random_device rd;
    //default_random_engine generator(rd());
    //std::uniform_int_distribution<int> uniform_dist(1, total_processes); //random number betwen 1 and total processes
    //default_random_engine generator{randutils::auto_seed_128{}.base()}; //this also worked

    ///1. Place scout bees randomly and with uniform distribution(all bees initially scouts)
    //num_scoutBees = total_bees;
    //num_scoutBees = round(total_bees/10); //10% of initial bees are scouts

    //use default rng generator
    randutils::default_rng generator;
    generator.seed(); //non-deterministic

    //generate random locations for scout bees(total bees for first iteration)
    for(int i = 0; i<total_bees ; i++)
    {
        //hive[i].type = SCOUT;
        hive[i].location = generator.uniform(0, static_cast<int>(Solution_Space.size()) );
        //hive[i].location = uniform_dist(generator);

        scout_bees.push_back(&hive[i]);
    }

    ///2. Calculate fitness of sites visited by scouts
    //TODO : figure out multithreading for this( determine if this is necessary of even helpful )
    evaluateBeeFitness(scout_bees); //writes fitness to bee

    ///>WHILE(STOPPING CRITERION)
    //TODO : What is the stopping criterion? Quality of solution? number of iterations of search? completion of search space?
    bool stop_criterion = false;
    //while(!stop_criterion)
    for(int iteration = 0; iteration < 3; iteration++)
    {
        ///3. Choose scout bees with highest fitness and choose their sites for a neighbourhood search
        //sort main hive to make it easier to seperate scouts and other bees, rather than having to check each bee type
        sort(hive.begin(), hive.end()); //sorts using default < operator(overloaded in Bee to compare fitness)

        ///4. Recruit bees to send to selected sites in 3. (more bees for selected sites) & assign location to bees
        //vector<int> patch_locations; //sorted in order or best to worst patch

        //i) populate patch_locations vector with locations of the chosen scout bees
        for(int bee_index = 0 ; bee_index<num_selectedSites; bee_index++) //200 selected sites
        {
            //if( find(patch_locations.begin(), patch_locations.end(), hive[bee_index].location) != patch_locations.end())
            //prevent duplication of locations
            if(patch_locations.find(hive[bee_index].location) != patch_locations.end())
            {
                continue;
                bee_index--;
            }
            else
            {
                //patch_locations[bee_index] = (hive[bee_index].location);
                //patch_locations.push_back(hive[bee_index].location);

                //initialise vector of bees around a patch
                patch_locations[hive[bee_index].location] = vector<Bee*>();

                //push first scout bee
                patch_locations[hive[bee_index].location].push_back(&hive[bee_index]);
           }
        }

        //ii) assign locations from list of patch locations to recruited bees probabilistically
        for(int current_bee = 0; current_bee < num_recruitedBees; current_bee++)
        {
            //normal distribution produces outliers which this loop looks to exclude
            int location_index; //this is an index to patch_locations
            do
            {
                //abs(normal dist) is a half/folded normal distribution about 0.0, so bees will be allocated to
                //patch locations with higher probability of being assigned to the patch of highest fitness
                location_index = abs(generator.variate<double, std::normal_distribution>(0.0,patch_locations.size()));
            }while( location_index < patch_locations.size());

            //get the patch location responding to the random location_index
            map<int, vector<Bee*>>::iterator patch_locations_iter = patch_locations.begin();
            for(int i = 0; i<location_index; i++)
            {
                patch_locations_iter++;
            }

            int patch_location = patch_locations_iter->first; //set patch location for later

            //alter central patch location to a location within the patch hyperspace with uniform prob
            int location = patch_location;
            location = generator.uniform(location - size_patches, location + size_patches) ;

            //deal with negative locations
            if(location < 0)
                location = Solution_Space.size()+location;

            //deal with locations out of bounds
            if(location > Solution_Space.size())
                location = location % Solution_Space.size();

            //push current_bee into patch vector
            //num_selectedSites is number of selected scout bees, the rest are remaining
            Bee* bee = &hive[num_selectedSites+current_bee]; //get bee to be recruited
            bee->location = location; //set location within patch
            patch_locations[patch_location].push_back( (bee) ); //add bee to patch with its location set

        }

        //vector<int> recruited_bee_locations(num_recruitedBees); //50 recruited bees
        //vector<int> remaining_bee_locations(num_remainingBees); //30 remaining
        //vector<int> new_scout_locations(num_scoutBees); //150 new scouts

        ///5. Select the fittest bee from each patch searched
        //replace scout_bees vector with the fittest bee of each patch
        scout_bees.clear();

        for(auto patch : patch_locations)
        {
            ///evaluateBeeFitness(patch.second);
            sort(patch.second.begin(), patch.second.end());//sort patch vector of bees based on fitness

            scout_bees.push_back(patch.second[0]);//add the first bee(bee of highest fitness)
        };

        ///6. Assign remaining bees to search randomly and evaluate fitness (search for new areas)
        num_remainingBees = total_bees - (num_selectedSites + num_recruitedBees);
        for(int bee_index = num_selectedSites + num_recruitedBees; bee_index<total_bees; bee_index++)
        {
            hive[bee_index].location = generator.uniform(0, static_cast<int>(Solution_Space.size()) );
            remaining_bees.push_back(&hive[bee_index]);
        }

        ///evaluateBeeFitness(remaining_bees);
        sort(remaining_bees.begin(), remaining_bees.end());

        //for(auto i : patch_locations)
            //cout<<i.first<<endl;

        //for(Bee* i : scout_bees)
            //cout<<i->fitness<<endl;
        for(Bee* i : remaining_bees)
            cout<<i->fitness<<endl;

    }///END WHILE
};

void Bees_Algorithm::evaluateBeeFitness(vector<Bee*> bees)
{
    /*for(Bee* bee : bees)
    {
        bee->fitness = Processes[bee->location].calculateFitness();
    };*/
    for(Bee* bee : bees)
    {
        bee->fitness = Solution_Space[bee->location].calculateFitness();
    }

};

void Bees_Algorithm::evaluateSolutionFitness()
{


};
