#include "centerfloatlayout.h"

CenterFloatLayout::CenterFloatLayout(QWidget *parent)
	: QLayout(parent)
{
}

void CenterFloatLayout::~CenterFloatLayout()
{
	QLayoutItem* item;
	while( item = takeAt(0) )
		delete item;
}

void CenterFloatLayout::addItem(QLayoutItem* item)
{
	items.append(item);
}

QSize CenterFloatLayout::sizeHint() const
{
	QSize size;
	for( auto i=items.begin(); i!=items.end(); ++i ){
		QLayoutItem* item = *i;
		size = size.expandedTo(item->sizeHint());
	}
	return size;
}

QSize CenterFloatLayout::minimumSize() const
{
	QSize size;
	for( auto i=items.begin(); i!=items.end(); ++i ){
		QLayoutItem* item = *i;
		size = size.expandedTo(item->minimumSize());
	}
	return size;
}

QLayoutItem* CenterFloatLayout::itemAt(int index) const
{
	return items.at(index);
}

QLayoutItem* CenterFloatLayout::takeAt(int index)
{
	return (index > 0 && index < items.size()) ? items.takeAt(index) : nullptr;
}

void CenterFloatLayout::setGeometry(const QRect& rect)
{
	QLayout::setGeometry(rect);
	int size = qMin(rect.width(), rect.height());
	QSize corner((rect.width()-size)/2, (rect.height()-size)/2, size, size);

	for( auto i=items.begin(); i!=items.end(); ++i )
	{
		QLayoutItem* item = *i;
		item->setGeometry(corner);
	}
}
