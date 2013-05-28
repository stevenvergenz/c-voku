#ifndef GRIDMODEL_H
#define GRIDMODEL_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QFont>
#include <QColor>

#include "grid.h"

class GridModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	explicit GridModel(Grid& grid, QObject *parent = 0);

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	Qt::ItemFlags flags(const QModelIndex& index) const;

signals:
	
public slots:
	void setShowDomainColor(bool value);

private:
	Grid& _grid;
	QFont givenFont, normalFont;
	bool showDomainColor;
};

#endif // GRIDMODEL_H
