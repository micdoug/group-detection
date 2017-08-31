//
// Created by micha on 8/22/2017.
//

#ifndef GROUP_DETECTION_GROUPDETECTION_H
#define GROUP_DETECTION_GROUPDETECTION_H

#include "Node.h"
#include "Group.h"
#include <memory>
#include <vector>

class GroupDetection final {
public:
    explicit GroupDetection(int nid);
    ~GroupDetection() = default;

    Node &node();
    
    const std::vector<std::unique_ptr<Group>> &groupHistory() const;
    void process(long time);
    Group &currentGroup();
    Group &transientGroup();

    // Static members
    static int ms_threshold;
    static double ms_destructionCoefficient;

private:
    
    Node m_node;
    std::unique_ptr<Group> m_currentGroup;
    std::unique_ptr<Group> m_transientGroup;
    std::vector<std::unique_ptr<Group>> m_groupHistory;
};


#endif //GROUP_DETECTION_GROUPDETECTION_H
