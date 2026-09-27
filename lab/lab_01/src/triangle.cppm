export module triangle;

import std;
import vector;

// 9
// S = 1/2 * sqrt(|a|^2 * |b|^2 - |(a, b)|^2)
export template <typename T>
auto TriangleArea(const Vector<T> &side_a, const Vector<T> &side_b)
{
    const auto norm_a{side_a.Norm()};
    const auto norm_b{side_b.Norm()};
    const auto dot{std::abs(side_a * side_b)};
    auto radicand{norm_a * norm_a * norm_b * norm_b - dot * dot};

    if (radicand < 0)
    {
        radicand = 0;
    }

    return std::sqrt(radicand) / 2;
}
