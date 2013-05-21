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
	Cell* subject = _grid.cellAt(index.row(), index.column());
	switch(role)
	{
	// sets the actual content of the cell
	case Qt::DisplayRole:
		if( subject->value() == Cell::UNKNOWN )
			return QVariant("");
		else
			return QVariant(subject->value());
		break;

	// sets the cell alignment
	case Qt::TextAlignmentRole:
		return QVariant(Qt::AlignCenter);
		break;

	case Qt::ToolTipRole:
		break;
	case Qt::StatusTipRole:
		break;
	case Qt::FontRole:
		break;
	case Qt::BackgroundRole:
		break;
	case Qt::ForegroundRole:
		break;
	default:
		return QVariant();
		break;
	}
	return QVariant();
}

bool GridModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	qDebug() << "setData role: " << role;
	if( role == Qt::EditRole )
	{
		if( _grid.cellAt(index.row(), index.column())->setValue(value.toChar().toAscii()) ){
			emit dataChanged(index, index);
			return true;
		}
		else return false;
	}
	else return false;
}

Qt::ItemFlags GridModel::flag(const QModelIndex& index) const
{
	Qt::ItemFlags retFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if( ! _grid.cellAt(index.row(), index.column())->isGiven() ){
		retFlags |= Qt::ItemIsEditable;
	}
	return retFlags;
}
