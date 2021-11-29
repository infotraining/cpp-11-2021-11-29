
#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <list>

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

// C++20
// void deduce_1(auto arg)
// {
//     puts(__PRETTY_FUNCTION__);
// }

TEST_CASE("auto is almost the same as type arg deduction for function template")
{
    auto ax1 = 42;
    auto ax2(42);
    auto ax3 = ax1;

    deduce_1(42);
    deduce_1(ax1);
}

TEST_CASE("auto type deduction - case 1")
{
    std::cout << "\n--------------------\n";
    int x = 10;
    auto ax1 = x; // int
    deduce_1(x);

    const int cx = 42;
    auto ax2 = cx; // int
    deduce_1(cx);

    int& ref_x = x;
    auto ax3 = ref_x; // int
    deduce_1(ref_x);

    const int& cref_x = x;
    auto ax4 = cref_x; // int
    deduce_1(cref_x);

    int tab[10];
    auto ax5 = tab; // int* - decay to pointer
    deduce_1(tab);

    auto ax6 = foo; // int(*ax6)(int) - decay to pointer
    deduce_1(foo);
}

TEST_CASE("auto type deduction - case 2")
{
    int x = 10;
    auto& a1 = x; // int&
    deduce_2(x);

    const int cx = 42;
    auto& a2 = cx; // const int& 

    int& ref_x = x;
    auto& a3 = ref_x; // int&

    const int& cref_x = x;
    auto& a4 = cref_x; // const int&

    int tab[10];
    auto& a5 = tab; // int(&)[10] - no decay to pointer -> reference to array
    deduce_2(tab);
        
    auto& a6 = foo; // int(&)(int) - no decay to pointer -> reference to function
    deduce_2(foo);
}

TEST_CASE("auto can be mixed with  modifiers")
{
    const auto a1 = 42;

    auto* aptr1 = &a1; // const int*

    const auto aptr2 = &a1; // const int* const
}

TEST_CASE("using auto for iteration")
{
    int data[10] = {1, 2, 3, 4};
    auto& view = data;

    for(auto it = std::begin(data); it != std::end(data); ++it)
    {
        *it *= 2;
    }

    for(auto it = std::cbegin(data); it != std::cend(data); ++it)
    {
        std::cout << *it << " ";
        std::cout << "\n";
    }
}

TEST_CASE("declaring variables with auto")
{
    SECTION("without auto")
    {
        int x1 = 10;
        int x2 = int(10);
        int x3(10);
        int x4{10};
    }

    SECTION("with auto")
    {
        auto ax1 = 10;
        auto ax2 = int(10);
        auto ax3(10);
        auto ax4{10}; // !!! int - since C++17
        //auto ax5{10, 11, 12}; // !!! ERROR - since C++17
        auto ax6 = {1, 2, 3, 4}; // std::initializer_list<int>
    }

}