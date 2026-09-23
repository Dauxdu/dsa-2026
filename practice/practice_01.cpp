const char *longest_repeat_seq(const char *str, std::size_t *max_len)
{
    if (!str || !*str)
    {
        if (max_len) { *max_len = 0; }
        return nullptr;
    }

    const char *best_start = str;
    std::size_t best_len = 0;
    const char *ptr = str;

    while (*ptr)
    {
        const char *cur_start = ptr;

        while (*ptr && *ptr == *cur_start)
        {
            ++ptr;
        }

        std::size_t cur_len = ptr - cur_start;
        if (cur_len > best_len)
        {
            best_len = cur_len;
            best_start = cur_start;
        }
    }

    if (max_len) { *max_len = best_len; }
    return best_start;
}

const char *find_sub_str(const char *str, const char *substr)
{
    if (!str || !substr) { return nullptr; }
    if (!*substr) { return str; }

    std::size_t sub_len = 0;
    while (substr[sub_len]) { sub_len++; }

    const char *ptr = str;
    while (*ptr)
    {
        const char *end_check = ptr + sub_len - 1;
        
        const char *ps = ptr;
        const char *pm = substr;
        
        while (*pm && *ps == *pm)
        {
            ++ps;
            ++pm;
        }

        if (!*pm) { return ptr; }        
        if (!*ps) { break; }

        ++ptr;
    }
    return nullptr;
}
