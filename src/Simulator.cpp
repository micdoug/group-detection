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
#include <cstring>

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

    cout << "Combining detected groups..." << endl;
    combineGroups();
    cout << "Unifying groups..." << endl;
    unifyGroups();
    cout << "Completed..." << endl;
    
    // ofstream output(m_outputFile, ios::trunc);
    // if (!output.is_open())
    // {
    //     cerr << "Error while opening the output file. Detail: " << endl
    //          << strerror(errno) << endl; 
    //     return;
    // }

    // output << "{" << endl;

    // int detector_index = 0;
    // for (const auto &detector: m_detectors)
    // {
    //     output << '"' << detector->node().id() << "\": [" 
    //            << endl;
    //     int index = 0;
    //     for (const auto &group: detector->groupHistory())
    //     {
    //         output << "{" << endl
    //                << "\"created\": " << group->created() << "," 
    //                << endl 
    //                << "\"destroyed\": " << group->destroyed() << ","
    //                << endl
    //                << "\"members\": "
    //                << ext::to_string(group->memberKeys()) 
    //                << endl
    //                << "}";
    //                if (index < (detector->groupHistory().size()-1))
    //                {
    //                     output << ",";
    //                }
    //                output << endl;
    //         index++;
    //     }
    //     output << "]";
    //     if (detector_index < (m_detectors.size()-1))
    //     {
    //         output << ",";
    //     }
    //     output << endl;

    //     detector_index++;
    // }

    // output << "}";
    
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
        //cout << "Processing from node " << start << " to " << end << endl;
        threads.push_back(async(launch::async, func, time, m_detectors.begin()+start, m_detectors.begin()+end));
        if (end == m_detectors.size())
        {
            break;
        }
        start += nodesPerThread;
        end += nodesPerThread;
        if (end > m_detectors.size())
        {
            end = m_detectors.size();
        }
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

void Simulator::combineGroups()
{
    cout << "Analisando " << m_detectors.size() << " detectores de grupos" << endl;
    // Para cada dispositivo da rede
    for (const auto &detector: m_detectors)
    {
        // Para cada grupo daquele dispositivo
        for (const auto &group: detector->groupHistory())
        {
            bool added = false;
            // Verifica se existe interseção de tempo
            for (int i=0; i<m_combinedGroups.size(); ++i)
            {
                // Se o grupo 
                if (m_combinedGroups[i]->created() > group->destroyed())
                {
                    break;
                }
                // Se existe a interseção, verifica a correlação entre membros do grupo
                else if (m_combinedGroups[i]->hasTimeIntersection(*group))
                {
                    auto member_intersec = ext::set_intersection(m_combinedGroups[i]->memberKeys(), group->memberKeys());
                    auto member_union = ext::set_union(m_combinedGroups[i]->memberKeys(), group->memberKeys());
                    double correlation = member_intersec.size()*1.0/member_union.size();
                    if (correlation >= 0.5)
                    {
                        auto new_nodes = ext::set_difference(group->memberKeys(), m_combinedGroups[i]->memberKeys());
                        //cout << ext::to_string(m_combinedGroups[i]->memberKeys()) << endl;
                        //cout << "Adicionando " << ext::to_string(new_nodes) << endl;
                        for (auto node: new_nodes)
                        {
                            m_combinedGroups[i]->addMember(node);
                        }
                        if (m_combinedGroups[i]->created() > group->created())
                        {
                            //cout << "Criação ajustando para " << group->created() << endl;
                            m_combinedGroups[i]->setCreated(group->created());
                            if (m_combinedGroups[i]->created() == -1)
                            {
                                throw std::runtime_error("-1");
                            }
                        }
                        if (m_combinedGroups[i]->destroyed() < group->destroyed())
                        {
                            //cout << "Destruição ajustando para " << group->destroyed() << endl;
                            m_combinedGroups[i]->setDestroyed(group->destroyed());
                        }
                        added = true;
                        break;
                    }
                }
            }
            // cria nova entrada para o grupo ordenada pelo tempo de criação
            if (!added)
            {
                int i = 0;
                for (;i<m_combinedGroups.size(); ++i)
                {
                    if (m_combinedGroups[i]->created() > group->created())
                    {
                        break;
                    }
                }
                m_combinedGroups.insert(m_combinedGroups.begin()+i, make_unique<Group>(*group));
                // cout << "Foi inserido o grupo " 
                //      << ext::to_string(m_combinedGroups[i]->memberKeys()) 
                //      << " que foi criado em " 
                //      << m_combinedGroups[i]->created()
                //      << endl;
                     if (m_combinedGroups[i]->created() == -1)
                     {
                         throw std::runtime_error("-1");
                     }
            }
        }
    }

    // Escreve a resposta no arquivo de saída
    // ofstream outfile(m_outputFile, ios::trunc);
    // if (!outfile.is_open())
    // {
    //     cerr << "Error while opening the output file. Detail: " << endl
    //          << strerror(errno) << endl; 
    //     return;
    // }
    // outfile << "[" << endl;
    // int totalGroups = m_combinedGroups.size();
    // for (const auto &cgroup: m_combinedGroups)
    // {
    //     totalGroups--;
    //     outfile << "{" << endl;
    //     outfile << "\"start\": " << cgroup->created() << "," << endl;
    //     outfile << "\"end\": " << cgroup->destroyed() << "," << endl;
    //     outfile << "\"members\": [ ";
    //     auto members = cgroup->memberKeys();
    //     int total = members.size();
    //     for(const auto &m: members)
    //     {
    //         --total;
    //         if (total)
    //         {
    //             outfile << m << ", ";
    //         }
    //         else
    //         {
    //             outfile << m;
    //         }
    //     }
    //     outfile << " ]" << endl;
    //     if (totalGroups)
    //     {
    //         outfile << "}," << endl;
    //     }
    //     else 
    //     {
    //         outfile << "}" << endl;
    //     }
    // }
    // outfile << "]" << endl;

    
}

