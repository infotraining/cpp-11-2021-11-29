#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// forward declaration
enum Currency : uint16_t;

void calculate_rates(Currency currency);

// definition
enum Currency : uint16_t
{
    usd,
    eur,
    pln,
    cad
};

TEST_CASE("enums")
{
    Currency c1 = usd;
    c1 = eur;

    std::underlying_type_t<Currency> index = c1;

    std::cout << sizeof(c1) << "\n";
}

enum class Country : uint8_t
{
    pl = 1, us, fr, gb, it
};

std::vector<std::string> country_names = { "Poland", "United States" };

const std::string& get_country(const std::vector<std::string>& dict, Country country)
{
    return dict.at(static_cast<std::underlying_type_t<Country>>(country));
}

TEST_CASE("enum class")
{
    static_assert(sizeof(Country) == 1, "Error");

    Country c1 = Country::pl;

    auto index = static_cast<std::underlying_type_t<Country>>(c1);
    REQUIRE(index == 1);

    c1 = static_cast<Country>(3);
    REQUIRE(c1 == Country::fr);
}

TEST_CASE("enum class in C++17")
{
    //Country country = 4;
    Country country{4}; // since C++17 - it is OK

    std::byte b1{42};
    std::byte b2{0b0000'1111};

    std::byte r1 = b1 & b2;
    //std::byte r2 = b1 + b2;
    std::cout << std::to_integer<int>(r1) << "\n";
}