#pragma once

#include "../Interfaces/interface_output.h"

#include <queue>
#include <atomic>
#include <boost/format.hpp> 
#include <condition_variable> 
#include <thread>

/**
* @brief ����� ��� ������ � ������� � ��������� ������.
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
        // �������� �������� ������ � ������� � �������.
        std::lock_guard<std::mutex> q_lock(data_mutex);
        
        // ��������� ������.
        data_to_output.push(data);

        // ��������� ��������� �����.
        cond_var.notify_one();
    }

private: // Methods
    void worker_thread();

private: // Data
    std::atomic<bool> done{ false };
    std::condition_variable cond_var; // ��� ����������� ��������� ������ � �������.
    std::mutex data_mutex; // ������� ��� ������������� ������� � ��������� ������.

    std::queue<std::string> data_to_output;
        
    std::thread console_thread;
};