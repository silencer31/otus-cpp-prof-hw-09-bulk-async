#pragma once

#include "collector.h"
#include "thread_writers/thread_console_writer.h"

/**
* @brief ����� ��� ������ ��������� ������ � �������.
*/
class Console : public Observer {
public:
	Console() = delete;

	explicit Console(const std::shared_ptr<Collector>& cltr_ptr, const std::shared_ptr<IWriter>& io_ptr)
		: collector_ptr(cltr_ptr)
		, console_writer_ptr(io_ptr)
	{}

	~Console() = default;

	/**
	* ����� ������ � �������.
	* @param c_begin �������� �� ������ ������
	* @param c_end �������� �� ��������� ������
	*/
	void output_to_console(const command_iterator& c_begin, const command_iterator& c_end);

	/**
	* ������� �� ��������� ������ � ����������.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
	const std::shared_ptr<IWriter>	 console_writer_ptr;
};