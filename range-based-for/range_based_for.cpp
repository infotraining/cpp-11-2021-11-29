#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

TEST_CASE("range based for")
{
    std::vector<int> vec = {1, 2, 3};

    for(const auto& item : vec)
    {   
        std::cout << item << " ";
    }
    std::cout << "\n";

    SECTION("is interpreted as")
    {
        auto&& container = vec;
        auto first = container.begin();
        auto last = container.end();
        for(; first != last; ++first)
        {
            const auto& item = *first;
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
}

TEST_CASE("most generic rased based for")
{
    std::vector<bool> vec = {1, 1, 1, 1};

    for(auto&& item : vec)
    {
        item = 0;
    }

    SECTION("is interpreted as")
    {
        auto&& container = vec;
        auto first = container.begin();
        auto last = container.end();
        for(; first != last; ++first)
        {
            auto&& item = *first;
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    REQUIRE(vec[1] == 0);
}

TEST_CASE("use cases")
{
    SECTION("arrays")
    {
        int tab[] = {1, 2, 3};

        for(auto& item : tab)
            item += 2;

        SECTION("is interpreted as")
        {
            auto&& container = tab;
            auto first = begin(tab);
            auto last = end(tab);
            for(; first != last; ++first)
            {
                auto& item = *first;
                item += 2;
            }
            std::cout << "\n";
        }
    }

    SECTION("initializer")
    {
        auto lst = {1, 2, 3, 4};

        for(const auto& item : lst)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";

        for(const auto& item : {1, 2, 3})
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
}

/////////////////////////////////////////////////
// dangling references in range-based-for loop!!!

class User
{
    std::string name_;

public:
    User(const std::string& name) : name_(name)
    {
    }

    const std::string& name() const
    {
        return name_;
    }
};

TEST_CASE("beware of dangling references in range-based-loop")
{    
    for(const auto& c : User{"Jan Kowalski"}.name())
    {
        std::cout << c << " ";
    }
    std::cout << "\n";

    SECTION("is interpreted as")
    {
        auto&& container = User{"Jan Kowalski"}.name(); // dangling reference to destroyed temp object
        auto first = begin(container);
        auto last = end(container);
        for(; first != last; ++first)
        {
            const auto& c = *first;
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
}