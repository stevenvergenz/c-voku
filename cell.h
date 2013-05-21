#ifndef CELL_H
#define CELL_H

#include <QSet>

class Cell
{
public:

	// represents an unknown value for the cell
	static const char UNKNOWN = -1;

	// constructor, fixes position and value of cell
	Cell(QList<Cell*> &row, QList<Cell*> &column, QList<Cell*> &block, char value = UNKNOWN);

	// returns the number of potential values in the cell
	int domainSize() const;

	// returns whether a given value is in the domain
	bool inDomain(char value) const;

	// returns a set of cells dependent on this one
	QList<Cell*> dependentCells() const;

	// attempts to set value of cell, returns false if invalid
	bool setValue(char value, bool given = false);
	char value() const;
	bool isGiven() const;

	bool operator==(const Cell& other) const;

private:
	QList<Cell*>& row;
	QList<Cell*>& column;
	QList<Cell*>& block;

	char _value;
	bool given;
	QSet<char> domain;
};

uint qHash(Cell c);

#endif // CELL_H
