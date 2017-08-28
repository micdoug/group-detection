//
// Created by micha on 8/21/2017.
//

#include "Connection.h"
#include <string>
#include <stdexcept>

Connection::Connection(std::istream &input)
{
    std::string temp;
    input >> m_time;
    input >> temp;
    int nodeA, nodeB;
    input >> nodeA
          >> nodeB;
    m_nodes = std::tuple<int, int>(nodeA, nodeB);
    input >> temp;
    if (temp == "up")
    {
        m_type = ConnectionType::UP;
    }
    else if(temp == "down")
    {
        m_type = ConnectionType::DOWN;
    }
    else
    {
        throw std::invalid_argument("Invalid connection type " + temp);
    }
    this->checkInvariant();
}

Connection::Connection(long time, int nodeA, int nodeB, ConnectionType conType):
    m_nodes{nodeA, nodeB},
    m_time{time},
    m_type{conType}
{
    this->checkInvariant();
}

void Connection::checkInvariant()
{
    if (std::get<0>(m_nodes) == std::get<1>(m_nodes))
    {
        throw std::invalid_argument("A connection must be between two distinct nodes.");
    }
}

const std::tuple<int, int> &Connection::nodes() const {
    return m_nodes;
}

const ConnectionType &Connection::type() const {
    return m_type;
}

int Connection::time() const {
    return m_time;
}

std::ostream &operator<<(std::ostream &stream, const Connection &con) {
    std::cout << "At time " << con.time()
              << " connection between "
              << std::get<0>(con.nodes())
              << " and "
              << std::get<1>(con.nodes())
              << " " << (con.type() == ConnectionType::UP ? "up" : "down");
    return stream;
}
