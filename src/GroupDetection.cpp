//
// Created by micha on 8/22/2017.
//

#include "GroupDetection.h"
#include "extensions.h"

using namespace std;

GroupDetection::GroupDetection(int nid):
    m_node{nid}
{
}

Node &GroupDetection::node()
{
    return m_node;
}

const std::map<int, GroupMember> &GroupDetection::currentGroup() const
{
    return m_currentGroup;
}

const std::map<int, GroupMember> &GroupDetection::transientGroup() const
{
    return m_transientGroup;
}

const std::vector<std::map<int, GroupMember>> &GroupDetection::groupHistory() const {
    return m_groupHistory;
}

void GroupDetection::processGroups()
{
    // Update currentGroup and transientGroup structures
    auto transient = getKeys(m_transientGroup);
    auto current = getKeys(m_currentGroup);

    // Transient group
    auto tg_responses = intersection(transient, m_node.neighbors());
    auto tg_failures = difference(transient, m_node.neighbors());

    // Current group
    auto cg_responses = intersection(current, m_node.neighbors());
    auto cg_failures = difference(current, m_node.neighbors());

    
}
