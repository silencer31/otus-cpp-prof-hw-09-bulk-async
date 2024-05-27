#pragma once

/**
* @brief Класс - интерфейс для классов многопоточного вывода.
*/
class IWriter
{

public:
	virtual void add_data() = 0;
};