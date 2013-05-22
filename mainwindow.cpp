#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// connect the open menu item
	connect( ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()) );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open sudoku", QString(), "Sudoku files (*.sko)");
}
