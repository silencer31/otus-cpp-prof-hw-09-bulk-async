#pragma once

#include "concurrent/concurrent_queue.h"
#include "Interfaces/interface_writer.h" 
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
            threads.emplace_back(&FileWriter::worker_thread, this, i);
        }
    }

    ~FileWriter() {
        done = true;
    }

    void add_data(const std::uint32_t& hid, const std::uint64_t& time, const std::string& data) override {
        // Добавляем данные в коллекцию.
        data_to_write.push(hid, time, data);
        
        // Оповещаем ожидающий поток.
        cond_var.notify_one();
    }

private: // Methods

    void worker_thread(const uint32_t thread_id);

private: // Data
    std::atomic<bool> done{ false };
    std::condition_variable cond_var; // Для нотификации появления данных в очереди.
    std::mutex data_mutex; // Мьютекс для синхронизации доступа к коллекции данных.

    ConcurrentQueue<std::string> data_to_write; // Коллекция данных.

    std::vector<std::thread> threads; // Потоки для выполнения.

    ThreadsJoiner threads_joiner; // Для корректного завершения потоков.
};