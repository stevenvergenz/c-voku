#ifndef HISTORYFRAME_H
#define HISTORYFRAME_H

#include <QSet>
#include <QQueue>
#include <QHash>

#include "cell.h"


class HistoryFrame
{
public:
	HistoryFrame(Cell* targetCell, QHash<Cell*, QSet<char> > changes, QQueue<char> options);
	Cell* target;
	QHash<Cell*, QSet<char> > domainChanges;
	QQueue<char> optionPriorities;
};

#endif // HISTORYFRAME_H
