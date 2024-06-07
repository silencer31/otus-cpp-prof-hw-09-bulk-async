#include "thread_console_writer.h"

#include <iostream>

void ConsoleWriter::worker_thread()
{
	while (!done) {
        // �������� �������� ������ � ��������� � �������.
        std::unique_lock<std::mutex> data_lock(data_mutex);

        // ��������� ������� � �������, ���� � ������� �������� ������.
        cond_var.wait(data_lock, [this]()->bool { return !data_to_output.empty() || done; });

        // ������� ����� ��������.

        if (data_to_output.empty()) { continue; }

        // ���� ������ ��������� ������� � ��������� � �������.
        const auto element = std::move(data_to_output.front());

        // ������� �� ������� �������� � ��������� �������.
        data_to_output.pop();

        // ����������� ������ � ���������.
        data_lock.unlock();

        std::cout << element << std::endl;
	}
}

