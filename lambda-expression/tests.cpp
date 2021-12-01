#include "catch.hpp"
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

class Lambda_237845283654
{
public:
    Lambda_237845283654() = delete;

    auto operator()(int x) const { return x * x; }
};

TEST_CASE("lambda expression")
{
    auto lambda = [](int x)
    { return x * x; };

    REQUIRE(lambda(4) == 16);

    SECTION("is interpreted as")
    {
        // auto lambda = Lambda_237845283654{};
    }

    SECTION("lambda with inconsistent return types")
    {
        auto describe = [](int x) -> std::string
        {
            if (x % 2 == 0)
                return "even";
            else
            {
                return "odd"s;
            }
        };

        REQUIRE(describe(2) == "even"s);
    }
}

class Lambda_2378452836777
{
    const int threshold_;

public:
    Lambda_2378452836777(int t) = delete;

    auto operator()(int x) const { return x > threshold_; }
};

struct Data
{
    std::vector<int> data;
    int factor;

    void filter(int threshold)
    {
        std::transform(begin(data), end(data), begin(data), [this, threshold](int x)
            { 
            if (x > threshold)
                return x * factor; 
            else    
                return x; });
    }
};

TEST_CASE("captures")
{
    int threshold = 100;

    auto is_bigger_than_threshold = [threshold](int x)
    { return x > threshold; };

    threshold = 200;

    REQUIRE(is_bigger_than_threshold(150));

    auto increase_threashold = [&threshold]()
    { threshold += 100; };

    increase_threashold();

    REQUIRE(threshold == 300);

    std::vector<int> vec = {1, 2, 3};
    int factor = 10;
    int sum {};

    std::for_each(begin(vec), end(vec), [factor, &sum](int x)
        { sum += x * factor; });
    REQUIRE(sum == 60);

    std::unique_ptr<int> uptr = std::make_unique<int>(13);

    SECTION("since C++14")
    {
        auto closure_with_unique_ptr = [uptr = std::move(uptr)]
        { if (uptr) std::cout << *uptr << "\n"; };
        closure_with_unique_ptr();
    }
}

auto create_generator(int seed)
{
    return [seed]() mutable
    { return ++seed; };
}

TEST_CASE("immutable vs. mutable")
{
    int x = 100;

    auto generator = create_generator(x);
    REQUIRE(generator() == 101);
    REQUIRE(generator() == 102);
}

class Lambda_2634526734582736
{
public:
    template <typename T1, typename T2>
    auto operator()(T1 a, T2 b) { return *a < *b; }
};

TEST_CASE("generic lambda")
{
    auto cmp_by_pointed_value = [](const auto& a, const auto& b)
    { return *a < *b; };

    int x = 10;
    int y = 20;

    REQUIRE(cmp_by_pointed_value(&x, &y) == true);

    auto p1 = std::make_unique<int>(10);
    auto p2 = std::make_unique<int>(20);

    REQUIRE(cmp_by_pointed_value(p1, p2) == true);
}

TEST_CASE("storing closures")
{
    auto best_way = [](int x)
    { std::cout << x << "\n"; };

    int (*ptr)(int) = [](int x)
    { return 2 * x; };

    int factor = 10;
    std::function<int(int)> f = [factor](int x)
    { return factor * x; };
    REQUIRE(f(10) == 100);
}

class TaskQueue
{
    using Task = std::function<void()>;
    std::queue<Task> tq_;

public:
    void submit(Task t)
    {
        tq_.push(std::move(t));
    }

    void run()
    {
        Task task;
        while (!tq_.empty())
        {
            task = tq_.front();
            tq_.pop();

            task();
        }
    }
};

template <typename F, typename TArg>
decltype(auto) call(F&& f, TArg&& arg)
{
    return f(std::forward<TArg>(arg));
}

TEST_CASE("task queue")
{
    TaskQueue tq;

    std::vector<int> vec;

    tq.submit([] { std::cout << "Start\n"; });
    tq.submit([&vec] { vec.push_back(42); });
    tq.submit([] { std::cout << "Stop\n"; });

    tq.run();
    REQUIRE(vec[0] == 42);

    auto result = call([](int x) { std::cout << x << "\n"; return x; }, 42);
    REQUIRE(result == 42);
}