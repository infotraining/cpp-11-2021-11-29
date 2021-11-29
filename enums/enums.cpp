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

template <typename EnumClass>
struct UseEnumAsIndex : std::false_type
{};

template <>
struct UseEnumAsIndex<Country> : std::true_type
{};

template <typename EnumClass, typename IndexType = size_t, typename = std::enable_if_t<UseEnumAsIndex<Country>::value>>
constexpr IndexType as_index(EnumClass enum_value)
{
    static_assert(sizeof(IndexType) >= sizeof(std::underlying_type_t<EnumClass>), 
        "Size of IndexType must be at least equal to size of underlying type in enum");

    return static_cast<IndexType>(enum_value);
}

TEST_CASE("enum class")
{
    static_assert(sizeof(Country) == 1, "Error");

    Country c1 = Country::pl;

    auto index = static_cast<std::underlying_type_t<Country>>(c1);
    REQUIRE(index == 1);

    c1 = static_cast<Country>(3);
    REQUIRE(c1 == Country::fr);

    std::vector<std::string> country_names = { "Poland", "United States" };

    REQUIRE(country_names[as_index(Country::pl)] == "Poland"s);

    static_assert(as_index(Country::pl) == 1, "Error");
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