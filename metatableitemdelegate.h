#ifndef METATABLEITEMDELEGATE_H
#define METATABLEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPen>
#include <QPainter>
#include <QColor>

class MetatableItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit MetatableItemDelegate(QObject *parent = 0);
	
protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	
private:
	QPen heavyPen;
	QPen lightPen;
};

#endif // METATABLEITEMDELEGATE_H
