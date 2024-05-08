#pragma once

#include <cctype>
#include <iostream>
#include <mutex>
#include <map>

// Идентификатор контекста
using handle_t = std::uint64_t;

using ContextMap = std::map<handle_t, std::shared_ptr<Context>>;

class ContextCollector
{
public:
    ContextCollector() = default;

    handle_t add_context(const std::size_t aBulkSize, std::ostream& aOutStream) {
        if (!m_OutPrintMutex)
            return 0;

        std::lock_guard<std::mutex> locker(collection_mutex);
        
        ++context_counter;
        
        context_collection.insert({ context_counter,
            std::make_unique<CommandDispatcher>(aBulkSize, aOutStream, 2, m_OutPrintMutex) });
        
        return context_counter;
    }

    void handle_message(const handle_t aHandle, const char* aData, const std::size_t aSize) {
        decltype(context_collection)::const_iterator iter;
        {
            std::lock_guard<std::mutex> locker(collection_mutex);
            iter = context_collection.find(aHandle);
        }

        if (iter == context_collection.end()) {
            if (m_OutPrintMutex && m_OutPrintMutex->try_lock()) {
                std::cout << "Cant find elem with id = " << aHandle << std::endl;
                m_OutPrintMutex->unlock();
            }
            return;
        }
        if (iter->second) {
            iter->second->ProcessCmdLine(std::string(aData, aData + aSize));
        }
    }

    void stop_handling(const handle_t aHandle) {
        decltype(context_collection)::const_iterator iter;
        
        {
            std::lock_guard<std::mutex> locker(collection_mutex);
            iter = context_collection.find(aHandle);
        }

        if (iter == context_collection.end()) {
            if (m_OutPrintMutex && m_OutPrintMutex->try_lock()) {
                std::cout << "Cant find elem with id = " << aHandle << std::endl;
                m_OutPrintMutex->unlock();
            }
            return;
        }

        if (iter->second) {
            iter->second->Flush();
        }

        {
            std::lock_guard<std::mutex> locker(collection_mutex);
            context_collection.erase(iter);
        }
    }

    bool Exist(const handle_t aHandle) {
        std::lock_guard<std::mutex> locker(collection_mutex);
        return  context_collection.find(aHandle) != context_collection.end();
    }
private:
    ContextMap context_collection;
    
    std::mutex collection_mutex;
    std::shared_ptr<std::mutex> m_OutPrintMutex = std::make_shared<std::mutex>();
    
    handle_t context_counter = 0;
};