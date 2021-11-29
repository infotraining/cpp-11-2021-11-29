#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

TEST_CASE("string literals")
{
    std::string str1 = "c:\nasz katalog\backup\n";
    std::cout << str1;

    std::string str2 = R"(c:\nasz katalog\backup\n)";
    std::cout << str2 << "\n";

    std::string lines = R"(line1
line2
line3
line4)";

    std::cout << lines << "\n";

    std::string custom_delimiters = R"kraw(cytat: "(test)" )kraw";
    std::cout << custom_delimiters << "\n";
}

TEST_CASE("std::string literal")
{
    using namespace std::literals;

    auto text1 = "text";  // const char*

    auto text2 = "text"s; // std::string

    auto text3 = "text"sv; // std::string_view
}

TEST_CASE("std::chrono")
{
    using namespace std::literals;

    auto time_span1 = 1s; 

    auto time_span2 = 150ms; 
}

enum class Currency
{
    usd, eur, pln
};

struct Money
{
    long double value;
    Currency currency;
};

Money operator""_USD(long double value)
{
    return Money{value, Currency::usd};
}

Money operator""_EUR(long double value)
{
    return Money{value, Currency::eur};
}


TEST_CASE("user defined literal")
{
    auto price1 = 100.99_USD;
    auto price2 = 100.99_EUR;

    REQUIRE(price.currency == Currency::usd);
    REQUIRE(price.value == Approx(100.99));
}