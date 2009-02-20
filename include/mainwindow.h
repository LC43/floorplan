#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "ui_floorplan2.h"
#include "svglistwidget.h"
#include "dragwidgetgrid.h"
#include "memoriadescritiva.h"
#include "assistant.h"
#include "configdialog.h"

class MainWindow : public QMainWindow, private Ui::FloorPlan
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		void showDocumentation(const QString &file);
		

	private:
		void createMenus();
		void setCurrentFile(const QString &fileName);
		void updateRecentFileActions();
		QString strippedName(const QString &fullFileName);
		void loadFile(const QString &fileName);
		bool  fileSaveAs();
		
		QAction *separatorAction;
		QTextEdit *textEdit;
		QString curFile;
		SvgListWidget * svg;
		DragWidgetGrid * drag;

		void closeEvent(QCloseEvent *);
		enum { MaxRecentFiles = 5 };
		QAction *recentFileActions[MaxRecentFiles];

		Assistant *assistant;

		QLabel *status_escala;
		QLabel *status_selectedItem;
		QLabel *status_modifiers;
		QLabel *status_modifiers_ctrl;
		QLabel *status_modifiers_ctrl_stt;
		QLabel *status_modifiers_meta;
		QLabel *status_modifiers_meta_stt;
		QLabel *status_modifiers_alt;
		QLabel *status_modifiers_alt_stt;

	private slots:
		void open();
		void save();
		void new_plant();
		void openRecentFile();
		void sobreFloorplan();
		void showDocumentation();
		void updateEscala(QString escala);
		void resetSelectedItem();
		void updateSelectedItem(QString name);
		void updateModifierPressed(bool state, int modifier);
};

#endif
