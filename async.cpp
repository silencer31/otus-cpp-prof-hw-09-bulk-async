#include "async.h"

namespace async {
    handle_t connect(std::size_t bulk) {
        return context_collector.add_context(bulk);
    }

    void receive(handle_t handle, const char* data, std::size_t size) {
        context_collector.handle_message(handle, data, size);
    }

    void disconnect(handle_t handle) {
        context_collector.stop_handling(handle);
    }
    /*
    bool exist(handle_t handle) {
        return g_Dispatcher.Exist(handle);
    }*/
}