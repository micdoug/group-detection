//
// Created by micha on 8/21/2017.
//

#include "Node.h"
#include <stdexcept>
#include <string>

Node::Node(int id):
    m_id{id}
{
    if (id < 0)
    {
        throw std::invalid_argument("The node id must be greater or equal 0.");
    }
}

void Node::addNeighbor(int nid)
{
    // Check if the node is already a neighbor
    if (nid == m_id)
    {
        throw std::invalid_argument("Can't add itself as a neighbor.");
    }
    if (m_neighbors.find(nid) != m_neighbors.end())
    {
        throw std::invalid_argument("The node " + std::to_string(nid) + " is already a neighbor of " + std::to_string(m_id));
    }
    m_neighbors.insert(nid);
}

const std::set<int> &Node::neighbors() const
{
    return m_neighbors;
}

void Node::removeNeighbor(int nid)
{
    // Check if the node is really a neighbor
    if (m_neighbors.find(nid) == m_neighbors.end())
    {
        throw std::invalid_argument("The node " + std::to_string(nid) + " is not a neighbor of " + std::to_string(m_id));
    }
    m_neighbors.erase(nid);
}

