#ifndef PUZZLEVIEW_H
#define PUZZLEVIEW_H

#include <QTableView>
#include <QDebug>
#include <QSizePolicy>

class PuzzleView : public QTableView
{
	Q_OBJECT
public:
	explicit PuzzleView(QWidget *parent = 0);
	int heightForWidth(int w) const;
	QSize sizeHint() const;

signals:
	
public slots:
	
protected:
	int sizeHintForRow(int row) const;
	int sizeHintForColumn(int column) const;
};

#endif // PUZZLEVIEW_H
