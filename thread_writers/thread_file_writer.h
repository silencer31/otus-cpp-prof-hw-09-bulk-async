#pragma once

#include "../concurrent/concurrent_queue.h"
#include "../Interfaces/interface_writer.h" 
#include "../threads_joiner.h"

#include <atomic>
#include <boost/format.hpp> 
#include <condition_variable> 

#include <iostream>

/**
* @brief ����� ��� ������ � ����� �� ���� �������. �������� ���������������� ������� ������ ��� ������.
*/
class FileWriter final : public IWriter
{

public:
    FileWriter(const uint32_t thr_number, std::shared_ptr<std::mutex>& cm_ptr)
        : console_mutex_ptr(cm_ptr)
        , threads_joiner(threads)
    {
        const uint32_t max_thr_number = std::thread::hardware_concurrency();
        const uint32_t threads_number = (thr_number > max_thr_number ? max_thr_number : thr_number);

        threads.reserve(threads_number);

        for (uint32_t i = 0; i < threads_number; ++i) {
            threads.emplace_back(&FileWriter::worker_thread, this, i);
            ++active_threads_number;
        }

        std::cout << "File writer - Constructor" << std::endl;
    }

    ~FileWriter() {
        if (!done) {
            for (unsigned int i = 0; i < threads.size(); ++i) {
                if (threads[i].joinable()) {
                    threads[i].join();
                }
            }
        }

        std::cout << "File writer - Destructor" << std::endl;
    }
   
    /**
    * ���������� ������ � ���������������� �������.
    * @param hid ����� ���������.
    * @param time �����. 
    * @param data ������.
    */
    void add_data(const std::uint32_t& hid, const std::uint64_t& time, const std::string& data) override {
        // ��������� ������ � ���������.
        data_to_write.push(hid, time, data);
        
        // ��������� ��������� �����.
        cond_var.notify_one();
    }

    /**
    * @return �������� �� ������� � ������� ������.
    */
    bool empty() override {
        // �������� �������� ������ � ������� � �������.
        std::lock_guard<std::mutex> q_lock(data_mutex);

        return data_to_write.empty();
    }

    /**
    * @return ���� �� ���� �� ���� �������� �����.
    */
    bool active() override {
        return (active_threads_number > 0);
    }

    /**
    * �������� ������� � ����������� ��������� ������.
    */
    void stop_process() override {
        done = true;
        for (unsigned int i = 0; i < threads.size(); ++i) {
            cond_var.notify_one();
        }
    }

private: // Methods

    /**
    * �����, ���������� � ��������� ������.
    * @param thread_id ����� ������.
    */
    void worker_thread(const uint32_t thread_id);

private: // Data
    // ������� ��� ������������� ������ �� ������� �� ������ �������.
    std::shared_ptr<std::mutex> console_mutex_ptr;

    std::condition_variable cond_var; // ��� ����������� ��������� ������ � �������.
    std::mutex data_mutex; // ������� ��� ������������� ������� � ��������� ������.

    ConcurrentQueue<std::string> data_to_write; // ��������� ������.

    std::vector<std::thread> threads; // ������ ��� ����������.

    ThreadsJoiner threads_joiner; // ��� ���������� ������� ���������.
};