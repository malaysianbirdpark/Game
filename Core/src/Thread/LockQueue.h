#pragma once

namespace Engine {
    template <typename T>
    class LockQueue {
    public:
        template <typename... Args>
        void emplace(Args&&... args) {
            std::lock_guard lock {_mutex};
            _queue.emplace(std::forward<Args>(args)...);
            _cv.notify_one();
        }

        void push(T&& value) {
            std::lock_guard lock {_mutex};
            _queue.push(std::forward<T>(value));
            _cv.notify_one();
        }

        bool try_pop(T& value) {
            std::lock_guard lock {_mutex};
            if (_queue.empty()) return false;

            value = std::move(_queue.front());
            _queue.pop();

            return true;
        }

        void wait_pop(T& value) {
            std::unique_lock lock {_mutex};
            _cv.wait(lock, [this] () {
                return _queue.empty() == false;
            });
            value = std::move(_queue.front());
            _queue.pop();
        }
    private:
        std::mutex    _mutex {};
        std::queue<T> _queue {};
        std::condition_variable _cv {};
    };
}

