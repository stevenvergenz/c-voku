#include "grid.h"


QHash<Cell*, QSet<char> > Grid::fixArcConsistency(Cell* dirtyCell)
{
	QHash<Cell*, QSet<char> > changes;
	QQueue<Cell*> dirtyCells;

	// initialize the set of cells to check
	// if no dirty cell specified, assume all are dirty
	if( dirtyCell != nullptr ){
		auto depCells = dirtyCell->dependentCells();
		for( auto dep=depCells.constBegin(); dep!=depCells.constEnd(); ++dep )
		{
			dirtyCells.enqueue(*dep);
		}
	}
	else {
		for( int i=0; i<cells.count(); i++ ){
			dirtyCells.enqueue( cells.at(i) );
		}
	}

	// loop until there are no more dirty cells
	while( !dirtyCells.empty() )
	{
		// update domain of dirty cell
		Cell* cell = dirtyCells.dequeue();
		QSet<char> cellChanges = cell->restrictDomain();

		// if the domain changes
		if( !cellChanges.empty() )
		{
			// add changes to the running diff
			changes[cell] += cellChanges;

			// add dependents to queue
			auto depCells = cell->dependentCells();
			for( auto dep=depCells.constBegin(); dep!=depCells.constEnd(); ++dep )
			{
				dirtyCells.enqueue(*dep);
			}
		}
		// else nothing to be done
	}
	return changes;
}

void Grid::unfixArcConsistency(QHash<Cell*, QSet<char> > diff)
{
	for( auto i=diff.begin(); i!=diff.end(); ++i ){
		Cell* cell = i.key();
		cell->appendToDomain(i.value());
	}
}

QHash<Cell*, QSet<char> > Grid::broadenDomains(Cell *unsetCell)
{
	QQueue<Cell*> dirtyCells;
	QHash<Cell*, QSet<char> > diff;

	// initialize set of cells to check
	auto depCells = unsetCell->dependentCells();
	for( auto dep=depCells.constBegin(); dep!=depCells.constEnd(); ++dep ){
		dirtyCells.enqueue(*dep);
	}
	diff.insert(unsetCell, QSet<char>());

	while( !dirtyCells.empty() )
	{
		Cell* dirty = dirtyCells.dequeue();

		// broaden domain and re-restrict to check for changes
		const QSet<char> oldDomain = dirty->domain();
		dirty->broadenDomain();
		dirty->restrictDomain();

		// if there are changes, enqueue dirty cell's dependents
		if( dirty->domain() != oldDomain ){
			diff.insert(dirty, QSet<char>());
			auto depCells = dirty->dependentCells();
			for( auto dep=depCells.constBegin(); dep!=depCells.constEnd(); ++dep ){
				dirtyCells.enqueue(*dep);
			}
		}
	}

	unsetCell->restrictDomain();
	return diff;
}

Cell* Grid::getSafestCell() const
{
	QSet<Cell*> remainingCells;

	// find the most constrained cells (fewest options in domain)
	int minimumDomainSize = 100;
	for( auto cellIter=cells.begin(); cellIter!=cells.end(); ++cellIter )
	{
		Cell* cell = *cellIter;
		if( cell->value() != Cell::UNKNOWN )
			continue;

		if( cell->domain().size() < minimumDomainSize ){
			minimumDomainSize = cell->domain().size();
			remainingCells.clear();
		}
		if( cell->domain().size() == minimumDomainSize ){
			remainingCells.insert(cell);
		}
	}

	// if there are no cells return null, otherwise return a minimum cell
	if( remainingCells.size() == 0 )
		return nullptr;
	else
		return remainingCells.values()[0];
}

// expects an unset cell with at least one option in its domain
QQueue<char> Grid::getSafestValues(Cell *target)
{
	QList<QPair<char, int> > options;
	QSet<char> originalDomain = target->domain();

	// loop over each possible value of target cell
	for( auto i=originalDomain.constBegin(); i!=originalDomain.constEnd(); ++i )
	{
		// assign the value to the cell and rebalance
		target->setValue(*i);
		auto diff = fixArcConsistency(target);
		int counter = 0;
		for( auto j=diff.begin(); j!=diff.end(); ++j ){
			QSet<char> diffDomain = j.value();
			counter += diffDomain.size();
		}

		// insert into the sorted options list
		for( auto j=options.begin(); j!=options.end(); ++j ){
			QPair<char,int> pair = *j;
			if( counter < pair.second ){
				options.insert(j, QPair<char,int>(*i,counter));
			}
		}
		if( !options.contains( QPair<char,int>(*i,counter) ) ){
			options.append( QPair<char,int>(*i,counter) );
		}

		// reset grid to pre-check levels
		unfixArcConsistency(diff);
	}
	target->setValue(Cell::UNKNOWN);
	target->setDomain(originalDomain);

	QQueue<char> retQueue;
	for( auto i=options.constBegin(); i!=options.constEnd(); ++i ){
		QPair<char,int> pair = *i;
		retQueue.enqueue( pair.first );
	}
	return retQueue;
}

