#pragma once

#include <string>

/**
* @brief Класс - интерфейс для классов многопоточного вывода в файлы.
*/
class IWriter
{

public:
	virtual ~IWriter() = default;

	virtual void add_data(const std::uint32_t& hid, const std::uint64_t& time, const std::string& data) = 0;
};