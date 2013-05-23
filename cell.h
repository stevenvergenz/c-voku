#ifndef CELL_H
#define CELL_H

#include <QSet>
#include <QDebug>

class Cell
{
public:

	static const char UNKNOWN = -1;
	static QSet<char> fullDomain;

	Cell(QList<Cell*> &row, QList<Cell*> &column, QList<Cell*> &block, char value = UNKNOWN);

	void setDomain(QSet<char> newDomain);
	const QSet<char> domain() const;

	// returns a set of cells dependent on this one
	QList<Cell*> dependentCells() const;

	bool setValue(char value, bool given = false);
	char value() const;
	bool isGiven() const;

	QSet<char> updateDomain();

private:
	QList<Cell*>& row;
	QList<Cell*>& column;
	QList<Cell*>& block;

	char _value;
	bool given;
	QSet<char> _domain;
};

#endif // CELL_H
