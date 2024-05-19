#pragma once

#include "context_collector.h"

namespace async
{
	// ��������� ����������.
	ContextCollector context_collector;

	/*
	* @brief ������� �������� ��� ���������� ������������� ������
	* @param number ���������� ������ � �����
	* @return �������� ��� ������� receive � disconnect
	*/
	handle_t connect(std::size_t number);

	/*
	* @param handle �������� ��� ������� receive � disconnect
	* @param data ��������� �� ����� � ���, ��� �����
	* @param number ���������� �������� � ������
	*/
	void receive(handle_t handle, const char* data, std::size_t number);

	/*
	* @param handle �������� ��� ������� receive � disconnect
	*/
	void disconnect(handle_t handle);

}