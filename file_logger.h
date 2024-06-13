#pragma once

#include "collector.h"
#include "thread_writers/thread_file_writer.h"

/**
* @brief ����� ��� ������ ��������� ������ � ����.
*/
class FileLogger : public Observer {
public:
	FileLogger() = delete;

	explicit FileLogger(const std::shared_ptr<Collector>& cltr_ptr,
		const std::shared_ptr<IWriter>& iw_ptr,
		const std::uint32_t hid)
	: collector_ptr(cltr_ptr)
	, file_writer_ptr(iw_ptr)
	, handle_id(hid)
	{}
	
	~FileLogger() = default;

	/**
	* ��������� �������� ������.
	* @param c_begin �������� �� ������ ������
	* @param c_end �������� �� ��������� ������
	* @param ftime �����
	*/
	void write_commands_to_file(const command_iterator& c_begin, const command_iterator& c_end, const file_time& ftime);

	/**
	* ������� �� ��������� ������ � ����������.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
	const std::shared_ptr<IWriter>	 file_writer_ptr;
	const std::uint32_t handle_id;
};