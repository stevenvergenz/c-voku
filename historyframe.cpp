#include "historyframe.h"

HistoryFrame::HistoryFrame(Cell* targetCell, QQueue<char> options, QHash<Cell *, QSet<char> > changes)
: target(targetCell), domainChanges(changes), optionPriorities(options)
{

}
