#include "grid.h"

const QString Grid::alphabet() const
{
	if( _size == 4 ){
		return "1234";
	}
	else if( _size == 9 ){
		return "123456789";
	}
	else if( _size == 16 ){
		return "0123456789ABCDEF";
	}
	else if( _size == 25 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXY";
	}
	else if( _size == 36 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	}
	else if( _size == 49 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx";
	}
	else if( _size == 64 ){
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	}
	else {
		return "";
	}
}

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
		throw new std::invalid_argument("Grid size is not a square");
	}

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

Grid::~Grid()
{
	for( int i=0; i<cells.size(); i++ ){
		delete cells[i];
	}
}

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
