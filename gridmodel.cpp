#include "gridmodel.h"

GridModel::GridModel(Grid& grid, QObject *parent) : QAbstractTableModel(parent), _grid(grid)
{

}

int GridModel::rowCount(const QModelIndex& parent) const
{
	return _grid.size();
}

int GridModel::columnCount(const QModelIndex& parent) const
{
	return _grid.size();
}

QVariant GridModel::data(const QModelIndex& index, int role) const
{
	return QVariant( _grid.cellAt(index.row(), index.column())->value() );
}

bool GridModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	return _grid.cellAt(index.row(), index.column())->setValue(value.toChar().toAscii());
}

Qt::ItemFlags GridModel::flag(const QModelIndex& index) const
{
	Qt::ItemFlags retFlags = Qt::ItemIsEnabled;
	if( ! _grid.cellAt(index.row(), index.column())->isGiven() ){
		retFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable;
	}
}
