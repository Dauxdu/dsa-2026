import std;
import vector;
import triangle;

// 10
template <typename T>
void DemoOperations(const T &min_value, const T &max_value)
{
    const std::size_t size{3};
    const T scalar{2};

    Vector<T> a(size, min_value, max_value);
    const Vector<T> b(size, min_value, max_value);
    const Vector<T> filled(size, scalar);
    std::cout << "a = " << a << "\nb = " << b << "\nfilled = " << filled << '\n';
    std::cout << "a.Size() = " << a.Size() << '\n';

    std::cout << "a + b = " << a + b << '\n';
    std::cout << "a - b = " << a - b << '\n';
    std::cout << "a * b = " << a * b << '\n';
    std::cout << "a * 2 = " << a * scalar << '\n';
    std::cout << "2 * a = " << scalar * a << '\n';
    std::cout << "a / 2 = " << a / scalar << '\n';
    std::cout << "|a| = " << a.Norm() << '\n';

    Vector<T> c{a};
    c += b;
    std::cout << std::boolalpha << "(c = a, c += b) == a + b: " << (c == a + b) << '\n';
    c -= b;
    std::cout << "(c -= b) == a + b - b: " << (c == a + b - b) << '\n';
    std::cout << "a - a == zero: " << (a - a == Vector<T>(size, T{})) << '\n';
    c = a;
    std::cout << "(c = a) == a: " << (c == a) << '\n';
    c *= scalar;
    std::cout << "a * 2 != a: " << (c != a) << '\n';
    c /= scalar;
    std::cout << "(a * 2) / 2 == a: " << (c == a) << '\n';

    a[0] = max_value;
    a.At(1) = min_value;
    const Vector<T> &const_a{a};
    std::cout << "after a[0] = " << max_value << ", a.At(1) = " << min_value << ": a = " << a
              << ", a[0] = " << const_a[0] << ", a.At(1) = " << const_a.At(1) << '\n';

    Vector<T> moved{std::move(c)};
    Vector<T> assigned(1, T{});
    assigned = a;
    std::cout << "moved = " << moved << ", assigned = " << assigned << '\n';
    assigned = std::move(moved);
    std::cout << "after move assignment: assigned = " << assigned << '\n';
}

// 5
template <typename T>
void DemoExceptions(const T &min_value, const T &max_value)
{
    const std::size_t size{3};
    const Vector<T> a(size, min_value, max_value);

    try
    {
        std::cout << a.At(size) << '\n';
    }
    catch (const std::out_of_range &error)
    {
        std::cout << "a.At(" << size << "): " << error.what() << '\n';
    }

    try
    {
        std::cout << a + Vector<T>(size + 1, T{}) << '\n';
    }
    catch (const std::invalid_argument &error)
    {
        std::cout << "a + vector of other size: " << error.what() << '\n';
    }

    try
    {
        std::cout << a / T{} << '\n';
    }
    catch (const std::invalid_argument &error)
    {
        std::cout << "a / 0: " << error.what() << '\n';
    }

    try
    {
        std::cout << Vector<T>(size, max_value, min_value) << '\n';
    }
    catch (const std::invalid_argument &error)
    {
        std::cout << "random vector with min > max: " << error.what() << '\n';
    }
}

template <typename T>
void Demo(const std::string &type_name, const T &min_value, const T &max_value)
{
    std::cout << "===== Vector<" << type_name << "> =====\n";
    DemoOperations(min_value, max_value);
    DemoExceptions(min_value, max_value);
    std::cout << '\n';
}

// 9
void SolveTriangleTask()
{
    std::cout << "===== Task: triangle area =====\n";

    Vector<int> example_a(2, 0);
    Vector<int> example_b(2, 0);
    example_a[0] = 3;
    example_b[1] = 4;
    std::cout << "a = " << example_a << ", b = " << example_b << ", S = " << TriangleArea(example_a, example_b) << "\n";

    const Vector<double> random_a(3, -10.0, 10.0);
    const Vector<double> random_b(3, -10.0, 10.0);
    std::cout << "a = " << random_a << ", b = " << random_b << ", S = " << TriangleArea(random_a, random_b) << '\n';
}

int main()
{
    try
    {
        Demo<int>("int", -10, 10);
        Demo<float>("float", -10.0F, 10.0F);
        Demo<double>("double", -10.0, 10.0);
        Demo<std::complex<float>>("std::complex<float>", {-10.0F, -10.0F}, {10.0F, 10.0F});
        Demo<std::complex<double>>("std::complex<double>", {-10.0, -10.0}, {10.0, 10.0});
        SolveTriangleTask();
    }
    catch (const std::exception &error)
    {
        std::cout << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}