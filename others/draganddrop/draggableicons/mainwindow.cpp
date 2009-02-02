#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	SvgArea->setWidget(new SvgListWidget);
	
	//connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
