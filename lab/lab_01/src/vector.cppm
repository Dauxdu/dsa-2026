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
        try
        {
            for (std::size_t i{0}; i < size_; ++i)
            {
                data_[i] = fill_value;
            }
        }
        catch (...)
        {
            delete[] data_;
            throw;
        }
    }

    // 7
    Vector(std::size_t size, const T &min_value, const T &max_value) : size_{size}, data_{new T[size]}
    {
        try
        {
            std::mt19937 engine{std::random_device{}()};
            for (std::size_t i{0}; i < size_; ++i)
            {
                data_[i] = RandomValue(engine, min_value, max_value);
            }
        }
        catch (...)
        {
            delete[] data_;
            throw;
        }
    }

    // 2
    Vector(const Vector &other) : size_{other.size_}, data_{new T[other.size_]}
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
            T *new_data{new T[other.size_]};

            for (std::size_t i{0}; i < other.size_; ++i)
            {
                new_data[i] = other.data_[i];
            }

            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
        }

        return *this;
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

    // 2
    ~Vector() { delete[] data_; }

    std::size_t Size() const { return size_; }

    // 5
    T &operator[](std::size_t index)
    {
        CheckIndex(index);
        return data_[index];
    }

    // 5
    const T &operator[](std::size_t index) const
    {
        CheckIndex(index);
        return data_[index];
    }

    // 8
    Vector &operator+=(const Vector &other)
    {
        CheckSameSize(other);
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] += other.data_[i];
        }
        return *this;
    }

    Vector &operator-=(const Vector &other)
    {
        CheckSameSize(other);
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] -= other.data_[i];
        }
        return *this;
    }

    Vector &operator*=(const T &scalar)
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] *= scalar;
        }
        return *this;
    }

    Vector &operator/=(const T &scalar)
    {
        CheckDivisor(scalar);
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] /= scalar;
        }
        return *this;
    }

    // 8
    Vector operator+(const Vector &other) const
    {
        Vector result{*this};
        result += other;
        return result;
    }

    Vector operator-(const Vector &other) const
    {
        Vector result{*this};
        result -= other;
        return result;
    }

    Vector operator*(const T &scalar) const
    {
        Vector result{*this};
        result *= scalar;
        return result;
    }

    Vector operator/(const T &scalar) const
    {
        Vector result{*this};
        result /= scalar;
        return result;
    }

    // Скалярное произведение
    T operator*(const Vector &other) const
    {
        CheckSameSize(other);
        T result{};
        for (std::size_t i{0}; i < size_; ++i)
        {
            result += data_[i] * Conjugate(other.data_[i]);
        }
        return result;
    }

    // 8
    auto Norm() const
    {
        return std::sqrt(std::abs(*this * *this));
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
    bool operator!=(const Vector &other) const { return !(*this == other); }

    friend Vector operator*(const T &scalar, const Vector &vector) { return vector * scalar; }

    // 4 и 6
    friend std::ostream &operator<<(std::ostream &stream, const Vector &vector)
    {
        stream << '(';
        for (std::size_t i{0}; i < vector.size_; ++i)
        {
            if (i != 0)
            {
                stream << ", ";
            }
            stream << vector.data_[i];
        }
        return stream << ')';
    }
};
