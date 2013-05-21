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
	if( role == Qt::DisplayRole ){
		char val = _grid.cellAt(index.row(), index.column())->value();
		if( val == Cell::UNKNOWN )
			return QVariant("");
		else
			return QVariant(val);
	}
	else if( role == Qt::ToolTipRole ){
		return QVariant();
	}
	else if( role == Qt::StatusTipRole ){
		return QVariant();
	}
	else {
		return QVariant();
	}
}

bool GridModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	return _grid.cellAt(index.row(), index.column())->setValue(value.toChar().toAscii());
}

Qt::ItemFlags GridModel::flag(const QModelIndex& index) const
{
	Qt::ItemFlags retFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if( ! _grid.cellAt(index.row(), index.column())->isGiven() ){
		retFlags |= Qt::ItemIsEditable;
	}
}
