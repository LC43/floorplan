#include <QtGui>

#include "memoriadescritiva.h"

MemoriaDescritiva::MemoriaDescritiva() {

	textEdit = new QTextEdit;
	setCentralWidget(textEdit);

	createActions();

	createMenus();
	createToolBars();
	createStatusBar();

	readSettings();

	connect(textEdit->document(), SIGNAL(contentsChanged()),
			this, SLOT(documentWasModified()));

	setCurrentFile("");
}

void MemoriaDescritiva::showMemoriaDescritiva() {
		
	this->show();

}


void MemoriaDescritiva::closeEvent(QCloseEvent *event) {
	if (maybeSave()) {
		writeSettings();
		event->accept();
	} else {
		event->ignore();
	}
}
void MemoriaDescritiva::newFile() {
	if (maybeSave()) {
		textEdit->clear();
		setCurrentFile("");
	}
}

void MemoriaDescritiva::open()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}
bool MemoriaDescritiva::save()
{
	if (curFile.isEmpty()) {
		return saveAs();
	} else {
		return saveFile(curFile);
	}
}

bool MemoriaDescritiva::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}
void MemoriaDescritiva::about()
{
	QMessageBox::about(this, tr("Sobre Floor Plan"),
					   trUtf8("O <b>Floorplan</b> é um programa de desenho de plantas arquitectónicas para habitações, "
						  "dando relevância à aprendizagem, ao uso intuitivo das ferramentas disponibilizadas."
						  "Serve também como plataforma de introdução a modelos de desenho assistido por computador "
						  "mais complexos."));
}


void MemoriaDescritiva::documentWasModified()
{
	setWindowModified(textEdit->document()->isModified());
}

void MemoriaDescritiva::createActions()
{
	newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	newAct->setShortcut(tr("Ctrl+N"));
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcut(tr("Ctrl+S"));
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
	cutAct->setShortcut(tr("Ctrl+X"));
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
			"clipboard"));
	connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

	copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
			"clipboard"));
	connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

	pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
	pasteAct->setShortcut(tr("Ctrl+V"));
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
			"selection"));
	connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(textEdit, SIGNAL(copyAvailable(bool)),
			cutAct, SLOT(setEnabled(bool)));
	connect(textEdit, SIGNAL(copyAvailable(bool)),
			copyAct, SLOT(setEnabled(bool)));
}
void MemoriaDescritiva::createMenus() {
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void MemoriaDescritiva::createToolBars() {
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
}

void MemoriaDescritiva::createStatusBar() {
	statusBar()->showMessage(tr("Ready"));
}

void MemoriaDescritiva::readSettings()
{
	QSettings settings("CodePoets", "Floor Plan Memoria Descritiva");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void MemoriaDescritiva::writeSettings()
{
	QSettings settings("CodePoets", "Floor Plan Memoria Descritiva");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}


bool MemoriaDescritiva::maybeSave() {
	if (textEdit->document()->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Floor Plan - Memória Descritiva"),
								   tr("O documento foi alterado.\n"
									   "Quer guardar o documento?"),
			 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

bool MemoriaDescritiva::saveFile(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Floor Plan - Memória Descritiva"),
							 tr("Cannot write file %1:\n%2.")
									 .arg(fileName)
									 .arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << textEdit->toPlainText();
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void MemoriaDescritiva::loadFile(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {

	    QMessageBox::warning(this, trUtf8("Floor Plan - Memória Descritiva"),
				 tr("Cannot read file %1:\n%2.")
				 .arg(fileName)
				 .arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}



void MemoriaDescritiva::setCurrentFile(const QString &fileName){
	curFile = fileName;
	textEdit->document()->setModified(false);
	setWindowModified(false);

	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled.txt";
	else
		shownName = strippedName(curFile);

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(trUtf8("Floor Plan - Memória Descritiva")));
}

QString MemoriaDescritiva::strippedName(const QString &fullFileName) {
	return QFileInfo(fullFileName).fileName();
}
