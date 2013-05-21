#ifndef CELL_H
#define CELL_H

#include <QSet>

class Cell
{
public:

	// represents an unknown value for the cell
	static const char UNKNOWN = -1;

	// constructor, fixes position and value of cell
	Cell(QSet<Cell &> &row, QSet<Cell &> &column, QSet<Cell &> &block, char value);

	// returns the number of potential values in the cell
	int domainSize();

	// returns whether a given value is in the domain
	bool inDomain(char value);

	// returns a set of cells dependent on this one
	bool dependentCells();

	// attempts to set value of cell, returns false if invalid
	bool setValue(char value, bool given);

private:
	QSet<Cell&>& row;
	QSet<Cell&>& column;
	QSet<Cell&>& block;

	char value;
	bool given;
	QSet<char> domain;
};

#endif // CELL_H
