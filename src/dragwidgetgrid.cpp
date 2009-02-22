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
	/*
		*****TODO: not critical: undo/redo logic
		*****TODO: not critical: a scene so mostra as scroll bars se houver objectos fora do "view port"
		*****TODO: impressao à escala
		*****TODO: conectores: janelas (1.2-1.5) , portas (0.90-1.20)
		*****TODO: boundingRegion <-- calcular a fronteira d todos os objectos ?
		*****TODO: FIXME: os connectores tem q ter por def um z alto.

		+ escala
		item_size = dragged_item->getSize()
		item_size /= 25; // 100pixels / 25 = 4 metros
		dragged_item->tooltip(item_size)

		if (transformation) then getSize..apply the same transformation..tooltip it
		ex:
		size: 100
		item_size = 100/25 = 4
		scale 1.1 -> 100*1.1 = 110 -> 4*1.1 = 4.4 ( 4.4 *25 = 110 certo!)
		rotation +1 -> não altera
		translation +1 -> nao altera
		shear +1 -> nao altera um dos lados, mas o outro
		qreal angulo = atan2(10 , 100 );
		qreal new_side = 100 / sen(angulo)


		
	*/
#include <QtGui>
#include <QMessageBox>
#include <QXmlStreamAttributes>

//#include <math.h>
#include "dragwidgetgrid.h"

//static int max_zoom = 10; //percentage

DragWidgetGrid::DragWidgetGrid(QWidget *parent)
: QGraphicsView (parent)
{
    setMinimumSize(600, 600);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);
    setScene(&scene);
    brush = QBrush (Qt::CrossPattern);
    setBackgroundBrush(brush);
	setDragMode(QGraphicsView::RubberBandDrag);
	selectedItem  = NULL;
	m_drawline=false;
	inicial_zoom = height();
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
	emit zoomChangedSignal(factor);
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
		
		item->setData(ObjectID,event->mimeData()->text());
		
		item->setData(ObjectX,pixmap.width());
		item->setData(ObjectY,pixmap.height());
		
		QString i_size = QString("x: %1 | y: %2").arg(pixmap.width()).arg(pixmap.height());
		item->setToolTip( i_size );
		
		
    } else {
        event->ignore();
    }
}


void DragWidgetGrid::mousePressEvent(QMouseEvent *event)
{
	QGraphicsItem *item;
	bool selected = false;
	qDebug() << "-------------";
	if ((item = itemAt(event->pos())) == NULL) {
		qDebug() << "You didn't click on an item: " << event->pos();
		selectedItem = NULL;
		emit selectedItemOff();
     } else {
		qDebug() << "You clicked on an item: " << event->pos();
		m_drawline = false;
		selected=true;
     }
	 
	 if(event->button() == Qt::LeftButton) {
		if(!selected){
			m_drawline = true;
			drag_start_pos = event->pos();
		}
		else{
			selectedItem = item;
			//NOTE: isnt there a more elegant way to get the name?
			QGraphicsPixmapItem *  pixmap = dynamic_cast<QGraphicsPixmapItem*>( item );
			QString name = pixmap->data(ObjectID).toString();
			// adjust mouse
			QPointF item_point = mapFromScene(selectedItem->pos().x(), selectedItem->pos().y());
			drag_distance_to_mouse = event->pos() - item_point;
			drag_start_pos = event->pos();
			if( !name.isEmpty() )
				emit selectedItemOn(name);
			else
				emit selectedItemOn("Linha");
			
			qDebug() << "Selected Item collides with:";
			foreach(QGraphicsItem*  item,detectBorderCollisions()){
				qDebug() << item->data(ObjectID).toString();	
			}
		}
	}
	else if(selected){
		QMessageBox diag;
		diag.setInformativeText(trUtf8("Deseja reverter alterações efectuadas ao objecto?"));
 		diag.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		diag.setDefaultButton(QMessageBox::Cancel);
		int ret = diag.exec();
		switch(ret){
			case QMessageBox::Ok:
				item->resetTransform();
			break;
			default:
			break;
		}
	}
}

