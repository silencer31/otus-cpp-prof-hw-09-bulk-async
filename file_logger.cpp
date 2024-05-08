#include "file_logger.h"

#include <fstream>
#include <sstream>

/**
* Запись коллекции в файл с указанием временной метки.
*/
void FileLogger::write_commands_to_file(const command_iterator& c_begin, const command_iterator& c_end, const file_time& ftime)
{
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ftime.time_since_epoch()).count();
	
	std::stringstream filename;
	filename << "bulk" << seconds << ".log";

	std::ofstream file(filename.str(), std::ofstream::out);
	
	for (command_iterator comm_iter = c_begin; comm_iter != c_end; ++comm_iter) {
		file << (*comm_iter);
		if (comm_iter + 1 != c_end) {
			file << ", ";
		}
	}
}

void FileLogger::update()
{
	write_commands_to_file(collector_ptr->get_iter_begin(), collector_ptr->get_iter_end(), collector_ptr->get_file_time());
}