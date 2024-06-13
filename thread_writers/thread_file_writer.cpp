#include "thread_file_writer.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <boost/format.hpp> 

// �����, ���������� � ��������� ������.
void FileWriter::worker_thread(const uint32_t thread_id)
{
    {
        std::lock_guard<std::mutex> lg(*console_mutex_ptr);
        std::cout << "File writer thread " << thread_id << " started " << std::endl;
    }

    while (!done) {
        // �������� �������� ������ � ��������� � �������.
        std::unique_lock<std::mutex> data_lock(data_mutex);

        // ��������� ������� � �������, ���� � ������� �������� ������.
        cond_var.wait(data_lock, [this]()->bool { return !data_to_write.empty() || done; });

        // ������� ����� ��������.

        if (data_to_write.empty()) { 
            data_lock.unlock();
            continue;
        }

        // ���� ������ ��������� ������� � ��������� � �������.
        const auto element = std::move(data_to_write.front());

        // ������� �� ������� �������� � ��������� �������.
        data_to_write.pop();

        // ����������� ������ � ���������.
        data_lock.unlock();

        // ���������� ������ � ����.
        const auto hid  = std::get<0>(element); // handle (��������) id
        const auto time = std::get<1>(element);

        std::string filename;
        uint64_t delta{ 0 };

        //const std::string filename = boost::str(boost::format("bulk_%1%_id[%2%]_%3%%4%.log") % time % thread_id % hid % delta);

        while (true) {
            filename = boost::str(boost::format("bulk_%1%_id[%2%]_%3%%4%.log") % time % thread_id % hid % delta);
            if (!std::filesystem::exists(filename)) {
                break;
            }
            ++delta;
        }

        std::ofstream file(filename, std::ofstream::out);
        
        if (file.is_open()) {
            file << std::get<2>(element);
        }
    }

    {
        std::lock_guard<std::mutex> lg(*console_mutex_ptr);
        std::cout << "File writer thread " << thread_id << " finished " << std::endl;
    }

    --active_threads_number;
}

