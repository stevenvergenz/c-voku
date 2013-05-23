#include "metatableitemdelegate.h"

MetatableItemDelegate::MetatableItemDelegate(QObject *parent)
 : QStyledItemDelegate(parent)
{
	heavyPen.setWidth(1);
	heavyPen.setColor( QColor("black") );

	lightPen.setWidth(1);
	lightPen.setColor( QColor("lightgrey") );
}

void MetatableItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	// figure out where the lines should go
	int gridSize = index.model()->columnCount();
	int blockSize = (int)sqrt((double)gridSize);

	// paint top gridline
	if( index.row() % blockSize == 0 && index.row() != 0 ){
		painter->setPen(heavyPen);
		painter->drawLine(option.rect.topLeft(), option.rect.topRight());
	}
	else {
		painter->setPen(lightPen);
		painter->drawLine(option.rect.topLeft(), option.rect.topRight());
	}

	/*// paint bottom gridline
	if( index.row() % blockSize == blockSize-1 && index.row() != gridSize-1 ){
		painter->setPen(heavyPen);
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
	}
	else {
		painter->setPen(lightPen);
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
	}*/

	// paint left gridline
	if( index.column() % blockSize == 0 && index.column() != 0 ){
		painter->setPen(heavyPen);
		painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
	}
	else {
		painter->setPen(lightPen);
		painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
	}

	/*// paint right gridline
	if( index.column() % blockSize == blockSize-1 && index.column() != gridSize-1 ){
		painter->setPen(heavyPen);
		painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
	}
	else {
		painter->setPen(lightPen);
		painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
	}*/

	painter->restore();
	this->QStyledItemDelegate::paint(painter, option, index);
}
