#include "catch.hpp"
#include "gadget.hpp"
#include <memory>
#include <string>

template <typename T>
class UniquePtr
{
    T* ptr_;

public:
    UniquePtr(nullptr_t) noexcept
        : ptr_ {nullptr}
    {
    }

    UniquePtr() noexcept
        : ptr_ {nullptr}
    {
    }

    explicit UniquePtr(T* ptr) noexcept
        : ptr_ {ptr}
    {
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move constructor
    UniquePtr(UniquePtr&& other) noexcept
        : ptr_ {other.ptr_}
    {
        other.ptr_ = nullptr;
    }

    // move assignment
    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other)
        {
            delete this->ptr_;

            this->ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr() noexcept
    {
        delete ptr_;
    }

    explicit operator bool() const
    {
        return ptr_ != nullptr;
    }

    T* get() const
    {
        return ptr_;
    }

    T* operator->() const
    {
        return ptr_;
    }

    T& operator*() const
    {
        return ptr_;
    }
};

template <typename T, typename TArg1>
UniquePtr<T> MakeUnique(TArg1&& arg1)
{
    return UniquePtr<T>(new T(std::forward<TArg1>(arg1));
}

template <typename T, typename TArg1, typename TArg2>
UniquePtr<T> MakeUnique(TArg1&& arg1, TArg2&& arg2)
{
    return UniquePtr<T>(new T(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2)));
}

TEST_CASE("2---")
{
    std::cout << "\n--------------------------\n\n";
}

UniquePtr<Gadget> create_gadget()
{
    static int gen_id {};

    const int id = ++gen_id;
    return UniquePtr<Gadget> {new Gadget {id, "Gadget-" + std::to_string(id)}};
}

void use(UniquePtr<Gadget> g)
{
    if (g)
        g->use();
}

TEST_CASE("move semantics - UniquePtr")
{
    UniquePtr<Gadget> pg1 = MakeUnique<Gadget>(1, "ipad");
    pg1->use();

    UniquePtr<Gadget> pg2 = std::move(pg1);
    pg2->use();

    UniquePtr<Gadget> pg3 = create_gadget();
    pg3->use();

    std::vector<UniquePtr<Gadget>> gadgets;

    gadgets.push_back(std::move(pg3));
    gadgets.push_back(create_gadget());
    gadgets.push_back(create_gadget());

    auto my_gadget = std::move(gadgets[1]);
    gadgets.erase(gadgets.begin() + 1);

    for (const auto& g : gadgets)
    {
        if (g)
            g->use();
    }

    UniquePtr<Gadget> pg4 = create_gadget();
    use(std::move(pg4));

    pg4 = create_gadget();
    pg4->use();
}

TEST_CASE("Do not use after move")
{
    std::vector<int> vec = {1, 2, 3, 4};
    std::vector<int> target = std::move(vec);

    // REQUIRE(vec.size() == 0); // use after move == UB
    // REQUIRE(vec.capacity() == 0); // use after move == UB
    REQUIRE(target == std::vector<int> {1, 2, 3, 4});
}