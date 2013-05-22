#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

class Exception : public std::exception
{
public:
	Exception(const QString message) noexcept;
	const char* what() const noexcept;
private:
	char _message[50];
};

#endif // EXCEPTION_H
