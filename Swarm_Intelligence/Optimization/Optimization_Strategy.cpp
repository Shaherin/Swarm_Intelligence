#include "Optimization_Strategy.h"

void Optimization_Strategy::loadData(const string filename)
{
    string line;
    ifstream file(filename);

    //regex for identifying specific lines of the data
    regex regex_num_data_sets("(^\\s{2})(\\d+)"); //(2)
    regex regex_num_processes("(^\\s{5,})(\\d+)"); //(2)
    regex regex_process_triple("(^\\s{5,})(\\d+)(\\s+)(\\d+)(\\s+)(\\d+)"); //processing time(2), earliness(4), tardiness(6)

    //line matcher, sub matcher, string conversion of sub matcher
    smatch regex_matcher;
    ssub_match group_matcher;
    string str_group_matcher;

    if(file.is_open())
    {
        /** Preliminary (variable collection) */
        //get number of data sets
        getline(file, line);
        if(regex_match(line, regex_matcher, regex_num_data_sets))
        {
            group_matcher = regex_matcher[2];
            istringstream(group_matcher.str()) >> num_data_sets;
        }

        //get number of processes per data set & calculate number of processes
        getline(file, line);
        if(regex_match(line, regex_matcher, regex_num_processes))
        {
            group_matcher = regex_matcher[2];
            istringstream(group_matcher.str()) >> num_processes;

            //total_processes = num_data_sets * num_processes;
        }

        /** Main reading loop */
        //read in data from each data set
        int current_set = 0;
        Process_Sets.push_back(Process_Manager());

        while(getline(file, line))
        {
            //if number of processes per set is encountered, begin reading a new set
            if(regex_match(line, regex_matcher, regex_num_processes))
            {
                //set start and completion times for all processes
                Process_Sets[current_set].initialiseProcesses();
                Process_Sets[current_set].calcCommonDueDate();

                //move to the next set
                current_set++;
                Process_Sets.push_back(Process_Manager());
            }

            //get process triple and create process
            if(regex_match(line, regex_matcher, regex_process_triple))
            {
                double processing_time;
                double earliness_penalty;
                double tardiness_penalty;

                istringstream(regex_matcher[2].str()) >> processing_time;
                istringstream(regex_matcher[4].str()) >> earliness_penalty;
                istringstream(regex_matcher[6].str()) >> tardiness_penalty;

                //create process and add to set
                Process_Sets[current_set].addProcess( move(Process(double(processing_time), earliness_penalty, tardiness_penalty)) );

                //cout<< processing_time<<endl;
            }
        }

        //set start and completion times for all processes in the final set
        Process_Sets[current_set].initialiseProcesses();
        Process_Sets[current_set].calcCommonDueDate();
    }

    /*cout<<"Test"<<endl;

    Process_Manager manager = Process_Sets[0];
    cout<<"due date: "<<manager.getDueDate()<<endl;
    cout<<"f(S): "<<manager.calculateFitness()<<endl;

    cout<<"\nFitness: "<<endl;
    cout<<manager[0].getDueDate();//*/

    //for(Process_Manager manager: Process_Sets)
        //cout<<manager.calculateFitness()<<endl;


    //cout<<Process_Sets[0].getProcesses()[0].getStartTime()()<<endl;
};

void Optimization_Strategy::generateSolutionSpace(int process_set)
{
    //Solution_Space.clear();

    Process_Manager temp_processes = Process_Sets[process_set];
    Solution_Space.push_back(temp_processes);

    for(int i = 1; i<1000 ;i++)
    {
        temp_processes.shuffleProcesses();
        temp_processes.initialiseProcesses();
        temp_processes.calcCommonDueDate();

        Solution_Space.push_back(temp_processes);
        //cout<<"fds "<<Solution_Space[i].calculateFitness()<<endl;
    }
    //cout<<"fe " <<Process_Sets[0].calculateFitness()<<endl;
    //cout<<"fds "<<Solution_Space[85].calculateFitness()<<endl;
    //cout<<temp_processes.getProcesses().size()<<endl;
};
