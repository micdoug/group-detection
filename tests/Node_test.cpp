#include "catch.h"
#include "../src/Node.h"

TEST_CASE("Node class tests") 
{
    SECTION("id must be a positive integer") 
    {
        REQUIRE_THROWS(Node(-2));
        REQUIRE_THROWS(Node(-1));
        REQUIRE_NOTHROW(Node(0));
        REQUIRE_NOTHROW(Node(1));
        REQUIRE_NOTHROW(Node(100));
    }

    SECTION("add neighbors")
    {
        Node node(1);
        // Can't add itself as a neighbor
        REQUIRE_THROWS(node.addNeighbor(1));
        // Check adding a neighbor
        node.addNeighbor(0);
        REQUIRE(node.neighbors().find(0) != node.neighbors().end());
        REQUIRE(node.neighbors().find(1) == node.neighbors().end());
        // Can't readd a node
        REQUIRE_THROWS(node.addNeighbor(1));
    }

    SECTION("remove neighbors")
    {
        Node node(0);
        node.addNeighbor(1);
        // Check if a neighbor is really removed
        REQUIRE_NOTHROW(node.removeNeighbor(1));
        REQUIRE(node.neighbors().find(1) == node.neighbors().end());

        // Check if when a neighbor is not present a exception is thrown
        REQUIRE_THROWS(node.removeNeighbor(1));
    }
}