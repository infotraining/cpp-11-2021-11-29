#include "catch.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Catch::Matchers;

class Noncopyable
{
public:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;    
protected:
    Noncopyable() = default;
};

struct MyNoncopyable : Noncopyable
{
    std::string value;

    MyNoncopyable(std::string value) : value{std::move(value)}
    {
    }
};

int calculate(int value)
{
    std::cout << "calculate(" << value << ")\n";
    return value;
}

double calculate(double) = delete;

TEST_CASE("delete")
{
    MyNoncopyable source{"text"s};
    //MyNoncopyable backup = source;

    calculate(42);
    //calculate(3.14);
}

class IGadget
{
public:
    virtual void use() const = 0;
    virtual ~IGadget() = default;
};

class Gadget : public IGadget
{
    int id_{-1};
    std::string name_ = "unknown";

public:
    Gadget() = default;

    Gadget(int id, std::string name)
        : id_ {id}
        , name_ {std::move(name)}
    {
    }

    Gadget(int id) : Gadget{id, "Gadget-"s + std::to_string(id)} // delegating contruction to another constructor
    {
        name_ += "!";
    }

    int id() const
    {
        return id_;
    }

    std::string name() const
    {
        return name_;
    }

    void use() const override
    {
        std::cout << "Using gadget: " << id() << " - " << name() << "\n";
    }
};

class SuperGadget : public Gadget
{
public:
    using Gadget::Gadget;   

    SuperGadget(int id) = delete;

    void use() const override final
    {
        std::cout << "Using super gadget: " << id() << " - " << name() << "\n";
    }
};

class HyperGadget final : public SuperGadget
{
public:
    using SuperGadget::SuperGadget;

    // void use() const override
    // {
    //     std::cout << "Using hyper gadget: " << id() << " - " << name() << "\n";
    // }
};

TEST_CASE("default constructor")
{
    Gadget g;
}

TEST_CASE("inheritance of constructors")
{
    SuperGadget sg1 {1, "super-gadget"};

    Gadget& g_ref = sg1;
    g_ref.use();
}