#include "catch.hpp"
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& ref_name = name;

        const std::string& ref_full_name = full_name(name, "kowalski");

        //ref_full_name[0] = 'p';
    }

    SECTION("C++11")
    {
        std::string&& ref_full_name = full_name(name, "kowalski");

        ref_full_name.reserve(1024);
        ref_full_name[0] = 'p';

        REQUIRE(ref_full_name == "pan kowalski"s);

        //std::string&& ref_name = name; // ERROR - illegal in C++11
    }
}

namespace Explain
{
    template <typename T>
    class Vector
    {
        std::vector<T> items_;
    public:
        using reference = T&;

        void push_back(const T& value)
        {
            std::cout << "push_back copies " << value << " into container\n";
        }

        void push_back(T&& value)
        {
            std::cout << "push_back moves " << value << " into container\n";
        }

        reference operator[](size_t index)
        {
            return items_[index];
        }

        // template <typename TT>
        // void push_back(TT&& value)
        // {            
        // }
    };

    template <>
    class Vector<bool>
    {        
    public:
        struct reference
        {
            uint8_t* byte_field;
            uint8_t index;
        };

        void push_back(bool value)
        {
            std::cout << "push_back copies " << value << " into container\n";
        }

        reference operator[](size_t index)
        {
            return reference{};
        }
    };

}

TEST_CASE("using vector")
{
    Explain::Vector<std::string> vec;

    std::string str = "text";

    vec.push_back(str);
    vec.push_back(str + str);
    vec.push_back("another text");
    vec.push_back(std::move(str));
}
