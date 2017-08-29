#include "catch.h"
#include "../src/Group.h"
#include "../src/extensions.h"

TEST_CASE("Group class tests")
{
    Group g;
    REQUIRE(g.created() == -1);
    REQUIRE(g.destroyed() == -1);
    REQUIRE(g.size() == 0);
    
    SECTION("add member")
    {
        g.addMember(0);
        REQUIRE(g.size() == 1);
        REQUIRE(g[0].id() == 0);
        REQUIRE(g[0].responseCounter() == 0);
        REQUIRE(g[0].failureCounter() == 0);
        g[0].incrementResponseCounter();
        REQUIRE(g[0].responseCounter() == 1);
    }

    SECTION("add existent member")
    {
        g.addMember(0);
        REQUIRE_THROWS(g.addMember(0));
    }

    SECTION("remove member")
    {
        g.addMember(0);
        REQUIRE(g.size() == 1);
        REQUIRE(ext::containsKey(g.memberKeys(), 0));
        g.removeMember(0);
        REQUIRE(g.size() == 0);
        REQUIRE(!ext::containsKey(g.memberKeys(), 0));
    }

    SECTION("remove non existent member")
    {
        REQUIRE_THROWS(g.removeMember(0));
    }
}