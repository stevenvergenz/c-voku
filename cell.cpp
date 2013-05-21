#include "cell.h"

Cell::Cell(QSet<Cell&>& row, QSet<Cell&>& column, QSet<Cell&>& block, char value = Cell::UNKNOWN)
: row(row), column(column), block(block), value(value)
{
	if( value != UNKNOWN ){
		given = true;
	}
}

int Cell::domainSize(){
	return domain.size();
}

bool Cell::inDomain(char value){
	return domain.contains(value);
}

bool Cell::dependentCells(){
	return row+column+block;
}

bool Cell::setValue(char value)
{
	if( domain.contains(value) )
	{
		this->value = value;
		domain.clear();
		return true;
	}
	else
		return false;
}
