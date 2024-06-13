#pragma once

#include "interpreter.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <mutex>
#include <map>

#define FILE_THREADS_NUMBER 2

// ������������� ���������
using handle_t = std::uint32_t;

// ��� ��� ��������� ����������
using contexts_collection = std::map<handle_t, std::unique_ptr<Interpreter>>;

using contexts_iter = std::map<handle_t, std::unique_ptr<Interpreter>>::const_iterator;

/**
* @brief ����� ��� ����� � ���������� ����������� ��������� ������.
*/
class ContextCollector
{
public:
    ContextCollector()
        : console_mutex_ptr( std::make_shared<std::mutex>() )
        , io_ptr(std::make_shared<ConsoleWriter>(console_mutex_ptr))
        , iw_ptr(std::make_shared<FileWriter>(FILE_THREADS_NUMBER, console_mutex_ptr))
    {}

    ~ContextCollector() {
        // ��� ��������� ���� ������������ ������.
        while (!iw_ptr->empty() || !io_ptr->empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // ������������� ������.
        io_ptr->stop_process();
        iw_ptr->stop_process();
        

        // ��� ����������� ���������� ���� �������.
        while (iw_ptr->active() || io_ptr->active()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "\nContext Collector - Destructor" << std::endl;
    }

    /**
    * ���������� ������ ���������.
    * @param bulk_size ������ ����������� ��� ������ ������
    * @return ����� ���������
    */
    handle_t add_context(const std::size_t bulk_size);

    /**
    * ��������� �������� ������.
    * @param context_id ����� ���������
    * @param data ������
    * @param data_len ����� ������
    */
    void handle_message(const handle_t context_id, const char* data, const std::size_t data_len);

    /**
    * ����������� �������� ��������� ��� ���������� ���������.
    * @param context_id ����� ���������
    */
    void stop_handling(const handle_t context_id);
    
private:
    // ��������� ����������.
    contexts_collection contexts;
    
    // ������� ��� ������������� ������� � ��������.
    std::mutex collection_mutex;
 
    // ������� ����������.
    handle_t context_counter{ 0 };

    // ������� ��� ������������� ������ �� ������� �� ������ �������.
    std::shared_ptr<std::mutex> console_mutex_ptr;

    // ������ ��� ������ � ������� � ��������� ������.
    const std::shared_ptr<IWriter> io_ptr;

    // ������ ��� ������ � ����� ����� ��� �������.
    const std::shared_ptr<IWriter> iw_ptr;
};