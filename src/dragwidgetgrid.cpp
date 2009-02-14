/****************************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include <QMessageBox>


#include "dragwidgetgrid.h"

static int max_zoom = 10; //percentage

void DragWidgetGrid::increaseZoom(){
  qreal factor = 1.2;
  scale(factor, factor);
}

void DragWidgetGrid::decreaseZoom(){
    qreal factor = 0.8;
    scale(factor, factor);
}
void DragWidgetGrid::resetZoom(){
    

}

DragWidgetGrid::DragWidgetGrid(QWidget *parent)
: QGraphicsView (parent)
{
    setMinimumSize(800, 800);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    setScene(&scene);
    brush = QBrush (Qt::CrossPattern);
    setBackgroundBrush(brush);
	setDragMode(QGraphicsView::RubberBandDrag);
	selectedItem  = NULL;
	m_drawline=false;
    //original 
    //
}


void DragWidgetGrid::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }

}


void DragWidgetGrid::dragMoveEvent(QDragMoveEvent *event)
{
  
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
  
}

void DragWidgetGrid::wheelEvent(QWheelEvent* event){
  qreal factor = 1.2;
  if (event->delta() < 0)
    factor = 1.0 / factor;
  scale(factor, factor);

}


void DragWidgetGrid::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
 
        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
		

		QGraphicsPixmapItem * item = scene.addPixmap(pixmap);
		
		item->setPos(item->pos() + mapToScene(event->pos() - offset));
		
		item->setCursor(Qt::ClosedHandCursor);
		
    } else {
        event->ignore();
    }
}


void DragWidgetGrid::mousePressEvent(QMouseEvent *event)
{
	QGraphicsItem *item;
	bool selected = false;
	if ((item = itemAt(event->pos())) == NULL) {
         qDebug() << "You didn't click on an item.";
     } else {
		qDebug() << "You clicked on an item.";
		selected=true;
     }
	 
	 if(event->button() == Qt::LeftButton) {
		if(!selected)
			m_drawline = true;
		else
			selectedItem = item;	

		drag_start_pos = event->pos();
	}
	else if(selected){
		QMessageBox diag;
		diag.setInformativeText("Tem a certeza que deseja apagar o item em questao?");
 		diag.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		diag.setDefaultButton(QMessageBox::Cancel);
		int ret = diag.exec();
		switch(ret){
			case QMessageBox::Ok:
				scene.removeItem(item);	
			break;
			default:
			break;
		}
	}
}

void DragWidgetGrid::mouseReleaseEvent(QMouseEvent *event){
	
	if ((event->pos() - drag_start_pos).manhattanLength()
          < QApplication::startDragDistance())
     return;

	if(m_drawline)
	{
		scene.addLine(QLineF(mapToScene(event->pos()),mapToScene(drag_start_pos)),QPen(Qt::SolidLine));
		m_drawline = false;	
	}
	else if(selectedItem){
		selectedItem->setPos(mapToScene(event->pos()));
		selectedItem = NULL;
	}
	drag_start_pos = QPoint(0,0);
}

void DragWidgetGrid::keyPressEvent( QKeyEvent * event ){
	if(selectedItem) {
		QPointF rec = selectedItem->sceneBoundingRect().center();
		 switch(event->key()){
			 case Qt::Key_Right:
			 	qDebug() << "Item scene pos " << selectedItem->mapFromParent(0,0);				
			 	selectedItem->moveBy(-rec.x(),-rec.y());
				selectedItem->rotate(45);
				selectedItem->moveBy(rec.x(),rec.y());
			 break;		 
			 case Qt::Key_Left:
				selectedItem->moveBy(-rec.x(),-rec.y());
				selectedItem->rotate(-45);
				selectedItem->moveBy(rec.x(),rec.y());
			 break;
			 case Qt::Key_Up:
			 	//shear default value
			 break;
			 case Qt::Key_Down:
			 	//shear "back" default value
			 break;
		 }
  	}
}

void DragWidgetGrid::copyToClipboard()
{
    QClipboard *cb = QApplication::clipboard();
    cb->setPixmap(QPixmap::grabWidget(this,NULL));
}

void DragWidgetGrid::saveToFile()
{
    QString name = QFileDialog::getSaveFileName(this, QLatin1String("Save as image"), QString(), QLatin1String("*.png"));
    if (!name.endsWith(QLatin1String(".png")))
        name.append(QLatin1String(".png"));
   QPixmap::grabWidget(this,NULL).save(name, "PNG");
}

void DragWidgetGrid::sendToPrinter(){
	
	QPrinter printer;

     QPrintDialog *dialog = new QPrintDialog(&printer, this);
     dialog->setWindowTitle(tr("Print Document"));
     if (dialog->exec() != QDialog::Accepted)
         return;
	QPainter painter;
	painter.begin(&printer);
	painter.drawPixmap(0,0,QPixmap::grabWidget(this,NULL));
	painter.end();
}
