/**
* @brief ����� - ��������� ��� ������� ��� ������ � ����� ��������� ������.
*/
class IOutput
{

public:
	virtual ~IOutput() = default;

	virtual void add_data( const std::string&) = 0;
};