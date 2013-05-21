#include "grid.h"

Grid::Grid(int size) : size(size)
{
	// initialize the groups to be the appropriate size
	rows = new QSet<Cell&>[size];
	columns = new QSet<Cell&>[size];
	blocks = new QSet<Cell&>[size];

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
			Cell* newCell = new Cell(rows[r], columns[c], blocks[b]);
			cells.append(newCell);

			rows[r].insert(*newCell);
			columns[c].insert(*newCell);
			blocks[b].insert(*newCell);
		}
	}
}

Grid::~Grid()
{
	for( int i=0; i<cells.size(); i++ ){
		delete cells[i];
	}

	delete rows;
	delete columns;
	delete blocks;
}
