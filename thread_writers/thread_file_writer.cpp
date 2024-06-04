#include "thread_file_writer.h"

#include <fstream>
#include <sstream>
#include <boost/format.hpp> 

void FileWriter::worker_thread(const uint32_t thread_id)
{
    while (!done) {
        // Пытаемся получить доступ к коллекции с данными.
        std::unique_lock<std::mutex> ulock(data_mutex);

        // Отпускаем мьютекс и ожидаем, пока в очереди появятся данные.
        cond_var.wait(ulock, [this]()->bool { return !data_to_write.empty() || done; });

        // Мьютекс снова захвачен.

        if (data_to_write.empty()) { continue; }

        // Берём первый доступный элемент в коллекции с данными.
        const auto element = std::move(data_to_write.front());

        // Удаляем из очереди принятый в обработку элемент.
        data_to_write.pop();

        // Освобождаем доступ к коллекции.
        lock.unlock();

        // Записываем данные в файл.
        const auto hid  = std::get<0>(element);
        const auto time = std::get<1>(element);

        std::string filename = boost::str(boost::format("bulk_%1%_id[%2%]_%3%.log") % time % thread_id % hid);
    
        std::ofstream file(filename.str(), std::ofstream::out);

        if (file.is_open()) {
            file << std::get<2>(element);
        }
    }
}

