#pragma once

/**
* @brief Класс - интерфейс для классов многопоточного вывода в файлы.
*/
class IWriter
{

public:
	virtual ~IWriter() = default;

	virtual void add_data(const std::uint32_t& , const std::uint64_t& , const std::string& ) = 0;
};