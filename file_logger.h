#pragma once

#include "collector.h"

/**
* @brief ����� ��� ������ ��������� ������ � ����.
*/
class FileLogger : public Observer {
public:
	FileLogger() = delete;

	explicit FileLogger(const std::shared_ptr<Collector>& cltr_ptr)
	: collector_ptr(cltr_ptr) {}
	
	~FileLogger() = default;

	void write_commands_to_file(const command_iterator& c_begin, const command_iterator& c_end, const file_time& ftime);

	/**
	* ������� �� ��������� ������ � ����������.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
};