void Simulator::unifyGroups()
{
    while (m_combinedGroups.size())
    {
        cout << "Remaining " << m_combinedGroups.size() << " groups." << endl;
        vector<int> remove = {0};
        auto &group = m_combinedGroups[0];
        UnifiedGroup ugroup(group->memberKeys());
        ugroup.addEncounter(group->created(), group->destroyed());
        for (int i=1; i<m_combinedGroups.size();++i)
        {
            if (ugroup.hasIntersection(m_combinedGroups[i]->created(), m_combinedGroups[i]->destroyed()))
            {
                continue;
            }
            auto mintersec = ext::set_intersection(ugroup.members(), m_combinedGroups[i]->memberKeys());
            auto munion = ext::set_union(ugroup.members(), m_combinedGroups[i]->memberKeys());
            double proportion = mintersec.size()*1.0/munion.size();
            if (proportion >= 0.5)
            {
                ugroup.addEncounter(m_combinedGroups[i]->created(), m_combinedGroups[i]->destroyed());
                auto new_nodes = ext::set_difference(m_combinedGroups[i]->memberKeys(), ugroup.members());
                for (auto node: new_nodes)
                {
                    ugroup.addMember(node);
                }
                remove.insert(remove.begin(), i);
            }
        }
        m_unifiedGroups.push_back(make_unique<UnifiedGroup>(ugroup));
        for (auto r: remove)
        {
            m_combinedGroups.erase(m_combinedGroups.begin()+r);
        }
    }


    // Escreve a resposta no arquivo de saída
    ofstream outfile(m_outputFile, ios::trunc);
    if (!outfile.is_open())
    {
        cerr << "Error while opening the output file. Detail: " << endl
             << strerror(errno) << endl; 
        return;
    }
    outfile << "[" << endl;
    int totalGroups = m_unifiedGroups.size();
    for (const auto &ugroup: m_unifiedGroups)
    {
        totalGroups--;
        outfile << "{" << endl;

        outfile << "\"start\": [";
        int totalEncounters = ugroup->encounters().size();
        for (const auto &encounter: ugroup->encounters())
        {
            totalEncounters--;
            if (totalEncounters)
            {
                outfile << encounter.start() << ", ";
            }
            else
            {
                outfile << encounter.start();
            }
        }
        outfile << " ]," << endl;

        outfile << "\"end\": [";
        totalEncounters = ugroup->encounters().size();
        for (const auto &encounter: ugroup->encounters())
        {
            totalEncounters--;
            if (totalEncounters)
            {
                outfile << encounter.end() << ", ";
            }
            else
            {
                outfile << encounter.end();
            }
        }
        outfile << " ]," << endl;

        outfile << "\"members\": [ ";
        auto members = ugroup->members();
        int total = members.size();
        for(const auto &m: members)
        {
            --total;
            if (total)
            {
                outfile << m << ", ";
            }
            else
            {
                outfile << m;
            }
        }
        outfile << " ]" << endl;
        if (totalGroups)
        {
            outfile << "}," << endl;
        }
        else 
        {
            outfile << "}" << endl;
        }
    }
    outfile << "]" << endl;
}
