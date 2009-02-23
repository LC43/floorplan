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
		*****DONE: (RUI)conectores: janelas (1.2-1.5) , portas (0.90-1.20)
		*****TODO: boundingRegion <-- calcular a fronteira d todos os objectos ?
		*****DONE: (RUI) os connectores tem q ter por def um z alto.


	*/
#include <QtGui>
#include <QMessageBox>
#include <QXmlStreamAttributes>
#include <QTransform>

#include "dragwidgetgrid.h"
#include "ScenePixmapItem.h"

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
		
		ScenePixmapItem * item;

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

		if(svg_list->isConnectorBeingDragged){
			qDebug() << "e um connector";
			svg_list->isConnectorBeingDragged = false;
			
			underItem = itemAt(event->pos());
			if(underItem){
				item = new ScenePixmapItem(NULL,&scene);
		
				item->setPixmap(pixmap);
				
				item->setPos(underItem->scenePos());
				qDebug() << "no no no :S";
				if(detectBorderCollisions(item)){
					qDebug() << "we collided";
					item->setParentItem(underItem);
					
					item->setZValue(underItem->zValue()+1);
										
					item->setCursor(Qt::ClosedHandCursor);
		
					item->setData(ObjectID,event->mimeData()->text());
		
					item->setData(ObjectX,pixmap.width());
					item->setData(ObjectY,pixmap.height());
				}
				else scene.removeItem(item);			
			}
			else return;
		}else{

			item = new ScenePixmapItem(NULL,&scene);
		
			item->setPixmap(pixmap);
		
			item->setPos(item->pos() + mapToScene(event->pos() - offset));
		
			item->setCursor(Qt::ClosedHandCursor);
		
			item->setData(ObjectID,event->mimeData()->text());
		
			item->setData(ObjectX,pixmap.width());
			item->setData(ObjectY,pixmap.height());
		
		}
		
		QString i_size = QString("x: %1 | y: %2").arg(pixmap.width()).arg(pixmap.height());
		item->setToolTip( i_size );
    } else {
        event->ignore();
    }
}


bool DragWidgetGrid::detectBorderCollisions(QGraphicsItem * item){
	qDebug() << "a detectar.." ;
	bool intersect_or_in = underItem->collidesWithItem(item,Qt::IntersectsItemBoundingRect);
	ScenePixmapItem *  pixmap = dynamic_cast<ScenePixmapItem*>( underItem );
	QString name = pixmap->data(ObjectID).toString();
	if( intersect_or_in)
		qDebug() << "inter com: " << name;
	bool in = underItem->collidesWithItem(item,Qt::ContainsItemShape);
	if( in )
		qDebug() << "colide com: " << name;
	return (intersect_or_in && !in);
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
			ScenePixmapItem *  pixmap = dynamic_cast<ScenePixmapItem*>( item );
			QString name = pixmap->data(ObjectID).toString();
			// adjust mouse
			QPointF item_point = mapFromScene(selectedItem->pos().x(), selectedItem->pos().y());
			drag_distance_to_mouse = event->pos() - item_point;
			drag_start_pos = event->pos();
			if( !name.isEmpty() )
				emit selectedItemOn(name);
			else
				emit selectedItemOn("Linha");
			/*
			qDebug() << "Selected Item collides with:";
			foreach(QGraphicsItem*  item,detectBorderCollisions()){
				qDebug() << item->data(ObjectID).toString();	
			}*/
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
		qDebug() << "distance: x: " << distance_moved_x << " | y: " << distance_moved_y;
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

				qreal old_width =  selectedItem->data(ObjectX).toDouble();
				qreal old_height = selectedItem->data(ObjectY).toDouble();
				
				qreal new_x, new_y;

				qreal sh = distance_moved_x/10;
				if ( distance_moved_x != 0 && fabs(distance_moved_x) > fabs(distance_moved_y) ){
					if( distance_moved_x > 0 ) {
						qDebug() << "right ffs! " << -sh ;
						selectedItem->shear( -sh ,0 ); //right;
					}
					else {
						qDebug() << "left left! " << -sh ;
						selectedItem->shear( -sh,0 ); // left;
					}

					QTransform mt = selectedItem->sceneTransform();

					// adjacent
					new_x = old_width;
					// opposite
					new_y = calculateOpposite(selectedItem->sceneBoundingRect(),new_x );
					
					qDebug() << "oposto: " << new_y << "adjacente: " << new_x;

				}
				else if( distance_moved_y != 0  ){
					if (distance_moved_y > 0 )
						selectedItem->shear( 0,0.1 ); // down;
					else
						selectedItem->shear( 0,-0.1 ); // up;
					// adjacent
					new_y = old_height;
					// opposite 
					new_x = calculateOpposite(selectedItem->sceneBoundingRect(), new_y);
					
					
				}
 				else {
					// nothing happened, both distances were zero
					new_y = old_height;
					// opposite
 					new_x = old_width;
 					
 				}

				//TODO: FIXME: shear still isnt this simple :S

				QString i_size;
				i_size = QString("x: %1 | y: %2").arg( new_x ).arg( new_y );
				selectedItem->setToolTip( i_size );
				qDebug() << "x_a:" << selectedItem->sceneBoundingRect().width();
				qDebug() << "y_a:" << selectedItem->sceneBoundingRect().height();
				selectedItem->setData(ObjectX,new_x);
				selectedItem->setData(ObjectY,new_y);
				break;
			}
			default:{
				// no modifier
				qreal new_pos_x = event->posF().x() - drag_distance_to_mouse.x();
				qreal new_pos_y = event->posF().y() - drag_distance_to_mouse.y();
				if(svg_list->isConnector(selectedItem->data(ObjectID).toString())){
					underItem = itemAt(event->pos());
					if(underItem)
						selectedItem->setPos(mapToScene( new_pos_x, new_pos_y ));		
				}
				else selectedItem->setPos(mapToScene( new_pos_x, new_pos_y ));
			}
		}
		
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

						selectedItem->scale(0.9,1);
						break;
					}
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(-0.1,0);
						break;
