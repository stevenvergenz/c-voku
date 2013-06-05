#include "puzzleview.h"

PuzzleView::PuzzleView(QWidget *parent) : QTableView(parent)
{
	QSizePolicy* policy = new QSizePolicy();
	policy->setHeightForWidth(true);
	setSizePolicy(*policy);
	setFocusPolicy(Qt::NoFocus);

	this->setItemDelegate(new MetatableItemDelegate());

#if QT_VERSION > 0x050000
	horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
	horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif
	horizontalHeader()->setMinimumSectionSize(5);
	verticalHeader()->setMinimumSectionSize(5);
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

void PuzzleView::updateContents()
{
	// calculate size
	double horizontalSize = (double)geometry().width()/horizontalHeader()->count();
	double verticalSize = (double)geometry().height()/verticalHeader()->count();
	double horizRollingSize = 0.0f, vertRollingSize = 0.0f;

	// apply size to all cells
	for( int i=0; i<horizontalHeader()->count(); i++ )
	{
		horizRollingSize += horizontalSize;
		vertRollingSize += verticalSize;

		horizontalHeader()->resizeSection(i, (int)horizRollingSize);
		verticalHeader()->resizeSection(i, (int)vertRollingSize);

		horizRollingSize -= (int)horizRollingSize;
		vertRollingSize -= (int)vertRollingSize;
	}
	Logger::log("Grid resized");
}

void PuzzleView::resizeEvent(QResizeEvent *event)
{
	if( model() == nullptr ){
		QWidget::resizeEvent(event);
		return;
	}
	updateContents();
	event->accept();
}
