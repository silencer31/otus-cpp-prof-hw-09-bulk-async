#pragma once

#include "concurrent/concurrent_queue.h"
#include "writer_interface.h"

#include <atomic>
#include <boost/format.hpp> 
#include <condition_variable> 
#include <thread>

/**
* @brief ����� ��� ������ � ������� � ��������� ������.
*/
class ConsoleWriter final : public IWriter
{
public:
    ConsoleWriter() = default;

    void add_data(const std::string& data) override {
        data_to_output.push(data);
    }

private: // Methods


private: // Data
    std::condition_variable cond_var;
    std::mutex m_MutexThread;

    //ConcurentDeque<std::string> m_Deque;

    std::atomic<bool> m_Execute{ true };
    std::thread console_thread;

    // ������� ��� ������ �� �����.
    std::mutex console_mutex;

    ConcurrentQueue<std::string> data_to_output;
};