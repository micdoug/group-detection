#include "catch.h"
#include "../src/Connection.h"
#include <sstream>

using namespace std;

TEST_CASE("Connection class tests")
{
    SECTION("construction from stream")
    {
        istringstream upstream("19 CONN 0 1 up");
        istringstream downstream("20 CONN 0 1 down");

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
        istringstream equalorigdest("12 CONN 0 0 up");
        REQUIRE_THROWS(Connection(equalorigdest));

        istringstream invalidtype("10 CONN 0 0 asdf");
        REQUIRE_THROWS(Connection(invalidtype));

        istringstream invalid1("asdf CONN 10 15 asdf");
        REQUIRE_THROWS(Connection(invalid1));

        istringstream invalid2("10 fjfj 10 15 up");
        REQUIRE_THROWS(Connection(invalid2));

        istringstream invalid3("10 CONN a 1 up");
        istringstream invalid4("10 CONN 1 a up");
        REQUIRE_THROWS(Connection(invalid3));
        REQUIRE_THROWS(Connection(invalid4));

        istringstream invalid5("");
        REQUIRE_THROWS(Connection(invalid5));
    }
}