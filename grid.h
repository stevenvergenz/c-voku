#ifndef GRID_H
#define GRID_H

#include <QSet>
#include <QVector>
#include <stdexcept>

#include "cell.h"

class Grid
{
public:

	Grid(int size);
	~Grid();

	int size();
	Cell* cellAt(int row, int column) const;
	QSet<char> fullDomain();
	const QString alphabet() const;

private:

	int _size;
	QList<Cell*> cells;
	QList<QList<Cell*> > rows;
	QList<QList<Cell*> > columns;
	QList<QList<Cell*> > blocks;


};

#endif // GRID_H
