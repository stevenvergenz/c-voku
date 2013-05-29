#include "logger.h"

Logger* Logger::_instance = nullptr;

const Logger *Logger::instance()
{
	if( _instance == nullptr ){
		_instance = new Logger();
	}
	return _instance;
}

void Logger::log(const QString msg){
	instance()->raiseMessage(msg);
}

Logger::Logger() : QObject(nullptr)
{
}

void Logger::raiseMessage(const QString msg) const
{
	emit onMessage(msg);
}
