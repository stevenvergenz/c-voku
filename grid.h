#ifndef GRID_H
#define GRID_H

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSet>
#include <QHash>
#include <QQueue>
#include <stdexcept>

#include "cell.h"
#include "exception.h"

class Grid
{
public:

	Grid(int size);
	~Grid();

	static Grid* parse(const QString filename);
	static const QString alphabet(int base);
	int size() const;
	Cell* cellAt(int row, int column) const;
	QSet<char> fullDomain() const;
	const QString alphabet() const;

	QHash<Cell*, QSet<char> > fixArcConsistency(Cell* dirtyCell = nullptr);

private:

	int _size;
	QList<Cell*> cells;
	QList<QList<Cell*> > rows;
	QList<QList<Cell*> > columns;
	QList<QList<Cell*> > blocks;


};

#endif // GRID_H
