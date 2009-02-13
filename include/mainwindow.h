#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "ui_floorplan2.h"
#include "svglistwidget.h"
#include "dragwidgetgrid.h"
#include "memoriadescritiva.h"

class MainWindow : public QMainWindow, private Ui::FloorPlan
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		
	private:
		void createMenus();
		void setCurrentFile(const QString &fileName);
		void updateRecentFileActions();
		QString strippedName(const QString &fullFileName);
		void loadFile(const QString &fileName);
		
		QAction *separatorAction;
		QTextEdit *textEdit;
		QString curFile;
		SvgListWidget * svg;
		DragWidgetGrid * drag;
		
		enum { MaxRecentFiles = 5 };
		QAction *recentFileActions[MaxRecentFiles];

	private slots:
		void open();
		void openRecentFile();
		void sobreFloorplan();
};

#endif
