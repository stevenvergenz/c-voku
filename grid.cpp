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
Grid::Grid(int size) : _size(size)
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
			int b = blockSize*(r%blockSize) + c%blockSize;

			// create new cell
			Cell* newCell = new Cell(rows[r], columns[c], blocks[b], Cell::UNKNOWN);
			newCell->setDomain(fullDomain());

			cells.append(newCell);

			rows[r].append(newCell);
			columns[c].append(newCell);
			blocks[b].append(newCell);
		}
	}
}


/*
 * Destructor, frees all cells
 */
Grid::~Grid()
{
	for( int i=0; i<cells.size(); i++ ){
		delete cells[i];
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

	// the whole file has been parsed, return
	return newGrid;
}


/******************************************
 * General grid accessors
 ******************************************/

int Grid::size(){
	return this->_size;
}

Cell* Grid::cellAt(int row, int column) const
{
	return rows[row][column];
}

QSet<char> Grid::fullDomain()
{
	QSet<char> retDomain;
	for( char i=0; i<_size; i++ ){
		retDomain << i;
	}
	return retDomain;
}
