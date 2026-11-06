export module sorted_merge;

import std;
import linked_list;

export [[nodiscard]] LinkedList<int> MakeSortedList(std::size_t size, int min_value, int max_value, unsigned int seed)
{
    LinkedList<int> steps(size, min_value, max_value, seed);
    LinkedList<int> result;
    int value{0};
    while (!steps.Empty())
    {
        value += steps.PopHead();
        result.PushTail(value);
    }
    return result;
}

export template <typename T>
[[nodiscard]] LinkedList<T> MergeUnique(LinkedList<T> lhs, LinkedList<T> rhs)
{
    LinkedList<T> result;
    while (!lhs.Empty() || !rhs.Empty())
    {
        const bool take_lhs{rhs.Empty() || (!lhs.Empty() && lhs[0] <= rhs[0])};
        const T value{take_lhs ? lhs.PopHead() : rhs.PopHead()};
        if (result.Empty())
        {
            result.PushTail(value);
            continue;
        }
        const T &last{result[result.Size() - 1]};
        if (value < last)
        {
            throw std::invalid_argument("Lists must be sorted");
        }
        if (last < value)
        {
            result.PushTail(value);
        }
    }
    return result;
}
