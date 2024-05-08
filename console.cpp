#include "console.h"

#include <iostream>

void Console::output_to_console(const command_iterator& c_begin, const command_iterator& c_end)
{
	for (command_iterator comm_iter = c_begin; comm_iter != c_end; ++comm_iter) {
		std::cout << (*comm_iter);
		if (comm_iter + 1 != c_end) {
			std::cout << ", ";
		}
	}

	std::cout << std::endl;
}

void Console::update()
{
	output_to_console(collector_ptr->get_iter_begin(), collector_ptr->get_iter_end());
}