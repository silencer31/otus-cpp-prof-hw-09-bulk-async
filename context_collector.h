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

class ContextCollector
{
public:
    ContextCollector()
        : io_ptr(std::make_shared<ConsoleWriter>())
        , iw_ptr(std::make_shared<FileWriter>(FILE_THREADS_NUMBER))
    {}

    handle_t add_context(const std::size_t bulk_size) {
        std::lock_guard<std::mutex> lg_mtx(collection_mutex);
        
        ++context_counter;

        contexts.insert(
            { context_counter, std::make_unique<Interpreter>(bulk_size, context_counter, io_ptr, iw_ptr)}
        );

        return context_counter;
    }

    void handle_message(const handle_t context_id, const char* data, const std::size_t data_len) {
        contexts_iter iter;
        
        {
            std::lock_guard<std::mutex> lg_mtx(collection_mutex);
            iter = contexts.find(context_id);
        }

        // ��������, ��� �������� ���������� ��������.
        if (iter == contexts.end()) {
            std::stringstream message;
            message << "Unable to handle message. Incorrect context id has been provided ! Value: " << context_id;

            io_ptr->add_data(message.str());

            return;
        }

        if (!iter->second) { return; }

        iter->second->handle_input(std::string(data, data + data_len));
    }

    void stop_handling(const handle_t context_id) {
        contexts_iter iter;

        {
            std::lock_guard<std::mutex> lg_mtx(collection_mutex);
            iter = contexts.find(context_id);
        }

        // ��������, ��� �������� ���������� ��������.
        if (iter == contexts.end()) {
            std::stringstream message;
            message << "Unable to stop handling. Incorrect context id has been provided ! Value: " << context_id;

            io_ptr->add_data(message.str());

            return;
        }

        if (!iter->second) { return; }

        // ��������� ������� ���� ������.
        iter->second->handle_eof();

        // ������� �������� �� ���������.
        {
            std::lock_guard<std::mutex> lg_mtx(collection_mutex);
            contexts.erase(context_id);
        }
    }
    
private:
    // ��������� ����������.
    contexts_collection contexts;
    
    // ������� ��� ������������� ������� � ��������.
    std::mutex collection_mutex;
    
    // ������� ����������.
    handle_t context_counter{ 0 };

    // ������ ��� ������ � ������� � ��������� ������.
    const std::shared_ptr<IOutput> io_ptr;

    // ������ ��� ������ � ����� ����� ��� �������.
    const std::shared_ptr<IWriter> iw_ptr;
};