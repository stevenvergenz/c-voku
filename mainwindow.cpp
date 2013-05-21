#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	grid = new Grid(9);
	model = new GridModel(*grid, this);
	ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
	delete ui;
}
