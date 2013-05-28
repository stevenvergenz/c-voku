#include "puzzleview.h"

PuzzleView::PuzzleView(QWidget *parent) : QTableView(parent)
{
	QSizePolicy* policy = new QSizePolicy();
	policy->setHeightForWidth(true);
	setSizePolicy(*policy);
	setFocusPolicy(Qt::NoFocus);

	this->setItemDelegate(new MetatableItemDelegate());

	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

int PuzzleView::heightForWidth(int w) const {
	return w;
}

QSize PuzzleView::sizeHint() const {
	if( model() != nullptr )
		return QSize(20*model()->columnCount(),20*model()->rowCount());
	else
		return QSize(400,400);
}

int PuzzleView::sizeHintForRow(int row) const {
	return height()/model()->rowCount();
}

int PuzzleView::sizeHintForColumn(int column) const {
	return width()/model()->columnCount();
}
