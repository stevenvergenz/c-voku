#ifndef GRID_H
#define GRID_H

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSet>
#include <QHash>
#include <QQueue>
#include <QStack>
#include <stdexcept>

#include "cell.h"
#include "exception.h"
#include "logger.h"
#include "historyframe.h"


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

	Cell* getSafestCell() const;
	QQueue<char> getSafestValues(Cell* target);

	QHash<Cell*, QSet<char> > fixArcConsistency(Cell* dirtyCell = nullptr);
	void unfixArcConsistency( QHash<Cell*, QSet<char> > diff );
	QHash<Cell*, QSet<char> > broadenDomains(Cell* unsetCell);

	QList<Cell *> solve(bool guess = false);

	HistoryFrame* undo(Cell *rewindTarget = nullptr);
	bool setCellAndUpdate(Cell* cell, char newValue, QQueue<char> otherOptions = QQueue<char>());

private:

	int _size;
	QList<Cell*> cells;
	QList<QList<Cell*> > rows;
	QList<QList<Cell*> > columns;
	QList<QList<Cell*> > blocks;

	QStack<HistoryFrame*> history;
};

#endif // GRID_H
