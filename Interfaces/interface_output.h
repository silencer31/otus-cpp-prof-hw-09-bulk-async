/**
* @brief Класс - интерфейс для классов для вывода в одном отдельном потоке.
*/
class IOutput
{

public:
	virtual ~IOutput() = default;

	virtual void add_data( const std::string&) = 0;
};