#include "cell.h"

Cell::Cell(QSet<Cell&>* row, QSet<Cell&>* column, QSet<Cell&>* block, char value = Cell::UNKNOWN)
: row(row), column(column), block(block), value(value)
{

}
