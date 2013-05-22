#include "exception.h"

Exception::Exception(const QString message) noexcept : std::exception()
{
	if( message.length() >= 50 )
		strcpy(_message, message.mid(0, 49).toStdString().c_str());
	else
		strcpy(_message, message.toStdString().c_str());
}

const char* Exception::what() const noexcept {
	return _message;
}
