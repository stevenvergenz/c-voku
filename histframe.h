#ifndef HISTFRAME_H
#define HISTFRAME_H

#include <QQueue>
#include <QHash>
#include <QSet>

#include "cell.h"

class HistFrame
{
public:
	HistFrame();
	HistFrame(Cell* decidingCell, QQueue<char> values);

	QHash<Cell*, char> newValues;
	//QHash<Cell*, QSet<char> > removedDomainValues;
	HistFrame* nextBestFrame;
};

#endif // HISTFRAME_H
