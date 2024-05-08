#include "bulk_async.h"

namespace async {
    AsyncDispatcher g_Dispatcher;

    handle_t connect(std::size_t bulk) {
        return g_Dispatcher.Add(bulk, aOutStream);
    }

    void receive(handle_t handle, const char* data, std::size_t size) {
        g_Dispatcher.ReceiveMsg(handle, data, size);
    }

    void disconnect(handle_t handle) {
        g_Dispatcher.CloseDispatcher(handle);
    }

    bool exist(handle_t handle) {
        return g_Dispatcher.Exist(handle);
    }
}