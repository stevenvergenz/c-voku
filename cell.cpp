#include "cell.h"

Cell::Cell(QList<Cell*>& row, QList<Cell*>& column, QList<Cell*>& block, char value)
: row(row), column(column), block(block), _value(value), given(false)
{
	if( value != UNKNOWN ){
		given = true;
	}
}

int Cell::domainSize() const {
	return domain.size();
}

bool Cell::inDomain(char value) const {
	return domain.contains(value);
}

QList<Cell*> Cell::dependentCells() const
{
	QList<Cell*> retList;
	for( auto i = row.begin(); i != row.end(); ++i ){
		if( this != *i && !retList.contains(*i) ){
			retList.append(*i);
		}
	}
	for( auto i = column.begin(); i != column.end(); ++i ){
		if( this != *i && !retList.contains(*i) ){
			retList.append(*i);
		}
	}
	for( auto i = block.begin(); i != block.end(); ++i ){
		if( this != *i && !retList.contains(*i) ){
			retList.append(*i);
		}
	}
}

bool Cell::setValue(char value, bool given)
{
	// only allow assignment if given value is valid
	if( domain.contains(value) )
	{
		this->_value = value;
		domain.clear();
		this->given = given;

		return true;
	}
	else
		return false;
}

char Cell::value() const {
	return this->_value;
}

bool Cell::isGiven() const {
	return given;
}

bool Cell::operator==(const Cell& other) const {
	return _value == other.value();
}
