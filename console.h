#pragma once

#include "collector.h"

/**
* @brief Класс для вывода коллекции команд в консоль.
*/
class Console : public Observer {
public:
	Console() = delete;

	explicit Console(const std::shared_ptr<Collector>& cltr_ptr)
		: collector_ptr(cltr_ptr) {}

	~Console() = default;

	/**
	* Вывод коллекции команд в консоль.
	*/
	void output_to_console(const command_iterator& c_begin, const command_iterator& c_end);

	/**
	* Реакция на появление данных в коллекторе.
	*/
	void update() override;

private:
	const std::shared_ptr<Collector> collector_ptr;
};