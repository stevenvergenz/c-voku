#include "puzzleview.h"

PuzzleView::PuzzleView(QWidget *parent) : QTableView(parent)
{
	QSizePolicy* policy = new QSizePolicy();
	policy->setHeightForWidth(true);
	setSizePolicy(*policy);
	qDebug() << "Size policy: " << sizePolicy().hasHeightForWidth() << endl;
}

int PuzzleView::heightForWidth(int w) const {
	return w;
}

QSize PuzzleView::sizeHint() const {
	return QSize(400,400);
}

int PuzzleView::sizeHintForRow(int row) const {
	return height()/model()->rowCount();
}

int PuzzleView::sizeHintForColumn(int column) const {
	return width()/model()->columnCount();
}