#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("Floor Plan"));
	setupUi(this);
	svg = new SvgListWidget();
	drag = new DragWidgetGrid();
	drag->setSvgList(svg);
	
	status_escala = new QLabel(this);
	status_selectedItem = new QLabel(this);
	//status_modifiers = new QLabel(this);
	status_modifiers_ctrl = new QLabel(this);
	status_modifiers_ctrl_stt = new QLabel(this);
	status_modifiers_meta = new QLabel(this);
	status_modifiers_meta_stt = new QLabel(this);
	status_modifiers_shift = new QLabel(this);
 	status_modifiers_shift_stt = new QLabel(this);

	scrollArea->setWidget(svg);
	//scrollArea->ensureWidgetVisible();
	scrollArea->setWidgetResizable(true);
	setCentralWidget(drag);
	assistant = new Assistant;

	createMenus();

}

void MainWindow::createMenus() {
	
	//menuFicheiro

	// novo
	action_Novo->setShortcut(tr("Ctrl+N"));
	action_Novo->setStatusTip(tr("Criar nova planta"));
	connect(action_Novo, SIGNAL(triggered()), this, SLOT(new_plant()));
	
	actionGuardar->setShortcut(tr("Ctrl+G"));
	actionGuardar->setStatusTip(tr("Guardar projecto actual"));
	connect(actionGuardar, SIGNAL(triggered()), this, SLOT(save()));
	
	// + abrir
	actionAbrir->setShortcut(tr("Ctrl+A"));
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
	
	for (int i = 0; i < MaxRecentFiles; ++i){
		menuRecentes->addAction(recentFileActions[i]);
		qDebug() << recentFileActions[i]->text() ;
	}
	updateRecentFileActions();


	// + sair
	actionSair->setShortcut(tr("Ctrl+Q"));
	connect(actionSair, SIGNAL(triggered()), qApp, SLOT(quit()));

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

	/********************** **********************************************/
	// + abrir novo widget para a memoria descritiva
	MemoriaDescritiva *mem_desc = new MemoriaDescritiva();
	mem_desc->resize(700,800);
//	TextEdit *mem_desc = new TextEdit();
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
	
	// ++ from drag

	connect(drag, SIGNAL(selectedItemOn(QString)), this, SLOT(updateSelectedItem(QString)));
	connect(drag, SIGNAL(selectedItemOff()), this, SLOT(resetSelectedItem()));

	//TODO: divide the text into their own qlabel
	status_selectedItem->setText(tr("Item seleccionado: ") + tr("Nada"));
	statusBar()->addPermanentWidget( status_selectedItem );

	connect(drag,SIGNAL(modifierKeySignal(bool, int)),this, SLOT(updateModifierPressed(bool, int)));
	//TODO: change *_stt (state) por pixmap colorizado
	QFont font_bold;
	font_bold.setBold(true);


	
	//status_modifiers->setText(tr("Modificadores: "));
	status_modifiers_ctrl->setText(trUtf8("Control (Rotação): "));
	status_modifiers_ctrl->setFont(font_bold);
	status_modifiers_ctrl_stt->setText(tr("Nada"));
#if !defined(Q_OS_WIN)
	status_modifiers_meta->setText(trUtf8("Meta (Inclinação): "));
#else
	status_modifiers_meta->setText(trUtf8("Alt (Inclinação): "));
#endif		
	status_modifiers_meta->setFont(font_bold);
	status_modifiers_meta_stt->setText(tr("Nada"));
	status_modifiers_shift->setText(trUtf8("Shift (Escalar): "));
	status_modifiers_shift->setFont(font_bold);
 	status_modifiers_shift_stt->setText(tr("Nada"));
	//statusBar()->addPermanentWidget( status_modifiers );
	statusBar()->addPermanentWidget( status_modifiers_ctrl );
	statusBar()->addPermanentWidget( status_modifiers_ctrl_stt );
	statusBar()->addPermanentWidget( status_modifiers_meta );
	statusBar()->addPermanentWidget( status_modifiers_meta_stt );
	statusBar()->addPermanentWidget( status_modifiers_shift );
 	statusBar()->addPermanentWidget( status_modifiers_shift_stt );
	

	status_escala->setText(tr("Escala: ") + preferencias->getEscala());
	statusBar()->addPermanentWidget( status_escala );
}
void MainWindow::resetSelectedItem(){
	status_selectedItem->setText(tr("Item: ") + tr("Nada"));
}
void MainWindow::updateSelectedItem(QString name){
	status_selectedItem->setText(tr("Item : ") + name);
	// set to name
}
// mylabel->setPalette(QPalette(Qt::red));

