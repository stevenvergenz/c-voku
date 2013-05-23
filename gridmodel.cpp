#include "gridmodel.h"

GridModel::GridModel(Grid& grid, QObject *parent) : QAbstractTableModel(parent), _grid(grid)
{
	givenFont.setWeight( QFont::Bold );
	if( grid.size() > 9 ){
		givenFont.setPointSize(12);
		normalFont.setPointSize(12);
	}
	else {
		givenFont.setPointSize(14);
		normalFont.setPointSize(14);
	}
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
		case Qt::EditRole:
		{
			if( subject->value() == Cell::UNKNOWN )
				return QVariant("");
			else {
				QChar value = _grid.alphabet().at(subject->value());
				return QVariant(value);
			}
			break;
		}

		// sets the cell alignment
		case Qt::TextAlignmentRole:
		{
			return QVariant(Qt::AlignCenter);
			break;
		}
		case Qt::ToolTipRole:
		case Qt::StatusTipRole:
		{
			QList<char> d = subject->domain().toList();
			QString ret = "";
			qSort(d);

			if( d.isEmpty() ){
				if( subject->value() != Cell::UNKNOWN ){
					return QVariant();
				}
				else {
					return QVariant("No domain!");
				}
			}
			else {
				ret += _grid.alphabet().at(d.takeFirst());
				for( auto i=d.begin(); i!=d.end(); ++i ){
					ret += "/";
					ret += _grid.alphabet().at(*i);
				}
				return QVariant(ret);
			}
			break;
		}

		case Qt::FontRole:
		{
			if( subject->isGiven() ){
				return QVariant(givenFont);
			}
			else {
				return QVariant(normalFont);
			}
			break;
		}
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
	if( role == Qt::EditRole )
	{
		Cell* subject = _grid.cellAt(index.row(), index.column());
		if( !value.toString().trimmed().isEmpty() ){
			char val = _grid.alphabet().indexOf( value.toString() );
			if( subject->setValue(val) ){
				emit dataChanged(index, index);
				return true;
			}
			else return false;
		}
		else {
			if( subject->setValue(Cell::UNKNOWN) ){
				emit dataChanged(index,index);
				return true;
			}
			else return false;
		}
	}
	else return false;
}

Qt::ItemFlags GridModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags retFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if( ! _grid.cellAt(index.row(), index.column())->isGiven() ){
		retFlags |= Qt::ItemIsEditable;
	}

	return retFlags;
}
