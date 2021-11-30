
#include "catch.hpp"
#include <array>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

int foo(int arg)
{
    return arg;
}

template <typename T>
void deduce_1(T arg)
{
    puts(__PRETTY_FUNCTION__);
}

template <typename T>
void deduce_2(T& arg)
{
    puts(__PRETTY_FUNCTION__);
}

TEST_CASE("auto is almost the same as type arg deduction for function template")
{
    auto a1 = 42;
    auto a2(42);
    auto a3 = a1;

    deduce_1(42);
    deduce_1(a1);
}

TEST_CASE("auto type deduction - case 1")
{
    std::cout << "\n--------------------\n";
    int x = 10;
    auto a1 = x; // int
    deduce_1(x);

    const int cx = 42;
    auto a2 = cx; // int
    deduce_1(cx);

    int& ref_x = x;
    auto a3 = ref_x; // int
    deduce_1(ref_x);

    const int& cref_x = x;
    auto a4 = cref_x; // int
    deduce_1(cref_x);

    int tab[10];
    auto a5 = tab; // int* - decay to pointer
    deduce_1(tab);

    auto a6 = foo; // int(*ax6)(int) - decay to pointer
    deduce_1(foo);
}

TEST_CASE("auto type deduction - case 2")
{
    std::cout << "\n--------------------\n";
    int x = 10;
    auto& a1 = x; // int&
    deduce_2(x);

    const int cx = 42;
    auto& a2 = cx; // const int&
    deduce_2(cx);

    int& ref_x = x;
    auto& a3 = ref_x; // int&
    deduce_2(ref_x);

    const int& cref_x = x;
    auto& a4 = cref_x; // const int&
    deduce_2(cref_x);

    int tab[10];
    auto& a5 = tab; // int(&)[10] - no decay to pointer -> reference to array
    deduce_2(tab);

    auto& a6 = foo; // int(&)(int) - no decay to pointer -> reference to function
    deduce_2(foo);
}

TEST_CASE("auto can be mixed with modifiers")
{
    const auto a1 = 42;

    auto* aptr1 = &a1; // const int*

    const auto aptr2 = &a1; // const int* const
}

TEST_CASE("using auto for iteration")
{
    std::cout << "\n--------------------\n";
    int data[10] = {1, 2, 3, 4};
    auto& view = data;

    for (auto it = std::begin(data); it != std::end(data); ++it)
    {
        *it *= 2;
    }

    for (auto it = std::cbegin(data); it != std::cend(data); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";
}

TEST_CASE("declaring variables with auto")
{
    SECTION("without auto")
    {
        int x1 = 10;
        int x2 = int(10);
        int x3(10);
        int x4 {10};
    }

    SECTION("with auto")
    {
        auto ax1 = 10;
        auto ax2 = int(10);
        auto ax3(10);
        auto ax4 {10}; // !!! int - since C++17
        // auto ax5{10, 11, 12}; // !!! ERROR - since C++17
        auto ax6 = {1, 2, 3, 4}; // std::initializer_list<int>
    }
}

struct Argument
{
    int value;

private:
    Argument(int v)
        : value {v}
    {
    }
};

int foo(Argument arg)
{
    return 42;
}

TEST_CASE("decltype")
{
    std::map<int, std::string> dict = {{1, "one"}, {2, "two"}};

    auto backup = dict;
    REQUIRE(backup.size() == 2);

    decltype(dict) other;
    REQUIRE(other.size() == 0);

    using EntryType = decltype(dict)::value_type;

    std::vector<decltype(dict)::mapped_type> items;

    dict[3];
    REQUIRE(dict.size() == 3);

    using RefMappedValue = decltype(dict[1]);
    REQUIRE(dict.size() == 3);

    using ReturnType = decltype(foo(std::declval<Argument>()));
}

template <typename T1, typename T2>
auto my_multiply(T1 a, T2 b)
{
    return a * b;
}

template <size_t N>
auto create_buffer()
{
    if constexpr (N <= 100)
        return std::array<int, N> {};
    else
        return std::vector<int>(N);
}

TEST_CASE("using auto in return type for function")
{
    auto result1 = my_multiply(1, 3.14);

    auto small_buffer = create_buffer<64>();
    auto large_buffer = create_buffer<1024>();
}

class Dictionary
{
    std::map<int, std::string> items = {{1, "one"}, {2, "two"}};

public:
    auto get_by_value(int key) // int get_by_value(int key)
    {
        return items[key];
    }

    auto& get_by_ref(int key) // int get_by_value(int key)
    {
        return items[key];
    }

    decltype(auto) get(int key) // int& get(int key)
    {
        return items[key];
    }
};

template <typename T>
decltype(auto) get_nth(std::vector<T>& vec, size_t nth)
{
    //decltype(auto) nth_item = vec[nth];
    //return nth_item;

    return vec[nth];
}

TEST_CASE("decltype(auto) vs. auto")
{
    Dictionary dict;

    REQUIRE(dict.get(1) == "one"s);
    dict.get(1) = "jeden"s;
    REQUIRE(dict.get(1) == "jeden"s);

    std::vector<int> vec_ints = {1, 2, 3};
    get_nth(vec_ints, 1) = 10;
    REQUIRE(vec_ints[1] == 10);

    std::vector<bool> vec_bools = { 0, 1, 0, 1 };
    get_nth(vec_bools, 1) = false;
    REQUIRE(vec_bools[1] == false);
}