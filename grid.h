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

private:

	int size;
	QVector<Cell*> cells;
	QSet<Cell&>* rows;
	QSet<Cell&>* columns;
	QSet<Cell&>* blocks;


};

#endif // GRID_H