void MainWindow::updateModifierPressed(bool state, int modifier){
	QString estado;
	QPalette palete;
	switch(modifier){
		case Qt::Key_Control:
			if( state ){
				estado = "On";
				palete = QPalette(Qt::green);
			} else {
				estado = tr("Nada");
				palete = QPalette();
			}
			status_modifiers_ctrl_stt->setPalette(palete);
			status_modifiers_ctrl_stt->setText(estado);
			
			break;
		case Qt::Key_Shift:
			if( state ){
				estado = "On";
				palete = QPalette(Qt::green);
			} else {
				estado = tr("Nada");
				palete = QPalette();
			}
			status_modifiers_shift_stt->setPalette(palete);
			status_modifiers_shift_stt->setText(estado);
		break;
		case Qt::Key_Meta:
			if( state ){
				estado = "On";
				palete = QPalette(Qt::green);
			} else {
				estado = tr("Nada");
				palete = QPalette();
			}
			status_modifiers_meta_stt->setPalette(palete);
			status_modifiers_meta_stt->setText(estado);
			break;
		default:
			qDebug() << "oops" ;
	}
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

/****************  about  ******************************/
void MainWindow::sobreFloorplan(){
	QMessageBox::about(this, tr("Sobre Floor Plan"),
					   trUtf8("O <b>Floorplan</b> é um programa de desenho de plantas arquitectónicas para habitações, "
							   "dando relevância à aprendizagem, ao uso intuitivo das ferramentas disponibilizadas."
							   "Serve também como plataforma de introdução a modelos de desenho assistido por computador "
							   "mais complexos."));
}

/* *******************************************************************/
/*          File related     -- nothing else to see                  */
/*********************************************************************/

void MainWindow::new_plant()
{
	QMessageBox diag;
	diag.setInformativeText(tr("Deseja guardar o projecto actual?"));
	//diag.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Discard);
	
	QPushButton *forgetButton = diag.addButton(trUtf8("Esquecer Alterações"),QMessageBox::DestructiveRole);
	QPushButton *saveButton = diag.addButton(tr("Gravar"),QMessageBox::AcceptRole);
	diag.addButton(tr("Cancelar"),QMessageBox::RejectRole);
	//diag.setDefaultButton(QMessageBox::Cancel);
	diag.exec();
	if( diag.clickedButton() == forgetButton )
		drag->resetView();
	else if( diag.clickedButton() == saveButton ) {
			save();
			drag->resetView();
	}

}
/****************  Save ******************************/

void MainWindow::save()
{
	fileSaveAs();
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

	stream->setAutoFormatting( true );
  	stream->writeStartDocument();
	stream->writeStartElement( "floorplan" );

	drag->SaveProject(stream);

	stream->writeEndDocument();

  // close the file and display useful message
	file.close();
	setCurrentFile( filename );
	statusBar()->showMessage( QString("Guardado em '%1'").arg(filename),2000 );
	return true;
}

/****************  Open ******************************/


void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty())
		loadFile(fileName);
}

void MainWindow::loadFile(const QString &filename)
{
	// reset the view otherwise the projects keep piling in.. nice testing as usual..
	drag->resetView();
	if ( filename.isEmpty() ) return ;

  // open the file and check we can read from it
	QFile file( filename );
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		statusBar()->showMessage( QString("Falhou a abrir o ficheiro '%1'").arg(filename) );
		return ;
	}

  // open an xml stream reader and load simulation data
	QXmlStreamReader * stream = new QXmlStreamReader( &file );
		
  while ( !stream->atEnd() )
  {
    stream->readNext();
    if ( stream->isStartElement() )
    {
      if ( stream->name() == "floorplan" )
        drag->LoadProject(stream);
      else
        stream->raiseError( QString("Unrecognised element '%1'").arg(stream->name().toString()) );
    }
  }
		
	 
	
	file.close();
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
 // textEdit->setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();
	setCurrentFile(filename);
	statusBar()->showMessage(tr("Ficheiro Aberto"), 2000);
  
}

/****************  Recents ******************************/

void MainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}
// guarda o nome do ficheiro nas preferencias
void MainWindow::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	if (curFile.isEmpty())
		setWindowTitle(tr("Ficheiros Recentes"));
	else
		setWindowTitle(tr("%1 - %2").arg(strippedName(curFile))
				.arg(tr("Ficheiros Recentes")));

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


