#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include <memory>
#include "GroupDetection.h"
#include "Connection.h"

class Simulator final
{
public:
    Simulator(int nrof_nodes, long timeStep, long duration, 
        std::string input_file, std::string output_file, int nrofThreads);
    ~Simulator() = default;

    void run();

private:
    void processConnection(const Connection &conn);
    void updateGroups(long time);
    void updateGroups(long time, std::vector<std::unique_ptr<GroupDetection>>::iterator begin, std::vector<std::unique_ptr<GroupDetection>>::iterator end);

    long m_timeStep;
    long m_duration;
    int m_nrofThreads;
    std::string m_inputFile;
    std::string m_outputFile;
    std::vector<std::unique_ptr<GroupDetection>> m_detectors;
};

#endif