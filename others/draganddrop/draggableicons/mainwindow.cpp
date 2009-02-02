#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	SvgListWidget * svg = new SvgListWidget();
	DragWidgetGrid * drag = new DragWidgetGrid();
	SvgArea->setWidget(svg);
	GridArea->setWidget(drag);
    actionImprimir->setShortcut(tr("Ctrl+I"));
	connect(actionImprimir, SIGNAL(triggered()), this, SLOT(drag->sendToPrinter()));
    actionGuardar_como_imagem->setShortcut(tr("Ctrl+S"));
	connect(actionGuardar_como_imagem, SIGNAL(triggered()), this, SLOT(drag->saveToFile()));
    actionCopiar->setShortcut(tr("Ctrl+C"));
	connect(actionCopiar, SIGNAL(triggered()), this, SLOT(drag->copyToClipboard()));

	//connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
