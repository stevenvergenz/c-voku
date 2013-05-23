#include "cell.h"

QSet<char> Cell::fullDomain;

Cell::Cell(QList<Cell*>& row, QList<Cell*>& column, QList<Cell*>& block, char value)
: row(row), column(column), block(block), _value(value), given(false)
{
	if( value != UNKNOWN ){
		given = true;
	}
}


void Cell::setDomain(QSet<char> newDomain){
	_domain = newDomain;
}

const QSet<char> Cell::domain() const {
	return _domain;
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
	return retList;
}

bool Cell::setValue(char value, bool given)
{
	// only allow assignment if given value is valid
	if( _domain.contains(value) )
	{
		this->_value = value;
		_domain.clear();
		this->given = given;

		return true;
	}
	else if( value == UNKNOWN ){
		this->_value = value;
		_domain.clear();
		foreach( char val, Cell::fullDomain ){
			_domain.insert(val);
		}
		return true;
	}
	else return false;
}

char Cell::value() const {
	return this->_value;
}

bool Cell::isGiven() const {
	return given;
}


QSet<char> Cell::updateDomain()
{
	QList<Cell*> depCells = dependentCells();
	QSet<char> domainLeavings;

	// arc consistency of cell -> dirty
	// for all v1 in cell.domain, there exists v2 in dirty.domain such that v1 != v2
	// if not, remove v1 from cell.domain
	for( auto dep=depCells.constBegin(); dep!=depCells.constEnd(); ++dep )
	{
		Cell* depCell = *dep;
		for( auto val=_domain.begin(); val!=_domain.end(); )
		{
			int invalidEntries = depCell->domain().contains(*val) ? 1 : 0;
			if( depCell->value() == *val || depCell->domain().size()-invalidEntries == 0 ){
				//qDebug() << "Removing a value";
				domainLeavings.insert(*val);
				val = _domain.erase(val);
			}
			else {
				//qDebug() << "NOT removing a value";
				++val;
			}
		}
	}

	return domainLeavings;
}
