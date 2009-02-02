#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
//#include <QMainWindow>
#include "ui_mainwindow.h"
#include "svglistwidget.h"
#include "dragwidgetgrid.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
			

};

#endif
