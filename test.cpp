#define CATCH_CONFIG_MAIN
#include "vector.h"
#include <string>
#include "include/catch.hpp"

TEST_CASE("Simple vector operations") {
    Vector<std::string> v;

    REQUIRE(v.size() == 0);

    v.resize(3);
    REQUIRE(v[0].empty());
    REQUIRE(v[1].empty());
    REQUIRE(v[2].empty());

    v[2] = "foo";
    v[1] = "bar";

    REQUIRE(v[2] == "foo");
    REQUIRE(v[1] == "bar");

    REQUIRE(v.back() == "foo");

    v.push_back("zog");
    REQUIRE(v.size() == 4);
    REQUIRE(v.back() == "zog");
}

TEST_CASE("Copy and move semantics") {
    Vector<std::string> v1;
    v1.push_back("foo");

    auto p1 = &v1[0];

    SECTION("Copy constructor") {
        Vector<std::string> v2{v1};
        REQUIRE(&v2[0] != p1);
        REQUIRE(v2[0] == "foo");
    }

    SECTION("Copy assignment") {
        Vector<std::string> v3;
        v3.push_back("bar");
        v3.push_back("zog");

        v3 = v1;
        REQUIRE(v3.size() == 1);
        REQUIRE(v3[0] == "foo");
    }

    SECTION("Move constructor") {
        Vector<std::string> v2{std::move(v1)};
        REQUIRE(v2[0] == "foo");
        REQUIRE(v1.size() == 0);
    }

    SECTION("Move assignment") {
        Vector<std::string> v2;
        v2.push_back("bar");
        v2.push_back("zog");

        v2 = std::move(v1);
        REQUIRE(v2[0] == "foo");
        REQUIRE(v1.size() == 0);
    }
}

TEST_CASE("Correct reallocation") {
    Vector<std::string> v;
    v.push_back("foo");
    v.push_back("foo");
    v.push_back("foo");

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == "foo");
    REQUIRE(v[1] == "foo");
    REQUIRE(v[2] == "foo");
}
