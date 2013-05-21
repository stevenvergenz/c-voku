#ifndef CELL_H
#define CELL_H

#include <QSet>

class Cell
{
public:
	static const char UNKNOWN = -1;

	Cell(QSet<Cell&>* row, QSet<Cell&>* column, QSet<Cell&>* block, char value);


private:
	QSet<Cell&>* row;
	QSet<Cell&>* column;
	QSet<Cell&>* block;

	char value;
	bool given;
	QSet<char> domain;
};

#endif // CELL_H
