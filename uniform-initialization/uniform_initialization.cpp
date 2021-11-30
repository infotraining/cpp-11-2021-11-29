#include "catch.hpp"

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Point
{
    int x, y;

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }
};

std::ostream& operator<<(ostream& out, const Point& pt)
{
    out << "[" << pt.x << ", " << pt.y << "]";
    return out;
}

class Vector2D
{
    const uint32_t x_, y_;

public:
    Vector2D(uint32_t x = 0, uint32_t y = 0)
        : x_ {x}
        , y_ {y}
    {
    }

    uint32_t x() const
    {
        return x_;
    }

    uint32_t y() const
    {
        return y_;
    }

    static Vector2D versor_x()
    {
        return {1, 0};
    }

    static Vector2D versor_y()
    {
        return Vector2D {0, 1};
    }
};

std::ostream& operator<<(ostream& out, const Vector2D& vec)
{
    out << "(" << vec.x() << ", " << vec.y() << ")";
    return out;
}

/////////////////////////////////////////////////////////////////
// comparison of initialization in C++ standards

TEST_CASE("initialization in C++98")
{
    SECTION("simple types")
    {
        int x1 = 10;
        int x2(10);
        int x3 = int(10);

        int x4 = 0;
        int x5 = int();
        // int x6(); // problem - most vexing parse
    }

    SECTION("aggregates")
    {
        Point pt1 {2, 4};
        Point pt2 = Point {2, 4};
        Point pt3 = {2, 4};

        int tab1[4] {1, 2, 3, 4};
        int tab2[4] = {1, 2, 3, 4};
        int tab3[4] = {1, 2};
        int tab4[4] = {};
    }

    SECTION("classes/structs with constructors")
    {
        Vector2D vec1(1, 2);
        Vector2D vec2 = Vector2D(1, 2);
        // Vector2D vec3 = (1, 2); // warning - expression result unused
    }

    SECTION("containers")
    {
        std::vector<int> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);

        REQUIRE(vec.size() == 3);
    }
}

TEST_CASE("initialization in C++11")
{
    SECTION("simple types")
    {
        int x1 = 10;
        int x2 {10};
        int x3 = int {10};

        int x4 = 0;
        int x5 = int {};
        int x6 {};

        int* ptra {};
        int* ptrb = nullptr;
        REQUIRE(ptra == ptrb);
    }

    SECTION("aggregates")
    {
        Point pt1 {2, 4};
        Point pt2 = Point {2, 4};
        Point pt3 = {2, 4};

        int tab1[4] {1, 2, 3, 4};
        int tab2[4] = {1, 2, 3, 4};
        int tab3[4] = {1, 2};
        int tab4[4] = {};
    }

    SECTION("classes/structs with constructors")
    {
        Vector2D vec1 {1, 2};
        Vector2D vec2 = Vector2D {1, 2};
        Vector2D vec3 = {1, 2};
    }

    SECTION("containers")
    {
        const std::vector<int> vec = {1, 2, 3};

        REQUIRE(vec.size() == 3);
    }
}

TEST_CASE("controlling narrowing conversion")
{
    int64_t x = 1000;

    int64_t y {x};
}

void print_items(std::initializer_list<int> lst)
{
    std::cout << "List of " << lst.size() << " items: ";
    for (const auto& item : lst)
        std::cout << item << " ";
    std::cout << "\n";
}

template <typename T>
void print_container(T lst)
{
    std::cout << "List of " << lst.size() << " items: ";
    for (const auto& item : lst)
        std::cout << item << " ";
    std::cout << "\n";
}

TEST_CASE("initializer list")
{
    print_items({1, 2, 3, 4});

    auto lst = {1, 2, 3};
    print_items(lst);

    print_container(std::initializer_list<int> {1, 2, 3});
}

TEST_CASE("maps & initializer_lists")
{
    std::map<int, std::string> dict = {{1, "one"}, {2, "two"}};
}

// TODO
class Polygon
{
private:
    vector<Point> points;

public:
    Polygon(std::vector<Point> lst)
        : points {std::move(lst)}
    {
        std::cout << "Polygon(std::vector<Point> lst)\n";
    }

    Polygon(std::initializer_list<Point> lst)
        : points {lst}
    {
        std::cout << "Polygon(std::initializer_list<Point> lst)\n";
    }
    Point& operator[](size_t i)
    {
        return points[i];
    }

    const Point& operator[](size_t i) const
    {
        return points[i];
    }

    auto begin()
    {
        return points.begin();
    }

    auto end()
    {
        return points.end();
    }

    auto begin() const
    {
        return points.cbegin();
    }

    auto end() const
    {
        return points.cend();
    }
};

TEST_CASE("Polygon")
{
    Polygon pgn = {{1, 2}, {2, 4}, Point {5, 6}};

    REQUIRE(pgn[0] == Point {1, 2});

    std::cout << "Polygon: ";
    for (const auto& pt : pgn)
        std::cout << pt << " ";
    std::cout << "\n";
}

TEST_CASE("beware of {} constructors")
{
    std::vector<int> vec1(5, 1);
    REQUIRE(vec1.size() == 5);
    REQUIRE(vec1 == std::vector<int> {1, 1, 1, 1, 1});

    std::vector<int> vec2 {10, 1};
    REQUIRE(vec2 == std::vector<int> {10, 1});
}