#include "catch.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <array>

using namespace std;
using namespace Catch::Matchers;

///////////////////////
// template aliases

template <typename T>
using Dictionary = std::map<std::string, T>;

using Task = std::function<void()>;

template <typename T>
using RemoveReference_t = typename std::remove_reference<T>::type;

///////////////////////////////
// template variables

template <typename T>
constexpr T pi = 3.141534534534534;

template <typename T>
constexpr static bool IsIntegral_v = std::is_integral<T>::value;

TEST_CASE("templates")
{
    Dictionary<int> dict = {{"one", 1}, {"two", 2}};

    REQUIRE(pi<float> == ::Approx(3.1415));

    static_assert(IsIntegral_v<int>, "Error");
}

///////////////////////////////////////////////////////////////

void print()
{
    std::cout << "\n";
}

template <typename THead, typename... TTail>
void print(const THead& head, const TTail&... tail)
{
    std::cout << head << " ";
    print(tail...);
}

namespace Cpp17
{
    template <typename THead, typename... TTail>
    void print(const THead& head, const TTail&... tail)
    {
        std::cout << head << " ";

        if constexpr(sizeof...(tail) > 0)
            print(tail...);
        else
            std::cout << "\n";
    }
}

//////////////////////////////////////

template <typename... Ts>
struct Count;

template <typename THead, typename... TTail>
struct Count<THead, TTail...>
{
    const static size_t value = 1 + Count<TTail...>::value;
};

template <>
struct Count<>
{
    const static size_t value = 0;
};

TEST_CASE("head-tail for variadic templates")
{
    print(1, 3.14, "one"s);

    static_assert(Count<int, int, double, char>::value == 4, "Error");
}


////////////////////////////////////////////////////////////////
// std::tuple
struct KDouble
{
    double value;
private:
    KDouble(double v) : value{v}
    {}

    friend tuple<int, int, KDouble> calc_stats(const std::vector<int>& data);
};

tuple<int, int, KDouble> calc_stats(const std::vector<int>& data)
{
    std::vector<int>::const_iterator min_pos, max_pos;
    std::tie(min_pos, max_pos) = std::minmax_element(begin(data), end(data));

    double avg = std::accumulate(begin(data), end(data), 0.0) / data.size();

    return std::make_tuple(*min_pos, *max_pos, KDouble{avg});
}

TEST_CASE("tuples")
{
    std::pair<int, std::string> p1{1, "text"};
    auto p2 = std::make_pair(2, "two"s);

    std::tuple<int, double, std::string, std::vector<int>> t1{1, 3.14, "text", std::vector<int>{1, 2, 3}};
    auto t2 = std::make_tuple(1, 3.14, "text"s, std::vector<int>{1, 2, 3});

    REQUIRE(std::get<0>(t1) == 1);
    REQUIRE(std::get<2>(t1) == "text"s);

    std::vector<int> data = {3, 643, 23, 456, 66, 23, 1, 34, 66 };

    {
        int min, max;
        std::tie(min, max, std::ignore) = calc_stats(data); 
    }

    auto [min, max, avg] = calc_stats(data);

    REQUIRE(min == 1);
    REQUIRE(max == 643);

    int x, y, z;

    std::tuple<int&, int&, int&> ref_t{x, y, z};
    ref_t = std::make_tuple(1, 2, 3); // x == 1, y == 2, z == 3

    std::tie(x, y, z) = std::make_tuple(7, 8, 9);
}

TEST_CASE("comparing tuples")
{
    std::tuple<int, int, int> t1{1, 2, 3};
    std::tuple<int, int, int> t2{1, 2, 3};
    std::tuple<int, int, char> t3{1, 2, 4};

    REQUIRE(t1 == t2);
    REQUIRE(t1 != t3);
    REQUIRE(t1 < t3);

    int x, y, z;

    auto ref_tpl = std::tie(x, y, z);
    ref_tpl = t1;
    REQUIRE(ref_tpl == t2);
}

struct Person
{
    std::string fname;
    std::string lname;
    int age;

    auto tied() const
    {
        return std::tie(fname, lname, age);
    }

    bool operator==(const Person& other) const
    {
        return tied() == other.tied();
    }

    bool operator<(const Person& other) const
    {
        return std::tie(fname, lname, age) < std::tie(other.fname, other.lname, other.age);
    }
};


///////////////////////////
// constexpr

constexpr int factorial(int n)
{
    return (n == 0) ? 1 : n * factorial(n-1);
}

template <size_t N>
constexpr std::array<size_t, N> create_factorial_lookup()
{
    std::array<size_t, N> results{};

    for(size_t i = 0; i < N; ++i)
        results[i] = factorial(i);

    return results;
}

TEST_CASE("constexpr")
{
    int tab[factorial(5)] = {};

    constexpr auto lookup_table = create_factorial_lookup<10>();

    auto temp_table = create_factorial_lookup<15>();
}

template <typename It, typename Pred>
constexpr auto constexpr_count_if(It first, It last, Pred pred)
{
    size_t count{};
    for(; first != last; ++first)
        if (pred(*first))
            ++count;
    return count;
}

TEST_CASE("constexpr algorithm")
{
    constexpr std::array<int, 10> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    constexpr auto no_of_evens = constexpr_count_if(begin(data), end(data), [](int x) { return x % 2 == 0; });

    std::vector<int> vec = {1, 2, 3, 4};
    const auto no_of_evens_in_vec = constexpr_count_if(begin(vec), end(vec), [](int x) { return x % 2 == 0; });
}