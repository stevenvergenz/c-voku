#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "grid.h"
#include "gridmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void openFile();
	
private:
	Ui::MainWindow *ui;
	Grid* grid;
	GridModel* model;
};

#endif // MAINWINDOW_H
