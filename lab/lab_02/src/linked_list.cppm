export module linked_list;

import std;

template <typename T>
struct Node
{
    T value{};
    Node *next{nullptr};
};

template <typename T>
concept ListValue = std::floating_point<T> || (std::integral<T> && !std::same_as<T, bool> && !std::same_as<T, char>);

template <typename T>
T RandomValue(std::mt19937 &engine, const T &min_value, const T &max_value)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> distribution(min_value, max_value);
        return distribution(engine);
    }
    else
    {
        std::uniform_real_distribution<T> distribution(min_value, max_value);
        return distribution(engine);
    }
}

export template <ListValue T>
class LinkedList final
{
private:
    Node<T> *tail_{nullptr};
    std::size_t size_{0};

    void CheckNotEmpty() const
    {
        if (size_ == 0)
        {
            throw std::out_of_range("LinkedList is empty");
        }
    }

    static void CheckRange(const T &min_value, const T &max_value)
    {
        if (min_value > max_value)
        {
            throw std::invalid_argument("Lower bound is greater than upper bound");
        }
    }

    Node<T> *NodeAt(std::size_t index) const
    {
        CheckNotEmpty();
        index %= size_;
        if (index == size_ - 1)
        {
            return tail_;
        }
        Node<T> *node{tail_->next};
        for (std::size_t i{0}; i < index; ++i)
        {
            node = node->next;
        }
        return node;
    }

    Node<T> *InsertAfterTail(const T &value)
    {
        auto *node{new Node<T>{value}};
        if (tail_ == nullptr)
        {
            node->next = node;
            tail_ = node;
        }
        else
        {
            node->next = tail_->next;
            tail_->next = node;
        }
        ++size_;
        return node;
    }

    void Append(LinkedList &other) noexcept
    {
        if (other.tail_ == nullptr)
        {
            return;
        }
        if (tail_ != nullptr)
        {
            Node<T> *head{tail_->next};
            tail_->next = other.tail_->next;
            other.tail_->next = head;
        }
        tail_ = other.tail_;
        size_ += other.size_;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    void Swap(LinkedList &other) noexcept
    {
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

public:
    LinkedList() = default;

    LinkedList(std::size_t size, const T &min_value, const T &max_value, unsigned int seed)
    {
        CheckRange(min_value, max_value);
        try
        {
            std::mt19937 engine{seed};
            for (std::size_t i{0}; i < size; ++i)
            {
                PushTail(RandomValue(engine, min_value, max_value));
            }
        }
        catch (...)
        {
            Clear();
            throw;
        }
    }

    ~LinkedList() { Clear(); }

    LinkedList(const LinkedList &other)
    {
        try
        {
            const Node<T> *node{other.tail_};
            for (std::size_t i{0}; i < other.size_; ++i)
            {
                node = node->next;
                PushTail(node->value);
            }
        }
        catch (...)
        {
            Clear();
            throw;
        }
    }

    LinkedList(LinkedList &&other) noexcept : tail_{other.tail_}, size_{other.size_}
    {
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    LinkedList &operator=(const LinkedList &other)
    {
        if (this != &other)
        {
            LinkedList copy(other);
            Swap(copy);
        }
        return *this;
    }

    LinkedList &operator=(LinkedList &&other) noexcept
    {
        if (this != &other)
        {
            Swap(other);
        }
        return *this;
    }

    [[nodiscard]] std::size_t Size() const noexcept { return size_; }

    [[nodiscard]] bool Empty() const noexcept { return size_ == 0; }

    void Clear() noexcept
    {
        for (std::size_t i{0}; i < size_; ++i)
        {
            Node<T> *head{tail_->next};
            tail_->next = head->next;
            delete head;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    void PushTail(const T &value) { tail_ = InsertAfterTail(value); }

    void PushTail(const LinkedList &other)
    {
        LinkedList copy(other);
        Append(copy);
    }

    void PushHead(const T &value) { InsertAfterTail(value); }

    void PushHead(const LinkedList &other)
    {
        LinkedList copy(other);
        copy.Append(*this);
        Swap(copy);
    }

    [[nodiscard]] T PopHead()
    {
        CheckNotEmpty();
        Node<T> *head{tail_->next};
        T value{std::move(head->value)};
        if (size_ == 1)
        {
            tail_ = nullptr;
        }
        else
        {
            tail_->next = head->next;
        }
        delete head;
        --size_;
        return value;
    }

    [[nodiscard]] T PopTail()
    {
        CheckNotEmpty();
        Node<T> *old_tail{tail_};
        T value{std::move(old_tail->value)};
        if (size_ == 1)
        {
            tail_ = nullptr;
        }
        else
        {
            Node<T> *previous{NodeAt(size_ - 2)};
            previous->next = old_tail->next;
            tail_ = previous;
        }
        delete old_tail;
        --size_;
        return value;
    }

    [[nodiscard]] std::size_t DeleteNode(const T &value)
    {
        std::size_t removed{0};
        Node<T> *previous{tail_};
        const std::size_t initial_size{size_};
        for (std::size_t i{0}; i < initial_size; ++i)
        {
            Node<T> *current{previous->next};
            if (current->value == value)
            {
                previous->next = current->next;
                if (current == tail_)
                {
                    tail_ = previous;
                }
                delete current;
                --size_;
                ++removed;
            }
            else
            {
                previous = current;
            }
        }
        if (size_ == 0)
        {
            tail_ = nullptr;
        }
        return removed;
    }

    template <typename Self>
    decltype(auto) operator[](this Self &&self, std::size_t index)
    {
        return std::forward_like<Self>(self.NodeAt(index)->value);
    }

    friend std::ostream &operator<<(std::ostream &stream, const LinkedList &list)
    {
        stream << '(';
        if (list.tail_ != nullptr)
        {
            const Node<T> *node{list.tail_->next};
            for (std::size_t i{0}; i < list.size_; ++i)
            {
                if (i != 0)
                {
                    stream << ", ";
                }
                stream << node->value;
                node = node->next;
            }
        }
        return stream << ')';
    }
};
