//
// Created by micha on 8/22/2017.
//

#include "GroupDetection.h"
#include "extensions.h"
#include <algorithm>
#include <iostream>

using namespace std;

// Static members
int GroupDetection::ms_threshold = 10;
double GroupDetection::ms_destructionCoefficient = 0.5;

GroupDetection::GroupDetection(int nid):
    m_node{nid}
{
    m_currentGroup = make_unique<Group>();
    // Add itself in the current group
    m_currentGroup->addMember(nid);
    currentGroup()[nid].setIsActive(true);
    m_transientGroup = make_unique<Group>();
}

Node &GroupDetection::node()
{
    return m_node;
}

Group &GroupDetection::currentGroup() 
{
    return *m_currentGroup;
}

Group &GroupDetection::transientGroup() 
{
    return *m_transientGroup;
}

const std::vector<std::unique_ptr<Group>> &GroupDetection::groupHistory() const {
    return m_groupHistory;
}

void GroupDetection::processGroups()
{
    // Update currentGroup and transientGroup structures
    auto transient = transientGroup().memberKeys();
    auto current = currentGroup().memberKeys();

    cout << "Transient group: " << transientGroup().to_string() << endl;
    cout << "CurrentGroup group: " << currentGroup().to_string() << endl;

    // Transient group
    auto tg_responses = ext::set_intersection(transient, m_node.neighbors());
    auto tg_failures = ext::set_difference(transient, m_node.neighbors());

    cout << "tg_responses: " << ext::to_string(tg_responses) << endl;
    cout << "tg_failures: " << ext::to_string(tg_failures) << endl;

    // Current group
    auto cg_responses = ext::set_intersection(current, m_node.neighbors());
    auto cg_failures = ext::set_difference(current, m_node.neighbors());

    cout << "cg_responses: " << ext::to_string(cg_responses) << endl;
    cout << "cg_failures: " << ext::to_string(cg_failures) << endl;

    // New nodes
    auto new_nodes = ext::set_difference(m_node.neighbors(), ext::set_union(transient, current));

    cout << "new nodes: " << ext::to_string(new_nodes) << endl;
    

    // Begin: processing transient group
    for (const int resp: tg_responses)
    {
        transientGroup()[resp].incrementResponseCounter();
        if (transientGroup()[resp].responseCounter() >= ms_threshold)
        {
            currentGroup().addMember(resp);
            transientGroup().removeMember(resp);
            currentGroup()[resp].setIsActive(true);
        }
    }
    for (const int fail: tg_failures)
    {
        transientGroup()[fail].incrementFailureCounter();
        if (transientGroup()[fail].failureCounter() >= ms_threshold)
        {
            transientGroup().removeMember(fail);
        }
    }
    // End: processing transient group

    // Begin: processing current group
    for (const int resp: cg_responses)
    {
        currentGroup()[resp].incrementResponseCounter();
        if (!currentGroup()[resp].isActive() && currentGroup()[resp].responseCounter() >= ms_threshold)
        {
            currentGroup()[resp].setIsActive(true);
        }
    }
    for (const int fail: cg_failures)
    {
        currentGroup()[fail].incrementFailureCounter();
        if (fail != m_node.id() && currentGroup()[fail].failureCounter() >= ms_threshold)
        {
            currentGroup()[fail].setIsActive(false);
        }
    }
    // End: processing current group

    // Adding new nodes to transient group
    for (const int nnode: new_nodes)
    {
        cout << "Adding node " << std::to_string(nnode) << "." << std::endl;
        transientGroup().addMember(nnode);
        transientGroup()[nnode].incrementResponseCounter();
    }

    // Check the proportion of inactive nodes in the current group
    int total_nodes = currentGroup().size();
    int inactive_nodes = std::count_if(currentGroup().begin(), currentGroup().end(), 
                            [](const auto &member) { return !member.second->isActive(); });
    
    
    double proportion = inactive_nodes * 1.0 / total_nodes;
    // If the proportion of inactive is greater than the coefficient it is added in the history
    cout << "Proportion of inactive nodes " << proportion << std::endl;
    if (proportion > ms_destructionCoefficient)
    {
        cout << "Destroying group" << endl;
        m_groupHistory.push_back(std::move(m_currentGroup));
        // Resets current group and transient group
        m_currentGroup = make_unique<Group>();
        // Add itself in the current group
        currentGroup().addMember(m_node.id());
        currentGroup()[m_node.id()].setIsActive(true);
        m_transientGroup = make_unique<Group>();
    }
    cout << endl << endl;
}
