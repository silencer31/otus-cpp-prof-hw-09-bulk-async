#pragma once

#include "../Interfaces/interface_output.h"

#include <queue>
#include <atomic>
#include <boost/format.hpp> 
#include <condition_variable> 
#include <thread>

/**
* @brief Класс для вывода в консоль в отдельном потоке.
*/
class ConsoleWriter final : public IOutput
{
public:
    ConsoleWriter()
        : console_thread(&ConsoleWriter::worker_thread, this)
    {}

    ~ConsoleWriter() {
        done = true;

        if (console_thread.joinable()) {
            console_thread.join();
        }
    }

    void add_data(const std::string& data) override {
        // Пытаемся получить доступ к очереди с данными.
        std::lock_guard<std::mutex> q_lock(data_mutex);
        
        // Добавляем данные.
        data_to_output.push(data);

        // Оповещаем ожидающий поток.
        cond_var.notify_one();
    }

private: // Methods
    void worker_thread();

private: // Data
    std::atomic<bool> done{ false };
    std::condition_variable cond_var; // Для нотификации появления данных в очереди.
    std::mutex data_mutex; // Мьютекс для синхронизации доступа к коллекции данных.

    std::queue<std::string> data_to_output;
        
    std::thread console_thread;
};