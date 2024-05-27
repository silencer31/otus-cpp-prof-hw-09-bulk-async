#pragma once

#include "concurrent/concurrent_queue.h"
#include "writer_interface.h"
#include "threads_joiner.h"

#include <atomic>
#include <boost/format.hpp> 
#include <condition_variable> 


/**
* @brief Класс для записи в файлы из двух потоков. Содержит потокобезопасную очередь данных для записи.
*/
class FileWriter final : public IWriter
{
public:
    FileWriter(const uint32_t thr_number)
        : threads_joiner(threads)
    {
        const uint32_t max_thr_number = std::thread::hardware_concurrency();
        const uint32_t threads_number = (thr_number > max_thr_number ? max_thr_number : thr_number);

        threads.reserve(threads_number);

        for (uint32_t i = 0; i < threads_number; ++i) {
            threads.emplace_back(&FileWriter::worker_thread, this);
        }
    }

    ~FileWriter() {
        done = true;
    }

    void add_data(const std::string& data) override {
        data_to_write.push(data);
    }

private: // Methods

    void worker_thread() {
        while (!done) {

        }
    }

private: // Data
    std::atomic<bool> done{ false };

    ConcurrentQueue<std::string> data_to_write;
    
    std::vector<std::thread> threads;

    ThreadsJoiner threads_joiner;

    std::condition_variable cond_var;
    std::mutex m_MutexThread;

    

    
    

    // Мьютекс для вывода в файлы.
    std::mutex files_mutex;

    
};