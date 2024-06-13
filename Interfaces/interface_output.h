#pragma once

#include <string>
#include <atomic>

/**
* @brief Класс - интерфейс для классов для вывода в одном отдельном потоке.
*/
class IOutput
{
protected:
	std::atomic<bool> done{ false };

public:
	virtual ~IOutput() = default;

	virtual void add_data( const std::string& data) = 0;

	virtual void stop_process() = 0;
};