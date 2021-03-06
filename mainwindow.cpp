#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow), grid(nullptr), model(nullptr)
{
	ui->setupUi(this);
	CenterFloatLayout* layout = new CenterFloatLayout(ui->frame);
	layout->setContentsMargins(8,8,8,8);
	layout->addWidget(ui->tableView);
	//ui->frame->setLayout(layout);

	// connect the open menu item
	connect( ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()) );
	connect( Logger::instance(), SIGNAL(onMessage(QString)), this, SLOT(printLog(QString)) );

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open sudoku", QString(),
		"Sudoku files (*.sko);;All files (*.*)");

	// make sure they didn't hit cancel
	if( filename.isNull() )
		return;

	// build new grid from file
	Grid* newGrid = nullptr;
	try {
		newGrid = Grid::parse(filename);
	}
	catch( Exception& e ){
		QMessageBox::critical(this, "File error", e.what(), QMessageBox::Ok);
		return;
	}

	GridModel* newGridModel = new GridModel(*newGrid, this);

	// free up the old grid structures
	if( grid != nullptr )
		delete grid;
	if( model != nullptr )
		delete model;

	grid = newGrid;
	model = newGridModel;

	model->setShowDomainColor(ui->action_Color_domains->isChecked());
	ui->action_Color_domains->setEnabled(true);
	ui->action_Fill_in_single_domains->setEnabled(true);
	ui->action_Solve->setEnabled(true);
	ui->action_Undo->setEnabled(true);
	connect( ui->action_Color_domains, SIGNAL(toggled(bool)), model, SLOT(setShowDomainColor(bool)) );
	connect( ui->action_Fill_in_single_domains, SIGNAL(triggered()), model, SLOT(fillSingleDomains()) );
	connect( ui->action_Solve, SIGNAL(triggered()), model, SLOT(solve()) );
	connect( ui->action_Undo, SIGNAL(triggered()), model, SLOT(undo()) );

	ui->tableView->setModel(model);
	ui->tableView->updateContents();
	printLog(filename+" loaded");
}


void MainWindow::printLog(QString msg)
{
	QString formattedMsg = QString("[%1]: %2").arg( QTime::currentTime().toString("hh:mm:ss"), msg );

	ui->txtLog->append(formattedMsg);
}
