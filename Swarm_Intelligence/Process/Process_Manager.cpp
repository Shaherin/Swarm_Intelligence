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
};

void Process_Manager::clearProcesses()
{
    m_processes.clear();
    m_num_processes = 0;
    m_available_IDs.clear();
};

//d = round [SUM_P * h]
double Process_Manager::calcCommonDueDate(vector<Process*> processes)
{
    double common_due_date = 0;

    //for each process,
    for(auto process_iter : processes)
        common_due_date += process_iter->calculateFitness();

    common_due_date = common_due_date * m_restrictive_factor;

    return common_due_date;

    /*time_duration common_due_date = time_duration(0);
    for(auto map_iter : m_processes)
        common_due_date += map_iter.second.getProcessingTime();

    m_common_due_date = common_due_date.count() * m_restrictive_factor;*/
};

double Process_Manager::calculateFitness(vector<Process*> processes)
{
    double fitness = 0;

    //for each process, calculate and sum fitness values
    for(auto process_iter : processes)
        fitness += process_iter->calculateFitness();

    return fitness;
};

