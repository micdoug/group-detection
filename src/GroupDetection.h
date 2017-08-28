//
// Created by micha on 8/22/2017.
//

#ifndef GROUP_DETECTION_GROUPDETECTION_H
#define GROUP_DETECTION_GROUPDETECTION_H

#include "Node.h"
#include "GroupMember.h"
#include <map>
#include <vector>

class GroupDetection final {
public:
    explicit GroupDetection(int nid);
    ~GroupDetection() = default;

    Node &node();
    const std::map<int, GroupMember> &currentGroup() const;
    const std::map<int, GroupMember> &transientGroup() const;
    const std::vector<std::map<int, GroupMember>> &groupHistory() const;
    
    void processGroups();

private:
    Node m_node;
    std::map<int, GroupMember> m_currentGroup;
    std::map<int, GroupMember> m_transientGroup;
    std::vector<std::map<int, GroupMember>> m_groupHistory;
};


#endif //GROUP_DETECTION_GROUPDETECTION_H
