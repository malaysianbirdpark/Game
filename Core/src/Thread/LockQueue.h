#pragma once

namespace Engine {
template <typename T>
class LockQueue {
public:
    void Push(T const& item) {
        WRITE_LOCK;
        _items.push(item);
    }

    T Pop() {
        WRITE_LOCK;
        if (_items.empty())
            return T{};

        auto ret {_items.front()};
        _items.pop();

        return ret;
    }

    void PopAll(OUT x_vector<T>& items) {
        WRITE_LOCK;
        while (T item {Pop()})
            items.push_back(item);
    }

    void Clear() {
        WRITE_LOCK;
        _items = x_queue<T>{};
    }
private:
    USE_LOCK;
    x_queue<T> _items;
};

}
