#ifndef PUZZLEVIEW_H
#define PUZZLEVIEW_H

#include <QTableView>
#include <QDebug>
#include <QSizePolicy>
#include <QHeaderView>
#include <QResizeEvent>

#include "metatableitemdelegate.h"
#include "logger.h"

class PuzzleView : public QTableView
{
	Q_OBJECT
public:
	explicit PuzzleView(QWidget *parent = 0);
	QSize sizeHint() const;

signals:
	
public slots:
	void updateContents();

protected:
	int sizeHintForRow(int row) const;
	int sizeHintForColumn(int column) const;
	void resizeEvent(QResizeEvent *event);
};

#endif // PUZZLEVIEW_H
