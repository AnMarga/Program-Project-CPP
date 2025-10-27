#define CATCH_CONFIG_MAIN
#include "../src/vector.h"
#include "../include/catch.hpp"
#include <string>

TEST_CASE("Simple vector operations") {
    Vector<std::string> v;

    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    v.resize(3);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() >= 3);

    REQUIRE(v[0].empty());
    REQUIRE(v[1].empty());
    REQUIRE(v[2].empty());

    v[0] = "a";
    v[1] = "b";
    v[2] = "c";

    REQUIRE(v.front() == "a");
    REQUIRE(v.back() == "c");
    REQUIRE(v.at(1) == "b");

    v.resize(2);
    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == "a");
    REQUIRE(v[1] == "b");

    v.push_back("zog");
    REQUIRE(v.size() == 3);
    REQUIRE(v.back() == "zog");

    size_t old_cap = v.capacity();
    v.push_back("foo");
    REQUIRE(v.size() == 4);
    REQUIRE(v.back() == "foo");
    REQUIRE(v.capacity() >= old_cap);
}

TEST_CASE("Reserve and reallocation") {
    Vector<std::string> v;
    v.reserve(10);
    REQUIRE(v.capacity() >= 10);
    REQUIRE(v.size() == 0);

    v.push_back("foo");
    v.push_back("bar");
    REQUIRE(v.size() == 2);

    size_t old_cap = v.capacity();
    v.reserve(old_cap + 10);
    REQUIRE(v.capacity() > old_cap);

    REQUIRE(v[0] == "foo");
    REQUIRE(v[1] == "bar");
}

TEST_CASE("Copy and move semantics") {
    Vector<std::string> v1;
    v1.push_back("foo");

    auto p1 = &v1[0];

    SECTION("Copy constructor") {
        Vector<std::string> v2{v1};
        REQUIRE(&v2[0] != p1);
        REQUIRE(v2.size() == v1.size());
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
        REQUIRE(v2.size() == 1);
        REQUIRE(v2[0] == "foo");
        REQUIRE(v1.size() == 0);
    }

    SECTION("Move assignment") {
        Vector<std::string> v2;
        v2.push_back("bar");
        v2.push_back("zog");

        v2 = std::move(v1);
        REQUIRE(v2.size() == 1);
        REQUIRE(v2[0] == "foo");
        REQUIRE(v1.size() == 0);
    }
}

TEST_CASE("Correct reallocation with multiple push_backs") {
    Vector<std::string> v;
    v.push_back("foo");
    v.push_back("bar");
    v.push_back("zog");

    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == "foo");
    REQUIRE(v[1] == "bar");
    REQUIRE(v[2] == "zog");

    size_t old_cap = v.capacity();
    v.push_back("extra");
    REQUIRE(v.size() == 4);
    REQUIRE(v[3] == "extra");
    REQUIRE(v[0] == "foo");
    REQUIRE(v[1] == "bar");
    REQUIRE(v[2] == "zog");
    REQUIRE(v.capacity() >= old_cap);
}

TEST_CASE("At() throws on out of range") {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    REQUIRE_NOTHROW(v.at(1));
    REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
}

/*
TEST_CASE("Front and back throw when empty") {
    Vector<int> v;
    REQUIRE_THROWS_AS(v.front(), std::out_of_range);
    REQUIRE_THROWS_AS(v.back(), std::out_of_range);
}
*/
