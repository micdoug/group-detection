#include "Simulator.h"
#include "Connection.h"
#include "extensions.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <vector>
#include <memory>
#include <future>
#include <cmath>
#include <functional>

using namespace std;

Simulator::Simulator(int nrof_nodes, long timeStep, long duration, 
    std::string inputFile, std::string outputFile, int nrofThreads):
    m_timeStep{timeStep},
    m_duration{duration},
    m_inputFile{inputFile},
    m_outputFile{outputFile},
    m_nrofThreads{nrofThreads}
{
    for (int i=0; i<nrof_nodes; ++i)
    {
        m_detectors.push_back(std::make_unique<GroupDetection>(i));
    }
}

void Simulator::run()
{
    cout << "Starting process of group detection. The following parameters are used: " << endl;
    cout << "Number of nodes: " << m_detectors.size() << endl;
    cout << "Time step: " << m_timeStep << endl;
    cout << "Duration: " << m_duration << endl;
    cout << "Input file: " << m_inputFile << endl;
    cout << "Output file: " << m_outputFile << endl;
    cout << "Number of threads: " << m_nrofThreads << endl;
    cout << "Group threshold: " << GroupDetection::ms_threshold << endl;
    cout << "Group destruction coefficient: " << GroupDetection::ms_destructionCoefficient << endl;

    cout << endl << endl;

    // Try to open the input file
    ifstream input(m_inputFile);
    if (input.fail())
    {
        cerr << "An error ocurred while openning the input file. Details:" << endl;
        cerr << strerror(errno) << endl;
        return;
    }

    long currentTime = 0;
    long endTime = m_duration == -1 ? LONG_MAX : m_duration;
    unique_ptr<Connection> lastconn = nullptr;

    cout << "Running..." << endl;
    while(currentTime <= endTime && input)
    {
        long endStep = currentTime + m_timeStep;
        // cout << "Advancing step to "
        //      << endStep << endl;

        int ccount = 0;
        // Check if there is a pending connection
        if (lastconn != nullptr)
        {
            // Check if the pending connection is inside the current time window
            if (lastconn->time() <= endStep)
            {
                processConnection(*lastconn);
                lastconn = nullptr;
            }
            // Advance the time window otherwise
            else
            {
                currentTime = endStep;
                continue;
            }
        }

        while(input)
        {
            // read connections until the time step is reached
            unique_ptr<Connection> conn = nullptr;
            try
            {
                conn = make_unique<Connection>(input);
            }
            catch (const std::runtime_error &re)
            {
                if (input.eof())
                {
                    continue;
                }
                else
                {
                    throw re;
                }
            }
            ccount++;
            if (conn->time() <= endStep)
            {
                processConnection(*conn);
            }
            else
            {
                lastconn = move(conn);
                break;
            }
        }
        // cout << "Loaded "
        //      << ccount
        //      << " connections" << endl;
        // currentTime = endStep;

        // run the group detection algorithm in t threads
        updateGroups(currentTime);
    }

    cout << "Completed." 
         << endl
         << "Writing output..."
         << endl;
    
    ofstream output(m_outputFile, ios::trunc);
    if (!output.is_open())
    {
        cerr << "Error while opening the output file. Detail: " << endl
             << strerror(errno) << endl; 
        return;
    }

    output << "{" << endl;

    int detector_index = 0;
    for (const auto &detector: m_detectors)
    {
        output << '"' << detector->node().id() << "\": [" 
               << endl;
        int index = 0;
        for (const auto &group: detector->groupHistory())
        {
            output << "{" << endl
                   << "\"created\": " << group->created() << "," 
                   << endl 
                   << "\"destroyed\": " << group->destroyed() << ","
                   << endl
                   << "\"members\": "
                   << ext::to_string(group->memberKeys()) 
                   << endl
                   << "}";
                   if (index < (detector->groupHistory().size()-1))
                   {
                        output << ",";
                   }
                   output << endl;
            index++;
        }
        output << "]";
        if (detector_index < (m_detectors.size()-1))
        {
            output << ",";
        }
        output << endl;

        detector_index++;
    }

    output << "}";
    
}

void Simulator::processConnection(const Connection &conn)
{
    int nodeA = get<0>(conn.nodes());
    int nodeB = get<1>(conn.nodes());
    if (conn.type() == ConnectionType::UP)
    {
        m_detectors[nodeA]->node().addNeighbor(nodeB);
        m_detectors[nodeB]->node().addNeighbor(nodeA);
    }
    else
    {
        m_detectors[nodeA]->node().removeNeighbor(nodeB);
        m_detectors[nodeB]->node().removeNeighbor(nodeA);
    }
}

void Simulator::updateGroups(long time)
{
    int nodesPerThread = ceil(m_detectors.size()*1.0 / m_nrofThreads);

    int start = 0;
    int end = start+nodesPerThread;
    if (end > m_detectors.size())
    {
        end = m_detectors.size();
    }
    vector<future<void>> threads;

    // Create a direct reference to the function call in order to use with async
    auto func = bind(
        (void (Simulator::*)(long, std::vector<std::unique_ptr<GroupDetection>>::iterator, std::vector<std::unique_ptr<GroupDetection>>::iterator ))
        &Simulator::updateGroups, this, placeholders::_1, placeholders::_2, placeholders::_3);

    while (end <= m_detectors.size())
    {
        threads.push_back(async(launch::async, func, time, m_detectors.begin()+start, m_detectors.begin()+end));
        start += nodesPerThread;
        end += nodesPerThread;
    }
    for(const auto &t: threads)
    {
        t.wait();
    }
}

void Simulator::updateGroups(long time, std::vector<std::unique_ptr<GroupDetection>>::iterator begin, std::vector<std::unique_ptr<GroupDetection>>::iterator end)
{
    for(auto it = begin; it != end; it++)
    {
        (*it)->process(time);
    }
}