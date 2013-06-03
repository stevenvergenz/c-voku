#include "historyframe.h"

HistoryFrame::HistoryFrame(Cell* targetCell, QHash<Cell *, QSet<char> > changes, QQueue<char> options)
: target(targetCell), domainChanges(changes), optionPriorities(options)
{

}
