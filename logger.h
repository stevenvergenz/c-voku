#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
	Q_OBJECT
public:
	static void log(const QString msg);
	static const Logger* instance();
	
signals:
	void onMessage(const QString msg) const;
	
public slots:

private:
	Logger();
	void raiseMessage(const QString msg) const;
	static Logger* _instance;
};

#endif // LOGGER_H
