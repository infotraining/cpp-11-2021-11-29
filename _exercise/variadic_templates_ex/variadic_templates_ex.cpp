#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>

using namespace std;

namespace vt
{
    // TODO
}

TEST_CASE("variadic sum")
{
    SECTION("for ints")
    {
        auto sum = vt::sum(1, 3, 3);

        REQUIRE(sum == 7);
        static_assert(is_same<int, decltype(sum)>::value, "Error");
    }

    SECTION("for floating points")
    {
        auto dbl_sum = vt::sum(1.1, 3.0f, 3);

        REQUIRE(dbl_sum == Approx(7.1));
        static_assert(is_same<double, decltype(dbl_sum)>::value, "Error");
    }

    SECTION("for strings")
    {
        auto text = vt::sum("Hello", string("world"), "!");

        REQUIRE(text == "Helloworld!");
        static_assert(is_same<string, decltype(text)>::value, "Error");
    }
}

TEST_CASE("select for tuple")
{
    std::tuple<int, std::string, std::string, std::vector<int>> row{1, "first-name", "last-name", std::vector<int>{1, 2, 3}};

    REQUIRE(vt::select<0, 2, 3>(row) == std::make_tuple(1, "last-name", std::vector<int>{1, 2, 3}));
    REQUIRE(vt::select<0, 0, 0>(row) == std::make_tuple(1, 1, 1));
    REQUIRE(vt::select<3, 2, 1, 0>(row) == std::make_tuple(std::vector<int>{1, 2, 3}, "last-name", "first-name", 1));
}


TEST_CASE("call_n_times wrapper")
{
    int counter{};
    std::vector<std::tuple<int, std::string>> results;

    auto func = [&counter, &results](auto&&... args) { ++counter;  results.emplace_back(std::forward<decltype(args)>(args)...); };

    vt::call_n_times(5, func, 1, "one"s); // TODO - it should call 5 times func function with the following argument (5 x func(1, "one"s))

    REQUIRE(counter == 5);
    REQUIRE(results.size() == 5);
    REQUIRE(std::all_of(begin(results), end(results), [](const auto& item) { return item == std::make_tuple(1, "one"s);}));
}