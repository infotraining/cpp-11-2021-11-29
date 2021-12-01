#include "catch.hpp"
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(std::string name, std::initializer_list<int> list)
        : name_ {std::move(name)}
        , data_ {new int[list.size()]}
        , size_ {list.size()}
    {
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    Data(const Data& other)
        : name_(other.name_)
        , data_ {new int[other.size_]}
        , size_(other.size_)
    {
        std::copy(other.begin(), other.end(), data_);

        std::cout << "Data(" << name_ << ": cc)\n";
    }

    Data& operator=(const Data& other)
    {
        Data temp(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    /////////////////////////////////////////////////
    // move constructor
    Data(Data&& other) noexcept
        : name_ {std::move(other.name_)}
        , data_ {other.data_}
        , size_ {other.size_}
    {
        other.data_ = nullptr;
        other.size_ = 0;

        std::cout << "Data(" << name_ << ": mv)\n";
    }

    /////////////////////////////////////////////////
    // move assignment
    Data& operator=(Data&& other)
    {
        if (this != &other)
        {
            Data temp(std::move(other));
            swap(temp);
        }

        std::cout << "Data=(" << name_ << ": mv)\n";

        return *this;
    }

    ~Data() noexcept
    {
        delete[] data_;
    }

    void swap(Data& other)
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin() noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }
};

namespace ModernCpp
{
    class Data
    {
        std::string name_;
        std::vector<int> data_;

    public:
        using iterator = std::vector<int>::iterator;
        using const_iterator = std::vector<int>::const_iterator;

        Data(std::string name, std::initializer_list<int> list)
            : name_ {std::move(name)}
            , data_ {list}
        {
            std::cout << "Data(" << name_ << ")\n";
        }

        void swap(Data& other)
        {
            name_.swap(other.name_);
            data_.swap(other.data_);
        }

        iterator begin()
        {
            return data_.begin();
        }

        iterator end()
        {
            return data_.end();
        }

        const_iterator begin() const
        {
            return data_.begin();
        }

        const_iterator end() const
        {
            return data_.end();
        }
    };

}

Data create_data_set()
{
    Data ds {"data-set-one", {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

template <typename TContainer>
void print(const std::string& prefix, const TContainer& container)
{
    std::cout << prefix << " - [ ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("3---")
{
    std::cout << "\n--------------------------\n\n";
}

Data create_large_dataset()
{
    Data ds {"ds-large", {1, 2, 53, 45, 645, 75647, 7561, 2, 3, 4, 5}};

    return ds;
}

TEST_CASE("Data & move semantics")
{
    std::string ds_name = "ds1";
    Data ds1 {std::move(ds_name), {1, 2, 3, 4, 5}};

    Data backup = ds1; // copy
    print("backup", backup);

    Data target = std::move(ds1); // copy ds1 to target
    print("target", target);

    Data ds2 = create_large_dataset();
    print("ds2", ds2);
}

namespace RuleOfFive
{
    struct DataRows
    {
        Data row1;
        Data row2;

        ~DataRows() { std::cout << "Object has been destroyed!\n"; } // if we need destructor we need also five other special functions
        DataRows(const DataRows&) = default;
        DataRows(DataRows&&) = default;
        DataRows& operator=(const DataRows&) = default;
        DataRows& operator=(DataRows&&) = default;
    };
}

namespace RuleOfZero
{
    struct DataRows
    {
        Data row1;
        Data row2;
    };
}

TEST_CASE("DataRows")
{
    using namespace RuleOfZero;

    std::cout << "\n-------------\n";
    DataRows rows {Data {"row1", {1, 2, 3}}, Data {"row2", {6, 7, 8}}};
    DataRows target_rows {std::move(rows)};
}

int foo(int x) noexcept
{
    std::vector<int> vec;
    return vec.at(10); // will cause std::terminate()
}

TEST_CASE("noexcept")
{
    std::cout << "\n--------------------noexcept\n";

    std::vector<Data> vec;

    vec.push_back(Data{"ds1", {1, 2, 3}});
    std::cout << "---\n";
    vec.push_back(Data{"ds2", {1, 2, 3}});
    std::cout << "---\n";
    vec.push_back(Data{"ds3", {1, 2, 3}});
    std::cout << "---\n";
    vec.push_back(Data{"ds4", {1, 2, 3}});
    std::cout << "---\n";
}