import std;
import linked_list;
import sorted_merge;

constexpr unsigned int SEED{608};

template <typename T>
void DemoOperations(const T &min_value, const T &max_value)
{
    const std::size_t size{5};
    LinkedList<T> a(size, min_value, max_value, SEED);
    const LinkedList<T> same_seed(size, min_value, max_value, SEED);
    const LinkedList<T> b(size, min_value, max_value, SEED + 1);
    std::cout << "a = " << a << "\nsame seed = " << same_seed << "\nb = " << b << '\n';
    std::cout << "a.Size() = " << a.Size() << ", LinkedList<T>().Empty() = " << std::boolalpha
              << LinkedList<T>().Empty() << '\n';
    LinkedList<T> c(a);
    c.PushTail(max_value);
    c.PushHead(min_value);
    std::cout << "c = a, PushTail(" << max_value << "), PushHead(" << min_value << "): c = " << c
              << ", a = " << a << '\n';
    c.PushTail(b);
    std::cout << "c.PushTail(b): " << c << '\n';
    c.PushHead(b);
    std::cout << "c.PushHead(b): " << c << '\n';
    c.PushTail(c);
    std::cout << "c.PushTail(c): size = " << c.Size() << '\n';
    const T head{c.PopHead()};
    const T tail{c.PopTail()};
    std::cout << "c.PopHead() = " << head << ", c.PopTail() = " << tail << ", size = " << c.Size() << '\n';
    LinkedList<T> d(b);
    d.PushTail(b[0]);
    d.PushHead(b[0]);
    std::cout << "d = " << d << ", d.DeleteNode(" << b[0] << ") removed " << d.DeleteNode(b[0])
              << ": d = " << d << '\n';
    a[0] = max_value;
    a[a.Size() + 1] = min_value;
    const LinkedList<T> &const_a{a};
    std::cout << "after a[0] = " << max_value << ", a[size + 1] = " << min_value << ": a = " << a
              << ", a[size] = " << const_a[size] << ", a[3 * size - 1] = " << const_a[3 * size - 1] << '\n';
    LinkedList<T> moved(std::move(d));
    LinkedList<T> assigned;
    assigned = a;
    std::cout << "moved = " << moved << ", assigned = " << assigned << '\n';
    assigned = std::move(moved);
    std::cout << "after move assignment: assigned = " << assigned << '\n';
    assigned.Clear();
    std::cout << "after Clear: assigned = " << assigned << '\n';
}

template <typename Name, typename Action>
void TryRun(const Name &what, Action &&action)
{
    try
    {
        action();
    }
    catch (const std::exception &error)
    {
        std::cout << what << ": " << error.what() << '\n';
    }
}

template <typename T>
void DemoExceptions(const T &min_value, const T &max_value)
{
    LinkedList<T> empty;
    TryRun("empty[0]", [&empty]
           { std::cout << empty[0] << '\n'; });
    TryRun("empty.PopHead()", [&empty]
           { std::ignore = empty.PopHead(); });
    TryRun("empty.PopTail()", [&empty]
           { std::ignore = empty.PopTail(); });
    TryRun("random list with min > max", [min_value, max_value]
           { std::cout << LinkedList<T>(3, max_value, min_value, SEED) << '\n'; });
}

template <typename T>
void Demo(const std::string_view type_name, const T &min_value, const T &max_value)
{
    std::cout << "===== LinkedList<" << type_name << "> =====\n";
    DemoOperations(min_value, max_value);
    DemoExceptions(min_value, max_value);
    std::cout << '\n';
}

void SolveMergeTask()
{
    std::cout << "===== Task 8: merge sorted lists without duplicates =====\n";
    const LinkedList<int> a{MakeSortedList(10, 0, 3, SEED)};
    const LinkedList<int> b{MakeSortedList(8, 0, 4, SEED + 1)};
    std::cout << "a = " << a << "\nb = " << b << "\nmerged = " << MergeUnique(a, b) << '\n';
    LinkedList<int> empty;
    std::cout << "merged with empty = " << MergeUnique(a, empty) << '\n';
    LinkedList<int> unsorted(a);
    unsorted.PushHead(100);
    std::cout << "merge unsorted " << unsorted << '\n';
    TryRun("merge unsorted", [&unsorted, &b]
           { std::cout << MergeUnique(unsorted, b) << '\n'; });
}

int main()
{
    try
    {
        Demo<int>("int", -10, 10);
        Demo<double>("double", -10.0, 10.0);
        SolveMergeTask();
    }
    catch (const std::exception &error)
    {
        std::cout << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
