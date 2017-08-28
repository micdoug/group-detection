#include "catch.h"
#include "../src/Connection.h"
#include <sstream>

using namespace std;

TEST_CASE("Connection class tests")
{
    SECTION("construction from stream")
    {
        istringstream upstream("19 conn 0 1 up");
        istringstream downstream("20 conn 0 1 down");

        Connection upconn(upstream);
        REQUIRE(upconn.type() == ConnectionType::UP);
        REQUIRE(get<0>(upconn.nodes()) == 0);
        REQUIRE(get<1>(upconn.nodes()) == 1);
        REQUIRE(upconn.time() == 19);
        Connection downconn(downstream);
        REQUIRE(downconn.type() == ConnectionType::DOWN);
        REQUIRE(get<0>(downconn.nodes()) == 0);
        REQUIRE(get<1>(downconn.nodes()) == 1);
        REQUIRE(downconn.time() == 20);
    }

    SECTION("construction from invalid stream")
    {
        istringstream equalorigdest("12 conn 0 0 up");
        REQUIRE_THROWS(Connection(equalorigdest));

        istringstream invalidtype("10 conn 0 0 asdf");
        REQUIRE_THROWS(Connection(invalidtype));
    }
}