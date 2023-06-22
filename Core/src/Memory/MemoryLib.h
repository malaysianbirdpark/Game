#pragma once

#include "MemoryAllocator.h"

namespace Engine {
// TODO: TEMP
template <typename T>
std::unique_ptr<T> MakeUnique(T&& t) {
    return std::make_unique<T>(std::forward<T>(t));
}

template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<T> MakeShared(T&& t) {
    return std::make_shared<T>(std::forward<T>(t));
}

template <typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T, uint32_t SIZE>
using x_array = std::array<T, SIZE>;

template <typename T>
using x_vector = std::vector<T, STLAllocator<T>>;

template <typename T>
using x_list = std::list<T, STLAllocator<T>>;

template <typename Key, typename T, typename Pred = std::less<Key>>
using x_map = std::map<Key, T, Pred, STLAllocator<std::pair<Key const, T>>>;

template <typename Key, typename Pred = std::less<Key>>
using x_set = std::set<Key, Pred, STLAllocator<Key>>;

template <typename T>
using x_deque = std::deque<T, STLAllocator<T>>;

template <typename T, typename Container = x_deque<T>>
using x_queue = std::queue<T, Container>;

template <typename T, typename Container = x_deque<T>>
using x_stack = std::stack<T, Container>;

template <typename T, typename Container = x_vector<T>, typename Pred = std::less<typename Container::value_type>>
using x_priority_queue = std::priority_queue<T, Container, Pred>;

using x_string = std::basic_string<char, std::char_traits<char>, STLAllocator<char>>;
using x_wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t>>;

template <typename Key, typename T, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
using x_unordered_map = std::unordered_map<Key, T, Hasher, KeyEq, STLAllocator<std::pair<Key const, T>>>;

template <typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
using x_unordered_set = std::unordered_map<Key, Hasher, KeyEq, STLAllocator<Key>>;
}
