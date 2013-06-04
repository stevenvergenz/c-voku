#ifndef HISTORYFRAME_H
#define HISTORYFRAME_H

#include <QSet>
#include <QQueue>
#include <QHash>

#include "cell.h"


class HistoryFrame
{
public:
	HistoryFrame(
		Cell* targetCell = nullptr,
		QQueue<char> options = QQueue<char>(),
		QHash<Cell*, QSet<char> > changes = QHash<Cell*,QSet<char> >()
	);

	Cell* target;
	QHash<Cell*, QSet<char> > domainChanges;
	QQueue<char> optionPriorities;
};

#endif // HISTORYFRAME_H
