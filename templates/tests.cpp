#include "catch.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

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