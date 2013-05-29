#include "gridmodel.h"

GridModel::GridModel(Grid& grid, QObject *parent) : QAbstractTableModel(parent), _grid(grid)
{
	showDomainColor = false;
	givenFont.setWeight( QFont::Bold );
	if( _grid.size() > 9 ){
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
		case Qt::EditRole:
		case Qt::DisplayRole:
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
		{
			if( showDomainColor && subject->value() == Cell::UNKNOWN ){
				if( subject->domain().size() > 0 ){
					double restrictionRatio = (double)subject->domain().size()/subject->fullDomain.size();
					//QColor bgColor(55+200*restrictionRatio, 255, 55+200*restrictionRatio);
					QColor bgColor = QColor::fromHsv(255*restrictionRatio, 100, 255);
					return QVariant(bgColor);
				}
				else {
					return QVariant( QColor("red") );
				}
			}
			break;
		}
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
		char val = Cell::UNKNOWN;
		if( !value.toString().trimmed().isEmpty() ){
			val = _grid.alphabet().indexOf( value.toString() );
		}

		if( subject->setValue(val) )
		{
			QHash<Cell*, QSet<char> > diff;
			if( val == Cell::UNKNOWN )
				diff = _grid.broadenDomains(subject);
			else
				diff = _grid.fixArcConsistency(subject);

			// calculate change in table space
			int minRow=_grid.size(), minCol=_grid.size(), maxRow=-1, maxCol=-1;
			for( auto i=diff.keys().constBegin(); i!=diff.keys().constEnd(); ++i ){
				Cell* cell = *i;
				if( cell->rowIndex() > maxRow ) maxRow = cell->rowIndex();
				if( cell->rowIndex() < minRow ) minRow = cell->rowIndex();
				if( cell->columnIndex() > maxCol ) maxCol = cell->columnIndex();
				if( cell->rowIndex() < minCol ) minCol = cell->columnIndex();
			}

			emit dataChanged( createIndex(minRow, minCol),
							  createIndex(maxRow, maxCol), {Qt::DisplayRole, Qt::BackgroundRole} );
			return true;
		}
		else return false;
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

void GridModel::setShowDomainColor(bool value)
{
	showDomainColor = value;
	emit dataChanged( createIndex(0,0), createIndex(rowCount()-1, columnCount()-1), {Qt::BackgroundRole} );
}
