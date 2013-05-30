#ifndef HISTFRAME_H
#define HISTFRAME_H

#include <QQueue>
#include <QHash>
#include <QSet>

#include "cell.h"

class HistFrame
{
public:
	HistFrame(Cell* decidingCell, QQueue<char> values);

	//void addDomainChanges(QHash<Cell*, QSet<char> > diff);
	void addValue(Cell* cell, char value);

	void revertChanges();
	HistFrame* getNextBestOption();

private:
	QHash<Cell*, char> newValues;
	//QHash<Cell*, QSet<char> > removedDomainValues;

	HistFrame* nextBestFrame;
};

#endif // HISTFRAME_H
