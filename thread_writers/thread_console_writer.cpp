#include "thread_console_writer.h"

#include <iostream>

void ConsoleWriter::worker_thread()
{
	while (!done) {
        // �������� �������� ������ � ��������� � �������.
        std::unique_lock<std::mutex> ulock(data_mutex);

        // ��������� ������� � �������, ���� � ������� �������� ������.
        cond_var.wait(ulock, [this]()->bool { return !data_to_write.empty() || done; });

        // ������� ����� ��������.

        if (data_to_write.empty()) { continue; }

        // ���� ������ ��������� ������� � ��������� � �������.
        const auto element = std::move(data_to_write.front());

        // ������� �� ������� �������� � ��������� �������.
        data_to_write.pop();

        // ����������� ������ � ���������.
        lock.unlock();

        std::cout << element << std::endl;
	}
}

