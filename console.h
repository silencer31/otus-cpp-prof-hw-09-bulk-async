#pragma once

#include "collector.h"

/**
* @brief ����� ��� ������ ��������� ������ � �������.
*/
class Console : public Observer {
public:
	Console() = delete;

	explicit Console(const std::shared_ptr<Collector>& cltr_ptr)
		: collector_ptr(cltr_ptr) {}

	~Console() = default;

	/**
	* ����� ��������� ������ � �������.
	*/
	void output_to_console(const command_iterator& c_begin, const command_iterator& c_end);

	/**
	* ������� �� ��������� ������ � ����������.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
};