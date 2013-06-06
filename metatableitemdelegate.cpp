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
	// paint like normal
	this->QStyledItemDelegate::paint(painter, option, index);

	painter->save();

	// figure out where the lines should go
	int gridSize = index.model()->columnCount();
	int blockSize = (int)sqrt((double)gridSize);

	bool horizDivider = index.row() % blockSize == 0 && index.row() != 0;
	bool vertDivider = index.column() % blockSize == 0 && index.column() != 0;

	// paint light lines
	if( !horizDivider ){
		painter->setPen(lightPen);
		painter->drawLine(option.rect.topLeft(), option.rect.topRight());
	}
	if( !vertDivider ){
		painter->setPen(lightPen);
		painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
	}

	// paint heavy lines
	if( horizDivider ){
		painter->setPen(heavyPen);
		painter->drawLine(option.rect.topLeft(), option.rect.topRight());
	}
	if( vertDivider ){
		painter->setPen(heavyPen);
		painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
	}



	painter->restore();

}