#else
					case Qt::AltModifier:
						selectedItem->shear(-0.1,0);
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
						selectedItem->scale(1,0.9);
						break;
					}
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(0,-0.1);
						break;
#else
					case Qt::AltModifier:
						selectedItem->shear(0,-0.1);
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
						selectedItem->shear(0,0.1);
						break;
#else
					case Qt::AltModifier:
						selectedItem->shear(0,0.1);
						break;
#endif
					default:
						selectedItem->translate(0,1);
				}
			break;
			case Qt::Key_Delete:
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
	
	 ScenePixmapItem *  pixmap = dynamic_cast<ScenePixmapItem*>( item );
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
	  stream->writeAttribute( "zValue", QString("%1").arg(item->zValue()));
	  if(item->parentItem ())
	  	stream->writeAttribute( "child", QString("%1").arg(1));
	  else stream->writeAttribute( "child", QString("%1").arg(0));
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
	  qreal z = 0.0;
	  int child = 0;
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
		if ( attribute.name() == "zValue" ) z = attribute.value().toString().toDouble();
		if ( attribute.name() == "child" ) child = attribute.value().toString().toInt();
      }
	  qDebug() << id;
	  QMatrix m = QMatrix(m11,m12,m21,m22,dx,dy);
	  // we need to add the pixmap here
	  QPixmap p = svg_list->getPixmapByName(id);

	 ScenePixmapItem * pixmap = new ScenePixmapItem(NULL,&scene);
		
	 pixmap->setPixmap(p);
	 
	 pixmap->setZValue(z);
	  
	 pixmap->setMatrix(m,false);
	  
	  if(child)
	  {
		  pixmap->setParentItem(itemAt(pixmap->pos().toPoint()));
	  }
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


/*
	only works if its sheared on only one side

*/
qreal DragWidgetGrid::calculateOpposite(QRectF rec, qreal adjacent ){

	qreal rec_w = rec.width();
	qreal rec_h = rec.height();
	// get diagonal, hypotenuse, h
	// diagonal: (0,0) -> ( b_w = b_rec.width, b_h = b_rec.height )
	// hypotenuse:
	qreal h = sqrt( rec_w*rec_w + rec_h*rec_h );

	// get the angle with arcsin:
	qreal a = asin ( adjacent / h );

	//calculate opposite
	qreal o = h * cos( a );
	return o;
				
}