void DragWidgetGrid::mouseReleaseEvent(QMouseEvent *event){

	if(m_drawline && (event->pos() - drag_start_pos).manhattanLength() != 0 ){
		QPen linha;
		linha.setStyle(Qt::SolidLine);
		linha.setWidth(2);
		scene.addLine(QLineF(mapToScene(event->pos()),mapToScene(drag_start_pos)),linha);
		m_drawline = false;	
	}
	else if(selectedItem){
		qreal distance_moved_x = drag_start_pos.x() - event->posF().x();
		qreal distance_moved_y = drag_start_pos.y() - event->posF().y();
		int mod = QApplication::keyboardModifiers();
		switch(mod){
			case Qt::ControlModifier:
				//FIXME: rotate exponential
				selectedItem->rotate(distance_moved_x+distance_moved_y);
				break;
			case Qt::ShiftModifier:{
				qDebug() << "x_b:" << selectedItem->sceneBoundingRect().width();
				qreal factor_x;
				qreal factor_y;
				qDebug() << "dist_x:" << distance_moved_x << "dist_y" << distance_moved_y;
				factor_x = 1 - (distance_moved_x/100);
				factor_y = 1 - (distance_moved_y/100);
				selectedItem->scale(factor_x, factor_y);
				//TODO: FIXME: boundingRect returns a wrong size, and i dont know if scale() works well
				QRectF rec = selectedItem->sceneBoundingRect();
				QString i_size;
				i_size = QString("x: %1 | y: %2").arg(rec.width()).arg(rec.height());
				selectedItem->setToolTip( i_size );
				qDebug() << "x_a:" << selectedItem->sceneBoundingRect().width();
				selectedItem->setData(ObjectX,rec.width());
				selectedItem->setData(ObjectY,rec.height());
				break;
			}

#if !defined(Q_OS_WIN)
			case Qt::MetaModifier:{
#else
			case Qt::AltModifier:{
#endif
				selectedItem->shear( distance_moved_x/10, distance_moved_y/10 );

				QRectF rec = selectedItem->sceneBoundingRect();

				//TODO: FIXME: shear isnt this simple :)
				//shear +1 -> nao altera um dos lados, mas o outro
				/*qreal angulo = atan2(distance_moved_x);
				qreal new_side = 100 / sin(angulo);
				qDebug() << "angulo: " << angulo << "side: " << new_side;
				o q acontece se ambos vectores se mexem :S
				*/
				QString i_size;
				i_size = QString("x: %1 | y: %2").arg(rec.width()).arg(rec.height());
				selectedItem->setToolTip( i_size );
				qDebug() << "x_a:" << selectedItem->sceneBoundingRect().width();
				selectedItem->setData(ObjectX,rec.width());
				selectedItem->setData(ObjectY,rec.height());
				
				break;
			}
			default:{
				// no modifier
				qreal new_pos_x = event->posF().x() - drag_distance_to_mouse.x();
				qreal new_pos_y = event->posF().y() - drag_distance_to_mouse.y();
				selectedItem->setPos(mapToScene( new_pos_x, new_pos_y ));
			}
		}
		
		//selectedItem = NULL;

		//WARNING: can go null until we can select more than one block
		//emit selectedItemOff();
	}
	drag_distance_to_mouse  = QPointF(0,0);
	drag_start_pos = QPoint(0,0);
}
void DragWidgetGrid::keyReleaseEvent( QKeyEvent * event ){

	switch(event->key()){
		case Qt::Key_Control:
			emit modifierKeySignal(false, Qt::Key_Control);
			break;
		case Qt::Key_Shift:
			emit modifierKeySignal(false, Qt::Key_Shift);
			break;
#if !defined(Q_OS_WIN)
			case Qt::Key_Meta:
#else
			case Qt::Key_Alt:
#endif		
			emit modifierKeySignal(false, Qt::Key_Meta);
			break;
	}
}

void DragWidgetGrid::keyPressEvent( QKeyEvent * event ){
	if(selectedItem) {
	
/*

		accoes:
		* rodar    :  ctrl             + setas/LMB
			-> void QGraphicsItem::rotate ( qreal angle )
			Rotates the current item transformation angle degrees clockwise around its origin.

		* inclinar :  meta             + setas/LMB
			-> void QGraphicsItem::shear ( qreal sh, qreal sv )
			Shears the current item transformation by (sh, sv).
		
		* escalar  :  shift           + setas/LBM
			-> void QGraphicsItem::scale ( qreal sx, qreal sy )
			Scales the current item transformation by (sx, sy) around its origin.
		
		* deslocar :  sem modificador  + setas/LMB
			-> void QGraphicsItem::translate ( qreal dx, qreal dy )
			Translates the current item transformation by (dx, dy).
		
		* inverter : ?
			-> shift + alt + left : reverte o valor extendido

		detalhes
		* deslocar :  + 1 pixel*(1/zoom)
			se esta com o zoom out em 2x -> 1
		*/
		QRectF rec = selectedItem->sceneBoundingRect();
		QPointF rec_center = rec.center();
		int mod = QApplication::keyboardModifiers();
		switch(event->key()){
			case Qt::Key_Control:
				emit modifierKeySignal(true, Qt::Key_Control);
			break;
			case Qt::Key_Shift:
				emit modifierKeySignal(true, Qt::Key_Shift);
			break;
			case Qt::Key_Meta:
				emit modifierKeySignal(true, Qt::Key_Meta);
				break;
			case Qt::Key_Right:
				switch(mod){
					case Qt::ControlModifier:
						selectedItem->rotate(1);
					break;
					case Qt::ShiftModifier:
						selectedItem->scale(1.1,1);
					break;
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(0.1,0);
					break;
#else
					case Qt::AltModifier:
						selectedItem->shear(0.1,0);
						break;
#endif
					default:
						selectedItem->translate(1,0);
				}
			break;
			case Qt::Key_Left:
				switch(mod){
					case Qt::ControlModifier:
						selectedItem->rotate(-1);
					break;
					case Qt::ShiftModifier:{
// 						qreal rec_width = rec.width();
						//rec_width -= rec_width*1.1;
						//selectedItem->translate(rec_width,0);
						selectedItem->scale(0.9,1);
						break;
					}
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(0.1,0);
						break;
#else
					case Qt::AltModifier:
						selectedItem->shear(0.1,0);
						break;
#endif
					default:
						selectedItem->translate(-1,0);
				}
			break;
			case Qt::Key_Up:
				switch(mod){
					case Qt::ControlModifier:
						break;
					case Qt::ShiftModifier:{
// 						qreal rec_height= rec.height();
// 						rec_height-= rec_height*1.1;
// 						selectedItem->translate(0,rec_height);
						selectedItem->scale(1,0.9);
						break;
					}
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(0.1,0);
						break;
#else
					case Qt::AltModifier:
						selectedItem->shear(0.1,0);
						break;
#endif
					default:
						selectedItem->translate(0,-1);
				}
				
			break;
			case Qt::Key_Down:
				switch(mod){
					case Qt::ControlModifier:
						break;
					case Qt::ShiftModifier:
						selectedItem->scale(1,1.1);
						break;
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(0.1,0);
						break;
#else
					case Qt::AltModifier:
						selectedItem->shear(0.1,0);
						break;
#endif
					default:
						selectedItem->translate(0,1);
				}
			break;
			case Qt::Key_Delete:
				qDebug() << "remove!";
				scene.removeItem(selectedItem);
			break;
			case Qt::Key_PageUp:
				switch(mod){
					case Qt::ControlModifier:
						selectedItem->setZValue(selectedItem->zValue()+1.0);
					break;
				}
			break;
			case Qt::Key_PageDown:
				switch(mod){
					case Qt::ControlModifier:
						selectedItem->setZValue(selectedItem->zValue()-1.0);
					break;
				}
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


/************************************ writeStream ************************************/

void  DragWidgetGrid::resetView(){
 foreach( QGraphicsItem*  item, items() )
  {
	scene.removeItem(item);  
  }
  resetMatrix();
  resetTransform();
}

void  DragWidgetGrid::SaveProject( QXmlStreamWriter* stream )
{
  // write station data to xml stream
  foreach( QGraphicsItem*  item, items() )
  {
    QGraphicsLineItem *  item_line = dynamic_cast<QGraphicsLineItem*>( item );
    if ( item_line )
    {
      stream->writeEmptyElement( "line" );
	  QLineF line = item_line->line();
      stream->writeAttribute( "x1", QString("%1").arg(line.x1()));
      stream->writeAttribute( "y1", QString("%1").arg(line.y1()));
	  stream->writeAttribute( "x2", QString("%1").arg(line.x2()));
      stream->writeAttribute( "y2", QString("%1").arg(line.y2()));
    }
	
	 QGraphicsPixmapItem *  pixmap = dynamic_cast<QGraphicsPixmapItem*>( item );
	 if ( pixmap )
    {
	  QMatrix m = pixmap->sceneMatrix ();
      stream->writeEmptyElement( "pixmap" );
      stream->writeAttribute( "id", QString("%1").arg(pixmap->data(ObjectID).toString()));
	  stream->writeAttribute( "m11", QString("%1").arg(m.m11 ()  ));
	  stream->writeAttribute( "m12", QString("%1").arg(m.m12 () ));
	  stream->writeAttribute( "m21", QString("%1").arg(m.m21 () ));
	  stream->writeAttribute( "m22", QString("%1").arg(m.m22 () ));  
	  stream->writeAttribute( "dx", QString("%1").arg(m.dx ()  ));
	  stream->writeAttribute( "dy", QString("%1").arg(m.dy ()));
    }
	
  }
}

void  DragWidgetGrid::LoadProject( QXmlStreamReader* stream )
{
  // read station data from xml stream

  while ( !stream->atEnd() )
  {
    stream->readNext();
    if ( stream->isStartElement() && stream->name() == "line" )
    {
      qreal x1 = 0.0, y1 = 0.0,x2 = 0.0,y2=0.0;
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
        if ( attribute.name() == "x1" ) x1 = attribute.value().toString().toDouble();
        if ( attribute.name() == "y1" ) y1 = attribute.value().toString().toDouble();
		if ( attribute.name() == "x1" ) x2 = attribute.value().toString().toDouble();
        if ( attribute.name() == "y1" ) y2 = attribute.value().toString().toDouble();
      }
      scene.addLine(QLineF(x1,y1,x2,y2),QPen(Qt::SolidLine));
    }
	
	if ( stream->isStartElement() && stream->name() == "pixmap" )
    {
      qreal m11 = 0.0;
	  qreal m12 = 0.0;
	  qreal m21 = 0.0;
	  qreal m22 = 0.0;
	  qreal dx = 0.0;
	  qreal dy = 0.0;
	  QString id;
      foreach( QXmlStreamAttribute attribute, stream->attributes() )
      {
		if ( attribute.name() == "id" ) id = attribute.value().toString();
        if ( attribute.name() == "m11" ) m11 = attribute.value().toString().toDouble();
        if ( attribute.name() == "m12" ) m12 = attribute.value().toString().toDouble();
		if ( attribute.name() == "m21" ) m21 = attribute.value().toString().toDouble();
        if ( attribute.name() == "m22" ) m22 = attribute.value().toString().toDouble();
		if ( attribute.name() == "dx" ) dx = attribute.value().toString().toDouble();
        if ( attribute.name() == "dy" ) dy = attribute.value().toString().toDouble();
      }
	  qDebug() << id;
	  QMatrix m = QMatrix(m11,m12,m21,m22,dx,dy);
	  // we need to add the pixmap here
	  QPixmap p = svg_list->getPixmapByName(id);

	  QGraphicsPixmapItem * pixmap = scene.addPixmap(p);
	  
	  pixmap->setMatrix(m,false);
	}	
	
  }
	
}
void DragWidgetGrid::increaseZoom(){
	qreal factor = 1.2;
	scale(factor, factor);
	}
void DragWidgetGrid::decreaseZoom(){
	qreal factor = 0.8;
	scale(factor, factor);
}

QList<QGraphicsItem *>  DragWidgetGrid::detectBorderCollisions(){
    QList<QGraphicsItem *>  intersect_or_in = scene.collidingItems(selectedItem,Qt::IntersectsItemBoundingRect);
	QList<QGraphicsItem *>  in = scene.collidingItems(selectedItem,Qt::ContainsItemShape);
	
	foreach(QGraphicsItem * item,in){
		intersect_or_in.removeAll(item);
	}
		
	return intersect_or_in;
}


