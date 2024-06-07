#pragma once

#include <string>

/**
* @brief ����� - ��������� ��� ������� ��� ������ � ����� ��������� ������.
*/
class IOutput
{

public:
	virtual ~IOutput() = default;

	virtual void add_data( const std::string& data) = 0;
};