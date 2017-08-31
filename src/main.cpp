#include <iostream>
#include <boost/program_options.hpp>
#include <string>
#include <climits>
#include "Simulator.h"

namespace po = boost::program_options;

using namespace std;


// Parâmetros de simulação
const int nro_nodes = 100;

using namespace std;

/**
 * Show a simple instruction on how to run the application.
 **/
void showHelp(const po::options_description &options)
{
    cout << "Use:" << endl
         << "group-detection [input-file] [output-file] <options>"
         << endl << endl;
    cout << options << endl;
}

int main(int argc, char **argv) {

    // Command line parameters
    string input_file;
    string output_file;
    int nrof_threads;
    long timeStep;
    long duration = -1;
    int nrofNodes;

    // Build a dictionary of the required parameters to run the application.
    po::options_description desc("Allowed options");
    desc.add_options()
         ("help,h", "Show help message.")
         ("nodes,n", po::value<int>(&nrofNodes), "The number of nodes in the scenario. The nodes will be indexed from 0 to (n-1).")
         ("time-step,s", po::value<long>(&timeStep)->default_value(60), "The time step used by the connection simulator (in seconds).")
         ("duration,d", po::value<long>(&duration), "The duration of simulation (in seconds). If not specified, the simulation will run until the input file ends.")
         ("group-threshold,g", po::value<int>(), "The value used as threshold to add a node in the current group.")
         ("group-destruction-coefficient,c", po::value<double>(), "The value used as destruction coefficient in the algorithm.")
         ("threads,t", po::value<int>(&nrof_threads)->default_value(1), "The number of threads used to process the groups.");
    po::options_description hidden;
    hidden.add_options()
            ("input,i", po::value<string>(&input_file), "The file with connection events.")
            ("output,o", po::value<string>(&output_file), "The file used to write the detected groups.");
    po::options_description all_options;
    all_options.add(desc).add(hidden);
    po::positional_options_description pos;
    pos.add("input", 1);
    pos.add("output", 1);
    po::variables_map vm;

    // Process the command line arguments.
    po::store(po::command_line_parser(argc, argv).options(all_options).positional(pos).run(), vm);
    po::notify(vm);

    // Check if a help command was executed.
    if (vm.count("help"))
    {
        showHelp(desc);
        return 1;
    }
    
    // Check if an input was defined.
    if (!vm.count("input"))
    {
        cout << "You need to pass an input file." << endl;
        showHelp(desc);
        return 1;
    }

    // Check if an output was defined.
    if (!vm.count("output"))
    {
        cout << "You need to pass an output file." << endl;
        showHelp(desc);
        return 1;
    }

    if (vm.count("group-threshold"))
    {
        int groupThreshold = vm["group-threshold"].as<int>();
        if (groupThreshold < 2)
        {
            cout << "The group-threshold must be greater than 1." << endl;
            showHelp(desc);
            return 1;
        }
        else
        {
            GroupDetection::ms_threshold = groupThreshold;
        }
    }

    if (vm.count("group-destruction-coefficient"))
    {
        double dc = vm["group-destruction-coefficient"].as<double>();
        if (dc < 0.0 || dc > 1.0)
        {
            cout << "The group-destruction-coefficient must be a value between 0.0 and 1.0" << endl;
            showHelp(desc);
            return 1;
        }
        else
        {
            GroupDetection::ms_destructionCoefficient = dc;
        }
    }

    if (!vm.count("nodes"))
    {
        cout << "You need to specify the number of nodes in the simulation." << endl;
        showHelp(desc);
        return 1;
    }
    else if(nrofNodes < 1)
    {
        cout << "The number of nodes must be a positive integer value." << endl;
        showHelp(desc);
        return 1;
    }

    // Creates a simulator 
    Simulator sim(nrofNodes, timeStep, duration, input_file, output_file, nrof_threads);
    sim.run();

    return 0;
}