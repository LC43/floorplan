#ifndef MEMORIADESCRITIVA_H
#define MEMORIADESCRITIVA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

class MemoriaDescritiva : public QMainWindow {
	Q_OBJECT

	public:
		MemoriaDescritiva();

	protected:
		void closeEvent(QCloseEvent *event);

	public slots:
		void showMemoriaDescritiva();
		
	private slots:
		void newFile();
		void open();
		bool save();
		bool saveAs();
		void sobreMemoria();
		void documentWasModified();
		
		
	private:
		void createActions();
		void createMenus();
		void createToolBars();
		void createStatusBar();
		void readSettings();
		void writeSettings();
		bool maybeSave();
		void loadFile(const QString &fileName);
		bool saveFile(const QString &fileName);
		void setCurrentFile(const QString &fileName);
		QString strippedName(const QString &fullFileName);

		QTextEdit *textEdit;
		QString curFile;

		QMenu *fileMenu;
		QMenu *editMenu;
		QMenu *helpMenu;
		QToolBar *fileToolBar;
		QToolBar *editToolBar;
		QAction *newAct;
		QAction *openAct;
		QAction *saveAct;
		QAction *saveAsAct;
		QAction *exitAct;
		QAction *cutAct;
		QAction *copyAct;
		QAction *pasteAct;
		QAction *aboutAct;
		QAction *aboutQtAct;
};


#endif
