#include "centerfloatlayout.h"

CenterFloatLayout::CenterFloatLayout(QWidget *parent)
	: QLayout(parent)
{
}

CenterFloatLayout::~CenterFloatLayout()
{
	QLayoutItem* item;
	while( (item = takeAt(0)) )
		delete item;
}

void CenterFloatLayout::addItem(QLayoutItem* item)
{
	items.append(item);
}

int CenterFloatLayout::count() const
{
	return items.count();
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
	return (index > 0 && index < items.size()) ? items.at(index) : nullptr;
}

QLayoutItem* CenterFloatLayout::takeAt(int index)
{
	return (index > 0 && index < items.size()) ? items.takeAt(index) : nullptr;
}

void CenterFloatLayout::setGeometry(const QRect& rect)
{
	QLayout::setGeometry(rect);
	int size = qMin(rect.width() - contentsMargins().left() - contentsMargins().right(),
					rect.height() - contentsMargins().top() - contentsMargins().bottom());
	QSize corner((rect.width()-size)/2, (rect.height()-size)/2);

	for( auto i=items.begin(); i!=items.end(); ++i )
	{
		QLayoutItem* item = *i;
		item->setGeometry( QRect(rect.x()+corner.width(), rect.y()+corner.height(), size, size) );
	}
}
