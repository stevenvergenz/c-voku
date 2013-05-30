#include "grid.h"

/*
 * Return the valid alphabet for the current size
 */
const QString Grid::alphabet(int base)
{
	if( base == 4 ){
		return "1234";
	}
	else if( base == 9 ){
		return "123456789";
	}
	else if( base == 16 ){
		return "0123456789ABCDEF";
	}
	else if( base == 25 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXY";
	}
	else if( base == 36 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	}
	else if( base == 49 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx";
	}
	else if( base == 64 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	}
	else {
		return "";
	}
}

const QString Grid::alphabet() const {
	return Grid::alphabet(_size);
}


/*
 * General constructor, takes size and populates a grid with empty cells
 */
Grid::Grid(int size) : QObject(), _size(size)
{
	// initialize the groups to be the appropriate size
	for( int i=0; i<_size; i++ )
	{
		rows.append( QList<Cell*>() );
		columns.append( QList<Cell*>() );
		blocks.append( QList<Cell*>() );
	}

	// calculate block size, make sure it's square
	double actualSqrt = sqrt((double)size);
	int blockSize = qRound( actualSqrt );
	if( abs((double)blockSize - actualSqrt) > 0.01 ){
		throw Exception("Grid size is not a square");
	}

	Cell::fullDomain = fullDomain();

	// start initializing cells
	for( int r=0; r<size; r++ )
	{
		for( int c=0; c<size; c++ )
		{
			// calculate the block of the current cell
			int b = blockSize*(r/blockSize) + c/blockSize;

			// create new cell
			Cell* newCell = new Cell(rows[r], r, columns[c], c, blocks[b], Cell::UNKNOWN);
			newCell->setDomain(fullDomain());

			cells.append(newCell);
			rows[r].append(newCell);
			columns[c].append(newCell);
			blocks[b].append(newCell);

			connect( newCell, SIGNAL(valueChanged()), &mapper, SLOT(map()) );
			mapper.setMapping( newCell, newCell );
		}
	}

	history.push( new HistFrame() );
}


/*
 * Destructor, frees all cells
 */
Grid::~Grid()
{
	for( int i=0; i<cells.size(); i++ ){
		delete cells[i];
	}
	while( history.size() > 0 ){
		delete history.pop();
	}
}


/*
 * Parses a puzzle file into a Grid object
 */
Grid* Grid::parse(const QString filename)
{
	// eventual return value
	Grid* newGrid = nullptr;

	// prepare the file for reading
	QFile file(filename);
	if( !file.open(QFile::ReadOnly) ){
		throw Exception("Could not open file for reading");
	}

	// initialize the stream
	QTextStream ifp(&file);

	// read the contents of the file into a buffer
	QStringList buffer;
	while( !ifp.atEnd() ){
		QString temp = ifp.readLine();
		if( !temp.isEmpty() ){
			buffer << temp;
		}
	}
	file.close();

	// detect size and check for validity
	int length = buffer.length();
	if( length != buffer[0].length() ){
		throw Exception("Input not of equal dimensions");
	}

	double dBase = (sqrt(5+4*length)-1)/2;
	dBase *= dBase;
	int base = qRound(dBase);
	double dBlockBase = sqrt((double)base);
	int blockBase = qRound(dBlockBase);

	if( abs(dBase-base)>0.01 || abs(dBlockBase-blockBase)>0.01 || Grid::alphabet(base) == "" ){
		throw Exception("Input not in a valid base");
	}

	// now that we're sure the size is correct, initialize the grid
	newGrid = new Grid(base);
	QString alpha = newGrid->alphabet();

	// loop over each row and column
	for( int r=0; r<newGrid->size(); r++ ){
		for( int c=0; c<newGrid->size(); c++ ){

			// calculate the input row/column for each cell
			int rf = r + r/blockBase;
			int cf = c + c/blockBase;

			QChar symbol = buffer[rf][cf];

			// check if it's a valid symbol in the appropriate base
			if( alpha.indexOf(symbol) != -1 ){
				bool valid = newGrid->cellAt(r,c)->setValue(alpha.indexOf(symbol), true);
				if( !valid ){
					delete newGrid;
					throw Exception(
						QString("Input contains illegal placement at (%1,%2)").arg(r,c)
					);
				}
			}

			// if it's not a symbol, is it a space?
			else if( symbol == ' ' ){
				// ain't no problem here, cells already initialized to unknown
				continue;
			}

			// anything else, and we've got a problem
			else {
				delete newGrid;
				throw Exception( QString("Invalid symbol '%1' at (%2,%3)")
					.arg(symbol, QString::number(rf), QString::number(cf)) );
			}

		}// column
	}// row

	connect( &(newGrid->mapper), SIGNAL(mapped(QObject*)), newGrid, SLOT(updateHistory(QObject*)) );

	// the whole file has been parsed, now update domains and return
	auto diff = newGrid->fixArcConsistency();

	return newGrid;
}


/******************************************
 * General grid accessors
 ******************************************/

int Grid::size() const{
	return this->_size;
}

Cell* Grid::cellAt(int row, int column) const
{
	return rows[row][column];
}

QSet<char> Grid::fullDomain() const
{
	QSet<char> retDomain;
	for( char i=0; i<_size; i++ ){
		retDomain << i;
	}
	return retDomain;
}

void Grid::updateHistory(QObject *cell){
	Cell* c = (Cell*)cell;
	history.top()->newValues.insert(c, c->value());
	Logger::log("Adding value to history");
}

void Grid::setCheckpoint(){
	Logger::log("Setting checkpoint");
	history.push( new HistFrame() );
}

void Grid::restoreCheckpoint()
{
	HistFrame* hist;
	if( history.size() > 1 )
		hist = history.pop();
	else
		hist = history.top();

	Logger::log( QString("Undoing the %1 changes since last checkpoint").arg(hist->newValues.size()) );

	for( auto i=hist->newValues.begin(); i!=hist->newValues.end(); ++i ){
		Cell* cell = i.key();
		cell->setValue(Cell::UNKNOWN);
		broadenDomains(cell);
	}

	if( !history.contains(hist) )
		delete hist;


}

/*******************************************
 * Fix Arc Consistency
 *******************************************/

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
	QSet<char> originalDomain = target->domain();

	// list of potential value/effect value pairs
	QList<QPair<char,int> > effectValues;

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

		// insert in place
		bool inserted = false;
		for( auto e=effectValues.begin(); e!=effectValues.end(); ++e ){
			QPair<char,int> p = *e;
			if( p.second > counter ){
				effectValues.insert(e, QPair<char,int>(*i, counter));
				inserted = true;
				break;
			}
		}
		if( !inserted )
			effectValues.append( QPair<char,int>(*i, counter) );

		// reset grid to pre-check levels
		unfixArcConsistency(diff);
	}

	// reset cell to pre-value check
	target->setValue(Cell::UNKNOWN);
	target->setDomain(originalDomain);

	// pop everything to a queue
	QQueue<char> priorities;
	for( auto i=effectValues.begin(); i!=effectValues.end(); ++i ){
		QPair<char, int> p = *i;
		priorities.enqueue( p.first );
	}

	return priorities;
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
			target->setValue( target->domain().values()[0] );
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
			QQueue<char> priorities = getSafestValues(target);
			Logger::log( QString("Safest value for (%1,%2) is %3")
				.arg(QString::number(target->rowIndex()), QString::number(target->columnIndex()),
					alphabet().at(priorities.head())
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
