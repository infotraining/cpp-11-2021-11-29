#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

void foo(int* ptr)
{
    std::cout << "foo(int*)\n";
    if (ptr)
        std::cout << *ptr << "\n";
}

void foo(nullptr_t value)
{
    std::cout << "foo(nullptr_t)\n";
}

void foo(int value)
{
    std::cout << "foo(int)\n";
}

static_assert(nullptr == NULL, "it must be true");

namespace LegacyCode
{
    void bar(int* ptr)
    {
        if (ptr == NULL)
            std::cout << "ptr is NULL\n";
    }
}

TEST_CASE("nullptr")
{
    SECTION("old-style NULL should be replaced with nullptr")
    {
        int* old_style_ptr = NULL;
        int* ptr1 = nullptr;
    }

    foo(nullptr);

    SECTION("using nullptr")
    {
        int* ptr = nullptr;

        int* ptr_b {nullptr};
        int* ptr_c {};

        if (ptr)
            std::cout << "ptr is not null\n";

        LegacyCode::bar(nullptr);
    }
}

namespace ModernCpp
{
    template <typename T>
    class UniquePtr
    {
        T* ptr_;

    public:
        explicit UniquePtr(T* ptr) noexcept
            : ptr_ {ptr}
        {
        }

        UniquePtr(nullptr_t) noexcept
            : ptr_ {nullptr}
        {
        }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr(UniquePtr&& other) : ptr_{other.ptr_}
        {
            other.ptr_ = nullptr;
        }

        ~UniquePtr()
        {
            delete ptr_;
        }
    };
}

TEST_CASE("using nullptr_t in smart ptrs")
{
    using namespace ModernCpp;

    UniquePtr<int> sp1 {new int(13)};
    UniquePtr<int> sp2 = nullptr;
}