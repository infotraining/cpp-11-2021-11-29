#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

TEST_CASE("lambda exercise")
{
    using namespace Catch::Matchers;

    vector<int> data = {1, 6, 3, 5, 8, 9, 13, 12, 10, 45};

    auto is_even = [](int x)
    { return x % 2 == 0; };

    SECTION("count even numbers")
    {
        auto evens_count = 0;

        std::for_each(begin(data), end(data), [&evens_count](int x)
            { evens_count += (x % 2 == 0 ? 1 : 0); });

        evens_count = count_if(begin(data), end(data), is_even);

        REQUIRE(evens_count == 4);
    }

    SECTION("copy evens to vector")
    {
        vector<int> evens;

        std::copy_if(begin(data), end(data), std::back_inserter(evens), is_even);

        REQUIRE_THAT(evens, Equals(vector<int> {6, 8, 12, 10}));
    }

    SECTION("create container with squares")
    {
        vector<int> squares;

        transform(begin(data), end(data), back_inserter(squares), [](int x)
            { return x * x; });

        REQUIRE_THAT(squares, Equals(vector<int> {1, 36, 9, 25, 64, 81, 169, 144, 100, 2025}));
    }

    SECTION("remove from container items divisible by any number from a given array")
    {
        const array<int, 3> eliminators = {3, 5, 7};

        // data.erase(std::remove_if(begin(data), end(data),
        //                [eliminators](int i)
        //                {
        //                    for (auto&& d : eliminators)
        //                    {
        //                        if (i % d == 0)
        //                            return true;
        //                    }
        //                    return false;
        //                }),
        //     end(data));

        data.erase(std::remove_if(begin(data), end(data),
                       [eliminators](int i)
                       {
                           return std::any_of(begin(eliminators), end(eliminators), [i](int n)
                               { return i % n == 0; });
                       }),
            end(data));

        REQUIRE_THAT(data, Equals(vector<int> {1, 8, 13}));
    }

    SECTION("calculate average")
    {
        auto sum = 0.0;
        double avg = std::accumulate(begin(data), end(data), 0.0 ) / data.size();

        REQUIRE(avg == ::Approx(11.2));

        SECTION("create two containers - 1st with numbers less or equal to average & 2nd with numbers greater than average")
        {
            vector<int> less_equal_than_avg;
            vector<int> greater_than_avg;

            partition_copy(begin(data), end(data), back_inserter(less_equal_than_avg), back_inserter(greater_than_avg), [avg](int x) { return x <= avg;} );

            REQUIRE_THAT(less_equal_than_avg, Contains(vector<int> {1, 6, 3, 5, 8, 9, 10}));
            REQUIRE_THAT(greater_than_avg, Contains(vector<int> {13, 12, 45}));
        }
    }
}

TEST_CASE("accumulate as fold")
{
    std::vector<int> vec = {1, 2, 3, 4};

    std::string result = std::accumulate(begin(vec), end(vec), "vec: "s, [](const std::string& result, int item) { return result + std::to_string(item) + ";"; });

    REQUIRE(result == "vec: 1;2;3;4;");
}