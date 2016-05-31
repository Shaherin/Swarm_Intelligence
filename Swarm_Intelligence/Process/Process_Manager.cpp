#include "Process_Manager.h"

void Process_Manager::addProcess(Process process)
{
    //if there are IDs available, assign them first
    if(m_available_IDs.size() != 0)
    {
        process.setID(m_available_IDs[0]);
        m_processes.insert( m_processes.begin() + m_available_IDs[0], process);
    }
    //else increase m_num_processes and introduce a new ID
    else
    {
        m_num_processes++;
        process.setID(m_num_processes);
        m_processes.push_back(process);
    }
};

void Process_Manager::addProcesses(vector<Process> processes)
{
    for(Process proc : processes)
    {
        addProcess(proc);
    }
};

void Process_Manager::removeProcess(int process_ID)
{
    m_processes.erase(m_processes.begin() + process_ID);
    m_available_IDs.push_back(process_ID);
};

void Process_Manager::shuffleProcesses()
{
    //shuffle processes
    random_shuffle(m_processes.begin(), m_processes.end());

    //reset IDs
    for(unsigned int i = 0; i<m_processes.size(); i+=10)
    {
        m_processes[i].setID(i);
        m_processes[i+1].setID(i+1);
        m_processes[i+2].setID(i+2);
        m_processes[i+3].setID(i+3);
        m_processes[i+4].setID(i+4);
        m_processes[i+5].setID(i+5);
        m_processes[i+6].setID(i+6);
        m_processes[i+7].setID(i+7);
        m_processes[i+8].setID(i+8);
        m_processes[i+9].setID(i+9);
    }

    //reset start time and completion time
    //initialiseProcesses();
};

void Process_Manager::clearProcesses()
{
    m_processes.clear();
    m_num_processes = 0;
    m_available_IDs.clear();
};

//initialises start and completion time of processes
void Process_Manager::initialiseProcesses()
{
    m_processes[0].setStartTime(0);
    m_processes[0].setCompletionTime(m_processes[0].getProcessingTime());

    for(int i = 1; i<m_processes.size(); i++)
    {
        m_processes[i].setStartTime(m_processes[i-1].getCompletionTime());
        m_processes[i].setCompletionTime(m_processes[i].getStartTime()+m_processes[i].getProcessingTime());
    }

    //calcCommonDueDate();
};

//d = round [SUM_P * h]
void Process_Manager::calcCommonDueDate(double restrictive_factor)
{
    m_restrictive_factor = restrictive_factor;
    m_common_due_date = 0.0;

    //for each process, sum the processing times
    for(Process process: m_processes)
        m_common_due_date += process.getProcessingTime();

    m_common_due_date = m_common_due_date * m_restrictive_factor;

    ///NOTE: This for loop does not work for some reason
    /*for(Process process: m_processes)
    {
        process.setDueDate(m_common_due_date);
    }*/

    //set the due date in each process
    for(int i = 0; i<m_processes.size(); i++)
    {
        m_processes[i].setDueDate(m_common_due_date);
    }

};

double Process_Manager::calculateFitness()
{
    double fitness = 0;

    //for each process, calculate and sum fitness values
    //for(Process process: m_processes)
        //fitness += process.calculateFitness();

    for(int i = 0; i<m_processes.size(); i++)
        fitness+=m_processes[i].calculateFitness();

    m_total_fitness = fitness;
    return fitness;
};

