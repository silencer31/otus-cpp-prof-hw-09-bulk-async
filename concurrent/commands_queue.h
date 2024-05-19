#pragma once
#include <deque>
#include <mutex>

/**
* \brief  Потокобезопасная очередь для потоков записи в файлы.
*/
template<class T>
class CommandsDeque {
public:

    bool empty() const {
        std::lock_guard<std::mutex> guard(m_Mutex);
        return m_Deque.empty();
    }

    void push_back(const T obj) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_Deque.push_back(obj);
    }

    T front() {
        std::lock_guard<std::mutex> guard(m_Mutex);
        return m_Deque.front();
    }

    void pop_front() {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_Deque.pop_front();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> guard(m_Mutex);
        return m_Deque.size();
    }

    void clear() {
        std::lock_guard<std::mutex> guard(m_Mutex);
        return m_Deque.clear();
    }
private:
    std::deque<T>      m_Deque;
    mutable std::mutex m_Mutex;
};