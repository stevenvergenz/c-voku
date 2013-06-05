#ifndef CENTERFLOATLAYOUT_H
#define CENTERFLOATLAYOUT_H

#include <QLayout>
#include <QList>

class CenterFloatLayout : public QLayout
{
	Q_OBJECT
public:
	explicit CenterFloatLayout(QWidget *parent = 0);
	~CenterFloatLayout();

	void addItem(QLayoutItem* item);
	QSize sizeHint() const;
	QSize minimumSize() const;
	QLayoutItem* itemAt(int index) const;
	QLayoutItem* takeAt(int index);
	void setGeometry(const QRect& rect);

signals:
	
public slots:
	
private:
	QList<QLayoutItem*> items;
};

#endif // CENTERFLOATLAYOUT_H
