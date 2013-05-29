#ifndef CELL_H
#define CELL_H

#include <QSet>
#include <QDebug>
#include <QPoint>

#include "logger.h"

class Cell
{
public:

	static const char UNKNOWN = -1;
	static QSet<char> fullDomain;

	Cell(QList<Cell*> &row, int rowNum, QList<Cell*> &column, int columnNum, QList<Cell*> &block, char value = UNKNOWN);

	void setDomain(QSet<char> newDomain);
	const QSet<char> domain() const;
	void appendToDomain(QSet<char> diff);

	// returns a set of cells dependent on this one
	QList<Cell*> dependentCells() const;

	bool setValue(char value, bool given = false);
	char value() const;
	bool isGiven() const;
	int rowIndex() const;
	int columnIndex() const;

	QSet<char> restrictDomain();
	void broadenDomain();

private:
	QList<Cell*>& row;
	QList<Cell*>& column;
	QList<Cell*>& block;

	QPoint location;

	char _value;
	bool given;
	QSet<char> _domain;
};

#endif // CELL_H
