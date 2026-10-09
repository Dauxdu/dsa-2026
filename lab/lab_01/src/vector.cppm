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

// 3
template <typename T>
struct RealType
{
    using Type = T;
};

template <typename T>
struct RealType<std::complex<T>>
{
    using Type = T;
};

// 11
export template <typename T>
class Vector final
{
private:
    // 2
    T *data_{nullptr};
    std::size_t size_{0};

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
        if (std::abs(scalar) <= kEpsilon)
        {
            throw std::invalid_argument("Division by zero");
        }
    }

public:
    using RealType = typename RealType<T>::Type;

    // 3
    static constexpr RealType kEpsilon{std::numeric_limits<RealType>::epsilon()};

    Vector(std::size_t size, const T &fill_value) : data_{new T[size]}, size_{size}
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] = fill_value;
        }
    }

    // 7
    Vector(std::size_t size, const T &min_value, const T &max_value) : data_{new T[size]}, size_{size}
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
            data_ = nullptr;
            size_ = 0;
            throw;
        }
    }

    // 2
    Vector(const Vector &other) : data_{new T[other.size_]}, size_{other.size_}
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            data_[i] = other.data_[i];
        }
    }

    // 2
    Vector(Vector &&other) noexcept : data_{other.data_}, size_{other.size_}
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 2
    Vector &operator=(const Vector &other)
    {
        if (this != &other)
        {
            Vector temp(other);
            *this = std::move(temp);
        }

        return *this;
    }

    // 2
    Vector &operator=(Vector &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }

        return *this;
    }

    // 2
    ~Vector() { delete[] data_; }

    std::size_t Size() const noexcept { return size_; }

    // 8
    auto Norm() const
    {
        return std::sqrt(std::abs(*this * *this));
    }

    // 5
    T &operator[](std::size_t index) noexcept { return data_[index]; }

    const T &operator[](std::size_t index) const noexcept { return data_[index]; }

    // 5
    T &At(std::size_t index)
    {
        CheckIndex(index);
        return data_[index];
    }

    const T &At(std::size_t index) const
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
    friend Vector operator+(Vector lhs, const Vector &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Vector operator-(Vector lhs, const Vector &rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend Vector operator*(Vector lhs, const T &scalar)
    {
        lhs *= scalar;
        return lhs;
    }

    friend Vector operator*(const T &scalar, Vector rhs)
    {
        rhs *= scalar;
        return rhs;
    }

    friend Vector operator/(Vector lhs, const T &scalar)
    {
        lhs /= scalar;
        return lhs;
    }

    // Скалярное произведение
    friend T operator*(const Vector &lhs, const Vector &rhs)
    {
        lhs.CheckSameSize(rhs);
        T result{};
        for (std::size_t i{0}; i < lhs.size_; ++i)
        {
            result += lhs.data_[i] * Conjugate(rhs.data_[i]);
        }
        return result;
    }

    // 3
    friend bool operator==(const Vector &lhs, const Vector &rhs)
    {
        if (lhs.size_ != rhs.size_)
        {
            return false;
        }

        for (std::size_t i{0}; i < lhs.size_; ++i)
        {
            const auto difference{std::abs(lhs.data_[i] - rhs.data_[i])};
            const auto scale{std::max(std::abs(lhs.data_[i]), std::abs(rhs.data_[i]))};
            if (difference > kEpsilon * scale)
            {
                return false;
            }
        }
        return true;
    }

    // 3
    friend bool operator!=(const Vector &lhs, const Vector &rhs) { return !(lhs == rhs); }

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
