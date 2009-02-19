#include <QtGui>

#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Floor Plan"));
	setupUi(this);
	svg = new SvgListWidget();
	drag = new DragWidgetGrid();
	status_escala = new QLabel(this);
	scrollArea->setWidget(svg);
	//scrollArea->ensureWidgetVisible();
	scrollArea->setWidgetResizable(true);
	setCentralWidget(drag);
	assistant = new Assistant;
	createMenus();

}

void MainWindow::createMenus() {
	
	//menuFicheiro
	// TODO: + novo
	// + abrir
	actionAbrir->setShortcut(tr("Ctrl+N"));
	actionAbrir->setStatusTip(tr("Abrir uma planta"));
	connect(actionAbrir, SIGNAL(triggered()), this, SLOT(open()));

    // + sub menu Recentes
	separatorAction = menuFicheiro->addSeparator();
	menuRecentes = new QMenu(menuFicheiro);
	menuRecentes->setObjectName(QString::fromUtf8("menu_Recentes"));

	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActions[i] = new QAction(this);
		recentFileActions[i]->setVisible(false);
		connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}
	for (int i = 0; i < MaxRecentFiles; ++i)
		menuRecentes->addAction(recentFileActions[i]);
	
	updateRecentFileActions();


	// + sair
	actionSair->setShortcut(tr("Ctrl+Q"));
	connect(actionSair, SIGNAL(triggered()), qApp, SLOT(quit()));

	//TODO: + guardar
	
	// menu Operacoes
	// + imprimir
	actionImprimir->setShortcut(tr("Ctrl+I"));
	connect(actionImprimir, SIGNAL(triggered()), drag, SLOT(sendToPrinter()));
	// + guardaar como imagem
	actionGuardar_como_imagem->setShortcut(tr("Ctrl+S"));
	connect(actionGuardar_como_imagem, SIGNAL(triggered()),drag, SLOT(saveToFile()));
	// + copiar para clipboard
	actionCopiar->setShortcut(tr("Ctrl+C"));
	connect(actionCopiar, SIGNAL(triggered()),drag, SLOT(copyToClipboard()));
	// + zooms
	actionAumentar_Zoom->setShortcut(tr("Ctrl++"));
	actionDiminuir_Zoom->setShortcut(tr("Ctrl+-"));

	connect(actionAumentar_Zoom, SIGNAL(triggered()),drag, SLOT(increaseZoom()));
	connect(actionDiminuir_Zoom, SIGNAL(triggered()),drag, SLOT(decreaseZoom()));

	// + abrir novo widget para a memoria descritiva
	MemoriaDescritiva *mem_desc = new MemoriaDescritiva();
	actionMemoria_Descritiva->setShortcut(tr("Ctrl+D"));
	connect(actionMemoria_Descritiva,SIGNAL(triggered()),mem_desc,SLOT(showMemoriaDescritiva()));

	// menu Preferencias
	// + SvgList->toggleViewAction()
	SvgList->toggleViewAction()->setText(QApplication::translate("FloorPlan", "Ver Lista", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
	SvgList->toggleViewAction()->setToolTip(QApplication::translate("FloorPlan", "Mostrar a Lista com os vários objectos", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

#ifndef QT_NO_STATUSTIP
	SvgList->toggleViewAction()->setStatusTip(QApplication::translate("FloorPlan", "Mostrar a Lista com os vários objectos", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP

	SvgList->toggleViewAction()->setShortcut(QApplication::translate("FloorPlan", "Ctrl+L", 0, QApplication::UnicodeUTF8));
	
	menuPre_ferencias->addAction( SvgList->toggleViewAction() );

	// + Dialogo das preferencias
	ConfigDialog *preferencias = new ConfigDialog();
	connect(actionEditar_Preferencias, SIGNAL(triggered()),preferencias,SLOT(showConfigDialog()));
	connect( preferencias , SIGNAL(escalaAlteradaSignal(QString)),this,SLOT(updateEscala(QString)));
	// menu Ajuda
	// + Ajuda
	
	// + Documentacao
	actionDocumenta_o->setShortcut(tr("F1"));
	connect(actionDocumenta_o,SIGNAL(triggered()), this, SLOT(showDocumentation()));
	// + sobre o floor plan
	connect(actionAcerca_de_FloorPlan, SIGNAL(triggered()), this, SLOT(sobreFloorplan()));
	// + sobre o Qt (em ingles)
	connect(actionAcerca_do_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	//Status bar:

	
	status_escala->setText("Escala: " + preferencias->getEscala());
	statusBar()->addPermanentWidget( status_escala );
}

void MainWindow::updateEscala(QString escala){

	status_escala->setText("escala: " + escala );

}

void MainWindow::showDocumentation() {
     assistant->showDocumentation("indice.html");
}
void MainWindow::closeEvent(QCloseEvent *){
     delete assistant;
}

void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
		loadFile(fileName);
}

void MainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}
void MainWindow::loadFile(const QString &filename)
{
	
  if ( filename.isEmpty() ) return ;

  // open the file and check we can read from it
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    statusBar()->showMessage( QString("Failed to open '%1'").arg(filename) );
    return ;
  }

  // open an xml stream reader and load simulation data
  QXmlStreamReader * stream = new QXmlStreamReader( &file );
	
  drag->LoadProject(stream);
	
  file.close();
	
  QApplication::setOverrideCursor(Qt::WaitCursor);
 // textEdit->setPlainText(in.readAll());
  QApplication::restoreOverrideCursor();
  setCurrentFile(filename);
  statusBar()->showMessage(tr("File loaded"), 2000);
  
}
void MainWindow::sobreFloorplan(){
	QMessageBox::about(this, tr("Sobre Floor Plan"),
					   trUtf8("O <b>Floorplan</b> é um programa de desenho de plantas arquitectónicas para habitações, "
							   "dando relevância à aprendizagem, ao uso intuitivo das ferramentas disponibilizadas."
							   "Serve também como plataforma de introdução a modelos de desenho assistido por computador "
							   "mais complexos."));
}

// guarda o nome do ficheiro nas preferencias
void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	if (curFile.isEmpty())
		setWindowTitle(tr("Recent Files"));
	else
		setWindowTitle(tr("%1 - %2").arg(strippedName(curFile))
				.arg(tr("Recent Files")));

	QSettings settings("CodePoets", "Floorplan");
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
			mainWin->updateRecentFileActions();
	}
}
// carrega os ficheiros recentes
void MainWindow::updateRecentFileActions()
{
	QSettings settings("CodePoets", "Floorplan");
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		recentFileActions[i]->setText(text);
		recentFileActions[i]->setData(files[i]);
		recentFileActions[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActions[j]->setVisible(false);

	separatorAction->setVisible(numRecentFiles > 0);
}

 QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

bool  MainWindow::fileSaveAs()
{
  // get user to select filename and location
  QString filename = QFileDialog::getSaveFileName();
  if ( filename.isEmpty() ) return false;

  // open the file and check we can write to it
  QFile file( filename );
  if ( !file.open( QIODevice::WriteOnly ) )
  {
    ( QString("Failed to write to '%1'").arg(filename) );
    return false;
  }

  // open an xml stream writer and write simulation data
  QXmlStreamWriter  * stream = new QXmlStreamWriter( &file );

  drag->SaveProject(stream);

  stream->writeEndDocument();

  // close the file and display useful message
  file.close();
  statusBar()->showMessage( QString("Saved to '%1'").arg(filename) );
  return true;
}
