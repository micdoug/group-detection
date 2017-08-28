//
// Created by micha on 8/21/2017.
//

#ifndef GROUP_DETECTION_CONNECTION_H
#define GROUP_DETECTION_CONNECTION_H

#include <iostream>
#include <tuple>

/**
 * Represents the type of connection event between two nodes.
 * Up refers to a connection creation and Down refers to a connection
 * lost.
 */
enum class ConnectionType {
    UP,
    DOWN
};

/**
 * Represents a connection event between two nodes.
 */
class Connection final {
public:
    explicit Connection(std::istream &input);
    Connection(long time, int nodeA, int nodeB, ConnectionType conType);
    ~Connection() = default;

    /**
     * Represent the nodes involved in the connection.
     */
    const std::tuple<int, int> &nodes() const;

    /**
     * Returns the type of the connection event.
     * @return
     */
    const ConnectionType &type() const;

    /**
     * The time that the event happened.
     */
    int time() const;

private:
    void checkInvariant();

    long m_time;
    std::tuple<int, int> m_nodes;
    ConnectionType m_type;
};

std::ostream &operator<<(std::ostream &stream, const Connection &con);

#endif //GROUP_DETECTION_CONNECTION_H
