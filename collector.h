#pragma once

#include <vector>
#include <string>
#include <memory>
#include <chrono>

#include "Observ/observer.h"

using command_iterator = std::vector<std::string>::const_iterator;
using file_time = std::chrono::system_clock::time_point;

// Тип введённых данных.
enum class InputType {
	NO_INPUT,
	COMMAND,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	END_INPUT
};

// Текущий тип хранилища команд.
enum class StorageType {
	UNKNOWN_T,
	STATIC_T,
	DYNAMIC_T
};

/**
* @brief Класс, содержащий коллекцию команд и управляющий ею.
*/
class Collector : public Observable {
public:
	Collector() = delete;

	explicit Collector( size_t size) :
		collection_max_size(size), storage_type(StorageType::UNKNOWN_T),
		open_brackets_number(0), close_brackets_number(0)
	{
		commands_collection.reserve(collection_max_size);
	}


	~Collector() {
		flush_collection(true);
	}
	
	/**
	* Обработка ввода.
	*/
	void handle_input(InputType input_type, const std::string& command);

	/**
	*  Вывод всей коллекции в консоль и запись в файл через оповещение наблюдателей.
	*  @param clear_flag - очищать ли коллекцию после вывода.
	*/
	void flush_collection(bool clear_flag);

	/**
	* Подготовка коллекции к новому вводу неизвестных данных.
	*/
	void reset_collection();

	/**
	* Функция для проверки корректного создания файлов в рамках тестирования.
	* @return время создания файла.
	*/
	file_time get_file_time() {
		return first_command_time;
	}

	/**
	* @return константный итератор на начало коллекции.
	*/
	command_iterator get_iter_begin() {
		return commands_collection.cbegin();
	}

	/**
	* @return константный итератор на конец коллекции.
	*/
	command_iterator get_iter_end() {
		return commands_collection.end();
	}

private:
	size_t collection_max_size;
	std::vector<std::string> commands_collection;

	StorageType storage_type;
	int open_brackets_number;
	int close_brackets_number;

	file_time first_command_time;
};