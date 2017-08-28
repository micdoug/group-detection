#include "catch.h"
#include "../src/GroupMember.h"

TEST_CASE("GroupMember class tests")
{
    GroupMember groupMember(0);
    SECTION("increment response counter")
    {
        REQUIRE(groupMember.responseCounter() == 0);
        REQUIRE(groupMember.failureCounter() == 0);
        groupMember.incrementResponseCounter();
        REQUIRE(groupMember.responseCounter() == 1);
        REQUIRE(groupMember.failureCounter() == 0);
        groupMember.incrementResponseCounter();
        REQUIRE(groupMember.responseCounter() == 2);
        REQUIRE(groupMember.failureCounter() == 0);
        groupMember.incrementResponseCounter();
        REQUIRE(groupMember.responseCounter() == 3);
        REQUIRE(groupMember.failureCounter() == 0);
    }

    SECTION("reset response counter")
    {
        groupMember.incrementResponseCounter();
        groupMember.incrementResponseCounter();
        groupMember.incrementResponseCounter();
        REQUIRE(groupMember.responseCounter() == 3);
        groupMember.incrementFailureCounter();
        REQUIRE(groupMember.responseCounter() == 0);
        REQUIRE(groupMember.failureCounter() == 1);
    }

    SECTION("increment failure counter")
    {
        REQUIRE(groupMember.responseCounter() == 0);
        REQUIRE(groupMember.failureCounter() == 0);
        groupMember.incrementFailureCounter();
        REQUIRE(groupMember.responseCounter() == 0);
        REQUIRE(groupMember.failureCounter() == 1);
        groupMember.incrementFailureCounter();
        REQUIRE(groupMember.responseCounter() == 0);
        REQUIRE(groupMember.failureCounter() == 2);
        groupMember.incrementFailureCounter();
        REQUIRE(groupMember.responseCounter() == 0);
        REQUIRE(groupMember.failureCounter() == 3);
    }

    SECTION("reset failure counter")
    {
        groupMember.incrementFailureCounter();
        groupMember.incrementFailureCounter();
        groupMember.incrementFailureCounter();
        REQUIRE(groupMember.failureCounter() == 3);
        groupMember.incrementResponseCounter();
        REQUIRE(groupMember.responseCounter() == 1);
        REQUIRE(groupMember.failureCounter() == 0);
    }
}