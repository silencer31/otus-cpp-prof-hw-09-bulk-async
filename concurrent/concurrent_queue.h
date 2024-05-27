#pragma once
#include <queue>
#include <mutex>

/**
* \brief  Потокобезопасная очередь с коллекцией строк для записи в файлы или вывода на консоль.
*/
template<class T>
class ConcurrentQueue {
public:

    bool empty() const {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.empty();
    }

    void push(const T obj) {
        std::lock_guard<std::mutex> guard(queue_mtx);
        collection.push_back(obj);
    }

    T front() {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.front();
    }

    void pop() {
        std::lock_guard<std::mutex> guard(queue_mtx);
        collection.pop_front();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.size();
    }

    void clear() {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return m_Deque.clear();
    }
private:
    std::queue<T>      collection;
    mutable std::mutex queue_mtx;
};