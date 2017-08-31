#include "catch.h"
#include "../src/GroupDetection.h"
#include "../src/extensions.h"

TEST_CASE("GroupDetection class tests")
{
    GroupDetection::ms_threshold = 2;
    GroupDetection::ms_destructionCoefficient = 0.5;

    GroupDetection detection(0);
    REQUIRE(detection.transientGroup().size() == 0);
    REQUIRE(detection.currentGroup().size() == 1);

    detection.node().addNeighbor(1);

    // Add node 1 to transient group
    detection.process(0L);
    REQUIRE(detection.transientGroup().size() == 1);
    REQUIRE(ext::containsKey(detection.transientGroup().memberKeys(), 1));
    REQUIRE(detection.currentGroup().size() == 1);

    // Add node 1 to current group
    detection.process(1L);
    REQUIRE(detection.transientGroup().size() == 0);
    REQUIRE(detection.currentGroup().size() == 2);
    REQUIRE(ext::containsKey(detection.currentGroup().memberKeys(), 1));
    REQUIRE(detection.currentGroup().created() == 1L);

    // Nothing happens
    detection.node().removeNeighbor(1);
    detection.process(2L);
    REQUIRE(detection.transientGroup().size() == 0);
    REQUIRE(detection.currentGroup().size() == 2);
    REQUIRE(ext::containsKey(detection.currentGroup().memberKeys(), 1));
    REQUIRE(detection.currentGroup()[1].isActive());

    // Node 1 still away, it will be set as inactive
    detection.process(3L);
    REQUIRE(detection.transientGroup().size() == 0);
    REQUIRE(detection.currentGroup().size() == 2);
    REQUIRE(ext::containsKey(detection.currentGroup().memberKeys(), 1));
    REQUIRE(!detection.currentGroup()[1].isActive());

    // Add node 2 to current group
    detection.node().addNeighbor(2);
    detection.process(4L);
    detection.process(5L);
    REQUIRE(detection.transientGroup().size() == 0);
    REQUIRE(detection.currentGroup().size() == 3);
    REQUIRE(ext::containsKey(detection.currentGroup().memberKeys(), 2));
    REQUIRE(!detection.currentGroup()[1].isActive());

    // Node 1 needs to be set as active again
    detection.node().addNeighbor(1);
    detection.process(6L);
    detection.process(7L);
    REQUIRE(detection.currentGroup()[1].isActive());

    // Set all nodes to inactive to destroy the group
    detection.node().removeNeighbor(1);
    detection.node().removeNeighbor(2);
    detection.process(8L);
    detection.process(9L);
    REQUIRE(detection.transientGroup().size() == 0);
    REQUIRE(detection.currentGroup().size() == 1);
    REQUIRE(detection.groupHistory().size() == 1);
    REQUIRE(detection.groupHistory()[0]->destroyed() == 9L);

}