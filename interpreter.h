#pragma once

#include "console.h"
#include "file_logger.h"

/**
* @brief Класс - анализатор поступающих команд из консоли.
*/
class Interpreter {
public:
	Interpreter() = delete;
	
	explicit Interpreter(std::shared_ptr<Collector>  cltr_ptr)
		: collector_ptr(cltr_ptr) {
		// Создаем Console и передаем ему указатель на коллекцию с данными.
		console_ptr = std::make_shared<Console>(collector_ptr);
		
		// Создаем File Logger и передаем ему указатель на коллекцию с данными.
		file_logger_ptr = std::make_shared<FileLogger>(collector_ptr);

		// У коллекции с данными указываем Console как обозреватель.
		collector_ptr->attach(console_ptr);

		// У коллекции с данными указываем File Logger как обозреватель.
		collector_ptr->attach(file_logger_ptr);
	}
	
	~Interpreter() {
		collector_ptr->detach(console_ptr);
		collector_ptr->detach(file_logger_ptr);
	}

	/**
	* Обработка введённой строки.
	*/
	void handle_input(const std::string& command);

	/**
	* Обработка полученного EOF.
	*/
	void handle_eof();

private:
	const std::shared_ptr<Collector>  collector_ptr; // Указатель на объект с коллекцией с данными.
	
	std::shared_ptr<FileLogger> file_logger_ptr; // Указатель на объект для вывода коллекции данных в файл.
	std::shared_ptr<Console>	console_ptr;	 // Указатель на объект для вывода коллекции данных в консоль.
};