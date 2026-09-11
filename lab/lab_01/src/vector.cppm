export module vector;

import std;

export template <typename T>
class Vector
{
private:
    std::size_t size_{0};
    T *data_;

    // 5
    void CheckIndex(std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Vector index is out of range");
        }
    }

    // 5
    void CheckSameSize(const Vector &other) const
    {
        if (size_ != other.size_)
        {
            throw std::invalid_argument("Vector sizes do not match");
        }
    }

public:
    // 3
    static constexpr double kEpsilon{1e-9}; /// 0.0000000001

    explicit Vector(std::size_t size, T fill_value) : size_(size), data_(new T[size])
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] = fill_value;
        }
    }

    // explicit Vector(std::size_t size, T min_value, T max_value);

    Vector() = delete;

    // 2
    ~Vector() { delete[] data_; }

    // 2
    Vector(const Vector &other) : size_(other.size_), data_(new T[other.size_])
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] = other.data_[i];
        }
    }

    // 2
    Vector(Vector &&other) noexcept : size_(other.size_), data_(other.data_)
    {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // 2
    Vector &operator=(const Vector &other)
    {
        if (this != &other)
        {
            return *this;
        }

        delete[] data_;
        size_ = other.size_;
        data_ = new T[size_];
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] = other.data_[i];
        }
    }

    // 2
    Vector &operator=(Vector &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }

        return *this;
    }

    // 3
    bool operator==(const Vector &other) const
    {
        if (size_ != other.size_)
        {
            return false;
        }

        for (std::size_t i{0}; i < size_; ++i)
        {
            if (std::abs(data_[i] - other.data_[i]) > kEpsilon)
            {
                return false;
            }
        }

        return true;
    }

    // 3
    bool operator!=(const Vector &other) const
    {
        return !(*this == other);
    }
};