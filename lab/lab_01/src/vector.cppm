export module vector;

import std;

template <typename T>
T Conjugate(const T &value)
{
    return value;
}

template <typename T>
std::complex<T> Conjugate(const std::complex<T> &value)
{
    return std::conj(value);
}

// 7
int RandomValue(std::mt19937 &engine, int min_value, int max_value)
{
    if (min_value > max_value)
    {
        throw std::invalid_argument("Lower bound is greater than upper bound");
    }
    std::uniform_int_distribution<int> distribution(min_value, max_value);
    return distribution(engine);
}

template <typename T>
T RandomValue(std::mt19937 &engine, T min_value, T max_value)
{
    if (min_value > max_value)
    {
        throw std::invalid_argument("Lower bound is greater than upper bound");
    }
    std::uniform_real_distribution<T> distribution(min_value, max_value);
    return distribution(engine);
}

template <typename T>
std::complex<T> RandomValue(std::mt19937 &engine, const std::complex<T> &min_value,
                            const std::complex<T> &max_value)
{
    return {RandomValue(engine, min_value.real(), max_value.real()),
            RandomValue(engine, min_value.imag(), max_value.imag())};
}

// 11
export template <typename T>
class Vector
{
private:
    // 2
    std::size_t size_{0};
    T *data_{nullptr};

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

    // 5
    void CheckDivisor(const T &scalar) const
    {
        if (scalar == T{0})
        {
            throw std::invalid_argument("Division by zero");
        }
    }

public:
    // 3
    static constexpr double kEpsilon{0.00001};

    Vector(std::size_t size, const T &fill_value) : size_{size}, data_{new T[size]}
    {
        std::fill_n(data_, size_, fill_value);
    }

    // 7
    Vector(std::size_t size, const T &min_value, const T &max_value) : size_{size}, data_{new T[size]}
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] = fill_value;
        }
    }

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
    Vector(Vector &&other) noexcept : size_{other.size_}, data_{other.data_}
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