#pragma once

#include "interpreter.h"

#include <cctype>
#include <iostream>
#include <mutex>
#include <map>

// ������������� ���������
using handle_t = std::uint64_t;

// ��� ��� ��������� ����������
using contexts_collection = std::map<handle_t, std::unique_ptr<Interpreter>>;

using contexts_iter = std::map<handle_t, std::unique_ptr<Interpreter>>::const_iterator;

class ContextCollector
{
public:
    ContextCollector() = default;

    handle_t add_context(const std::size_t bulk_size) {
        std::lock_guard<std::mutex> lg_mtx(collection_mutex);
        
        ++context_counter;

        contexts.insert({ context_counter , std::make_unique<Interpreter>(bulk_size)});

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
            if (console_mutex->try_lock()) {
                std::cout << "Incorrect context id has been provided ! Value: " << context_id << std::endl;
                console_mutex->unlock();
            }

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
            if (console_mutex->try_lock()) {
                std::cout << "Incorrect context id has been provided ! Value: " << context_id << std::endl;
                console_mutex->unlock();
            }

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
    
    // ������� ��� ���������� � ���������.
    std::mutex collection_mutex;
    
    // ������� ����������.
    handle_t context_counter{ 0 };
};