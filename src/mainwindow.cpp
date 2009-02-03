#include <QtGui>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Floor Plan"));
	setupUi(this);
	SvgListWidget * svg = new SvgListWidget();
	DragWidgetGrid * drag = new DragWidgetGrid();
	SvgArea->setWidget(svg);
	GridArea->setWidget(drag);
    actionImprimir->setShortcut(tr("Ctrl+I"));
	connect(actionImprimir, SIGNAL(triggered()), drag, SLOT(sendToPrinter()));
    actionGuardar_como_imagem->setShortcut(tr("Ctrl+S"));
	connect(actionGuardar_como_imagem, SIGNAL(triggered()),drag, SLOT(saveToFile()));
    actionCopiar->setShortcut(tr("Ctrl+C"));
	connect(actionCopiar, SIGNAL(triggered()),drag, SLOT(copyToClipboard()));
	
	MemoriaDescritiva *mem_desc = new MemoriaDescritiva();
	actionMemoria_Descritiva->setShortcut(tr("Ctrl+D"));
	connect(actionMemoria_Descritiva,SIGNAL(triggered()),mem_desc,SLOT(showMemoriaDescritiva()));
	//connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}