QList<Cell*> Grid::solve(bool guess)
{
	// get the safest cell
	QSet<Cell*> diffList;
	Cell* target = getSafestCell();

	while( target != nullptr )
	{
		// the safest cell has no valid numbers, we made a mistake somewhere
		if( target->domain().size() == 0 ){
			Logger::log(QString("Cell at (%1,%2) has no solution!").arg(
				QString::number(target->rowIndex()), QString::number(target->columnIndex())
			));
			break;
		}

		// the safest cell has just one possibility, choose it
		else if( target->domain().size() == 1 )
		{
			// set cell right away, no need to optimize
			auto oldDomain = target->setValueAndGetDomainChanges( target->domain().values()[0] );
			Logger::log(QString("Setting value of (%1,%2) to %3").arg(
				QString::number(target->rowIndex()), QString::number(target->columnIndex()),
				alphabet().at(target->value())
			));
			auto diff = fixArcConsistency(target);

			// add changes
			diffList.insert(target);
			for( auto i=diff.constBegin(); i!=diff.constEnd(); ++i ){
				diffList.insert(i.key());
			}
		}

		// the safest cell has multiple options, guess and continue
		else if( guess ){
			// do make a guess and continue
			auto options = getSafestValues(target);
			char bestValue = options.dequeue();
			Logger::log( QString("Safest value for (%1,%2) is %3")
				.arg(QString::number(target->rowIndex()), QString::number(target->columnIndex()),
					alphabet().at(bestValue)
				)
			);
			break;
		}

		// multiple options, and guessing not allowed, break
		else {
			break;
		}

		// get the next cell
		target = getSafestCell();
	}

	return diffList.values();
}

HistoryFrame* Grid::undo(Cell *rewindTarget)
{
	HistoryFrame* frame = nullptr;

	// only pop off the most recent change
	if( rewindTarget == nullptr )
	{
		// break if no history
		if( history.size() == 0 )
			return nullptr;

		// pop top frame off the stack and restore state
		frame = history.pop();
		frame->target->setValue( Cell::UNKNOWN );
		frame->target->setDomain( frame->domainChanges.value( frame->target ));

		// restore the domains of all other affected cells
		for( auto pair=frame->domainChanges.begin(); pair!=frame->domainChanges.end(); ++pair ){
			Cell* target = pair.key();
			target->appendToDomain( pair.value() );
		}
	}

	else
	{
		// see if the specified frame is in the history
		bool cellFound = false;
		for( auto i=history.begin(); i!=history.end(); ++i ){
			HistoryFrame* temp = *i;
			if( temp->target == rewindTarget ){
				cellFound = true;
				break;
			}
		}
		if( !cellFound )
			return nullptr;

		HistoryFrame* tempFrame = nullptr;
		while( (tempFrame = undo()) != nullptr ){
			if( tempFrame->target == rewindTarget ){
				frame = tempFrame;
				break;
			}
			else {
				delete tempFrame;
			}
		}
	}

	return frame;
}

bool Grid::setCellAndUpdate(Cell *cell, char newValue, QQueue<char> otherOptions)
{
	// if newValue is a valid choice for cell
	QSet<char> oldDomain = cell->setValueAndGetDomainChanges(newValue);
	if( oldDomain.size() > 0 )
	{
		// populate a history frame
		HistoryFrame* frame = new HistoryFrame( cell, otherOptions );
		frame->domainChanges[cell].unite(oldDomain);

		// store domain changes too
		auto changes = fixArcConsistency(cell);
		for( auto i=changes.begin(); i!=changes.end(); ++i ){
			frame->domainChanges[i.key()].unite(i.value());
		}

		// push frame to stack and return
		history.push(frame);
		return true;
	}
	else return false;
}
