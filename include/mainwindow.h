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
};

#endif
