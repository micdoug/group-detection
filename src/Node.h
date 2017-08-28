//
// Created by micha on 8/21/2017.
//

#ifndef GROUP_DETECTION_NODE_H
#define GROUP_DETECTION_NODE_H

#include <set>

class Node final {
public:
    explicit Node(int id);
    ~Node() = default;

    void addNeighbor(int nid);
    void removeNeighbor(int nid);

    const std::set<int> &neighbors() const;
private:
    int m_id;
    std::set<int> m_neighbors;
};


#endif //GROUP_DETECTION_NODE_H
