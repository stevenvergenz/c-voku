#include "histframe.h"

HistFrame::HistFrame(Cell* decidingCell, QQueue<char> values)
	: nextBestFrame(nullptr)
{
	if( decidingCell != nullptr ){
		char value = values.dequeue();
		newValues.insert(decidingCell, value);

		if( values.size() > 0 ){
			nextBestFrame = new HistFrame(decidingCell, values);
		}
	}
}

/*void HistFrame::addDomainChanges(QHash<Cell*, QSet<char> > diff)
{
	for( auto i=diff.constBegin(); i!=diff.constEnd(); ++i ){
		removedDomainValues[i.key()] += i.value();
	}
}*/

void HistFrame::addValue(Cell* cell, char value)
{
	newValues.insert(cell, value);
}

void HistFrame::revertChanges()
{
	/*for( auto i=removedDomainValues.constBegin(); i!=removedDomainValues.constEnd(); ++i ){
		Cell* changedCell = i.key();
		changedCell->appendToDomain(i.value());
	}*/

	for( auto i=newValues.begin(); i!=newValues.end(); ++i ){
		Cell* changedCell = i.key();
		changedCell->setValue(Cell::UNKNOWN);
	}
}

HistFrame* HistFrame::getNextBestOption()
{
	return nextBestFrame;
}
