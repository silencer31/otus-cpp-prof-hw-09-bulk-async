#include "thread_console_writer.h"

#include <iostream>

void ConsoleWriter::worker_thread()
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

        std::cout << element << std::endl;
	}
}

