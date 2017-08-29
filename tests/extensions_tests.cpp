#include "catch.h"
#include "../src/extensions.h"
#include <set>
#include <map>

using namespace std;
using ext::set_intersection;
using ext::containsKey;
using ext::getKeys;
using ext::set_difference;
using ext::set_union;

TEST_CASE("Métodos de extensão")
{
    SECTION("intersection")
    {
        set<int> setA { 1, 2, 3, 0};
        set<int> setB { 3, 4, 5, 0};
        auto inter = set_intersection(setA, setB);
        REQUIRE(containsKey(inter, 3));
        REQUIRE(containsKey(inter, 0));
        REQUIRE(!containsKey(inter, 1));
        REQUIRE(!containsKey(inter, 2));
        REQUIRE(!containsKey(inter, 5));
    }

    SECTION("difference")
    {
        set<int> setA { 1, 2, 3, 0};
        set<int> setB { 3, 4, 5, 0};
        auto inter = set_difference(setA, setB);
        REQUIRE(containsKey(inter, 1));
        REQUIRE(containsKey(inter, 2));
        REQUIRE(!containsKey(inter, 3));
        REQUIRE(!containsKey(inter, 0));
        REQUIRE(!containsKey(inter, 4));
        REQUIRE(!containsKey(inter, 5));
    }

    SECTION("getKeys")
    {
        map<int, int> mapping 
        {
            {1, 1},
            {2, 1},
            {3, 1}
        };
        auto keys = getKeys(mapping);
        REQUIRE(keys.size() == 3);
        REQUIRE(containsKey(keys, 1));
        REQUIRE(containsKey(keys, 2));
        REQUIRE(containsKey(keys, 3));
    }

    SECTION("set_union")
    {
        set<int> setA { 1, 2, 3};
        set<int> setB { 5, 4, 3};

        auto u = set_union(setA, setB);
        REQUIRE(containsKey(u, 1));
        REQUIRE(containsKey(u, 2));
        REQUIRE(containsKey(u, 3));
        REQUIRE(containsKey(u, 4));
        REQUIRE(containsKey(u, 5));
    }
}