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
		*****TODO: boundingRegion <-- calcular a fronteira d todos os objectos ?



	*/
#include <QtGui>
#include <QMessageBox>
#include <QXmlStreamAttributes>
#include <QTransform>

#include "dragwidgetgrid.h"
#include "scenepixmapitem.h"

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

					item->setZValue(underItem->zValue()+1);
										
					item->setCursor(Qt::ClosedHandCursor);
		
					item->setData(ObjectID,event->mimeData()->text());
		
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
		

		}
		
		qreal pixmap_width = pixmap.width();
		pixmap_width /= ScalingToReal;
		qreal pixmap_height = pixmap.height();
		pixmap_height /= ScalingToReal;
		QString i_size = QString("x: %1 | y: %2").arg(pixmap_width).arg(pixmap_height);
		item->setToolTip( i_size );

		item->setData(ObjectX,pixmap_width);
		item->setData(ObjectY,pixmap_height);
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
			
			QString name = itemName( item );
			// adjust mouse
			QPointF item_point = mapFromScene(selectedItem->pos().x(), selectedItem->pos().y());
			drag_distance_to_mouse = event->pos() - item_point;
			drag_start_pos = event->pos();
			if( !name.isEmpty() )
				emit selectedItemOn(beautifyName(name));
			else
				emit selectedItemOn("Linha");
		}
	}
	else if(selected){
		QMessageBox diag;
		diag.setText(trUtf8("Deseja reverter alterações efectuadas ao objecto?"));
 		diag.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		diag.setDefaultButton(QMessageBox::Cancel);
		int ret = diag.exec();
		switch(ret){
			case QMessageBox::Ok:
				item->resetTransform(); // n é bem reset, pq assim tb vao os dx dy :S
			break;
			default:
			break;
		}
	}
}
QString DragWidgetGrid::itemName(QGraphicsItem *item){
	QString name = item->data(ObjectID).toString();
	return name;
}
QString DragWidgetGrid::beautifyName(QString uglyname){
	uglyname.replace(0,1,(uglyname.at(0).toUpper()));
	uglyname.remove(QRegExp(".[a-z]*$"));
	// returning uglyname but with better looks :D
	return uglyname;
}
QRectF DragWidgetGrid::msceneBoundingRect(QGraphicsItem *item){
	ScenePixmapItem *  scene_pixmap_item = dynamic_cast<ScenePixmapItem*>( item );
	return scene_pixmap_item->sceneBoundingRect();
	return item->sceneBoundingRect();
}
qreal DragWidgetGrid::metodoDosQuadrados( QRectF big_rec, qreal side ){
	// we build a new square rect with the known side
	qreal k_x = side*ScalingToReal;
	qreal k_y = side*ScalingToReal;
	qreal k_w = big_rec.width();// - new_x*ScalingToReal;
	qreal k_h = big_rec.height();// - new_x*ScalingToReal;
	QRectF * known_rec = new QRectF ( k_x,  k_y , k_w, k_h );
	qreal rec_w = known_rec->width()-k_x;
	qreal rec_h = known_rec->height()-k_y;
	// the remaining rect will contain the new side
	qreal new_y = sqrt( rec_w*rec_w + rec_h*rec_h );
	new_y /= ScalingToReal;
	return new_y;
}

/* 	only works if its sheared on only one side */
qreal DragWidgetGrid::calculateOpposite(QRectF rec, qreal adjacent ){

	qreal rec_w = rec.width();
	qreal rec_h = rec.height();
	// get diagonal, hypotenuse, h
	// diagonal: (0,0) -> ( b_w = b_rec.width, b_h = b_rec.height )
	// hypotenuse:
	qreal h = sqrt( rec_w*rec_w + rec_h*rec_h );
	// get the angle with arcsin:
	qreal a = acos ( adjacent / h );
	//calculate opposite
	qreal o = h * sin( a );
	return o;
}
qreal DragWidgetGrid::newShearedY( QGraphicsItem * item, qreal x ) {
	qreal new_y;
	QTransform mt = item->sceneTransform();
	QRectF big_rec = msceneBoundingRect(  item );
	if (mt.m12() != 0 )
		new_y = metodoDosQuadrados( big_rec, x*ScalingToReal );
	else
		new_y = calculateOpposite( big_rec, x*ScalingToReal );
	return new_y;
}

void DragWidgetGrid::reShear( QGraphicsItem * item, qreal sh, qreal new_x, qreal new_y ) {
	QString name = itemName( selectedItem );
    new_y /= ScalingToReal;
	QTransform mt = item->sceneTransform();
	if(svg_list->isConnector(name)) {
	// os connectores so tem shear horizontal..
		if ( name.contains("porta", Qt::CaseInsensitive) ){

		    if( new_y > 1.2 ){
				// height esta fixo
				selectedItem->shear( -mt.m21() , 0  );

				qreal angle = acos( new_x / 1.2 );
				qreal new_sh = 1.2 * sin(angle) ;

				if( sh > 0 )
					selectedItem->shear( -new_sh , 0  );
				new_y = 1.2;
			}
		} else if ( name.contains("janela", Qt::CaseInsensitive) ){
			if( new_y > 1.5){
				selectedItem->shear( -mt.m21() , 0  );
				qreal angle = acos( new_x / 1.5 );
				qreal new_sh = 1.5 * sin(angle) ;
				if( sh > 0 )
					selectedItem->shear( -new_sh , 0  );
				new_y = 1.5;
			}
		}
		QString i_size;
		i_size = QString("x: %1 | y: %2").arg( new_x ).arg( new_y );
		selectedItem->setToolTip( i_size );
		selectedItem->setData(ObjectX,new_x);
		selectedItem->setData(ObjectY,new_y);
	}
}




void DragWidgetGrid::printQTransform(  QTransform mt ){
	qDebug() << "mt1x: " << mt.m11() << "\t" << mt.m12() << "\t" << mt.m13();
	qDebug() << "mt2x: " << mt.m21() << "\t" << mt.m22() << "\t" << mt.m23();
	qDebug() << "mt3x: " << mt.m31() << "\t" << mt.m32() << "\t" << mt.m33();
	qDebug() << "___________________";
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
				qreal factor_x;
				qreal factor_y;

				factor_x = 1 - (distance_moved_x/100);
				factor_y = 1 - (distance_moved_y/100);
				if(svg_list->isConnector(selectedItem->data(ObjectID).toString())) {
					// limite porta: 0.90 - 1.2
					// limite porta: 0.90 - 1.2
                    // nao podem ser escalados no eixo_y (in)felizmente, o x roda com o objecto
                    selectedItem->scale(1 , factor_y );
                    QRectF rec = selectedItem->sceneBoundingRect();
                    QString name = beautifyName(itemName( selectedItem ));
                    if ( name.contains("porta", Qt::CaseInsensitive) ){
                    // se a nova dimensao da porta for maior que M ou menor q m
						if( rec.height() > 1.2*ScalingToReal ) {
							qreal factor_h = 1.2*ScalingToReal / rec.height();
							selectedItem->scale(1, factor_h );
						} else if( rec.height() < 0.9*ScalingToReal){
							qreal factor_h = 0.9*ScalingToReal / rec.height() ;
							selectedItem->scale(1, factor_h );
						}
						rec = selectedItem->sceneBoundingRect();
						selectedItem->setData(ObjectY,rec.height()/ScalingToReal);

					} else if ( name.contains("janela", Qt::CaseInsensitive) ){
						qDebug() << ":S: rec_h: " << rec.height() << "<=?" << 1.5*ScalingToReal;
						qDebug() << ":S: rec_h: " << rec.height() << ">=?" << 1.2*ScalingToReal;
						if( rec.height() > 1.5*ScalingToReal ){
							// need to find max factor & rescale 
							qreal factor_h = 1.5*ScalingToReal / rec.height();
							selectedItem->scale(1, factor_h );
						} else if ( rec.height() < 1.2*ScalingToReal) {
							qreal factor_h = 1.2*ScalingToReal / rec.height() ;
							selectedItem->scale(1, factor_h );
						} // always something changes if the dragging occurs..
						rec = selectedItem->sceneBoundingRect();
						selectedItem->setData(ObjectY,rec.height()/ScalingToReal);
					}
				} else {
					qDebug() << "x_b:" << selectedItem->sceneBoundingRect().width();

					selectedItem->scale(factor_x, factor_y);
					//TODO: FIXME: boundingRect returns a wrong size, and i dont know if scale() works well
					QRectF rec = selectedItem->sceneBoundingRect();
					QString i_size;
					i_size = QString("x: %1 | y: %2").arg(rec.width()/ScalingToReal).arg(rec.height()/ScalingToReal);
					selectedItem->setToolTip( i_size );
					qDebug() << "x_a:" << selectedItem->sceneBoundingRect().width();
					selectedItem->setData(ObjectX,rec.width()/ScalingToReal);
					selectedItem->setData(ObjectY,rec.height()/ScalingToReal);
				}
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

				qreal sh = distance_moved_x/100;
				
				QString name = itemName( selectedItem );
				
				// moving on the x axis.. parallel..lely */
				if ( distance_moved_x != 0 && fabs(distance_moved_x) > fabs(distance_moved_y) ){
				
					QRectF f_rec = msceneBoundingRect(  selectedItem );


					selectedItem->shear( -sh ,0 ); //right!! turn right! the other right! .. :p
					/*
					se for um shear uni-direccional aplicado a uma matriz onde ja haja
					um shear noutra direccao de um vector  perpendicular,
					 + entao: metodo dos quadrados
					 + caso contrario, metodo do triangulo :>
							 */
					QTransform mt = selectedItem->sceneTransform();
					QRectF big_rec = msceneBoundingRect(  selectedItem );

					// adjacent
					new_x = old_width; //*ScalingToReal;

					// opposite
					if (mt.m12() != 0 )
						new_y = metodoDosQuadrados( big_rec, new_x*ScalingToReal );
					else 
						new_y = calculateOpposite( big_rec, new_x*ScalingToReal );
					
					new_y /= ScalingToReal;
					if(svg_list->isConnector(name)) {
						// os connectores so tem shear horizontal..
						if ( name.contains("porta", Qt::CaseInsensitive) ){
							if( new_y > 1.2 ){
								// height esta fixo
								selectedItem->shear( -mt.m21() , 0  );
								
								qreal angle = acos( new_x / 1.2 );
								qreal new_sh = 1.2 * sin(angle) ;
								
								if( sh > 0 )
									selectedItem->shear( -new_sh , 0  );
 								else
 									selectedItem->shear( new_sh , 0  );

								new_y = 1.2;
								/*
								 // TODO: parece-me q o minimo q o connector pode ter é o tamanho
								// que tinha inicial, q é smp válido.
							} else if( new_y < 0.9 ){
								qreal less_min = 0.9 - new_y ;
								selectedItem->shear( less_min*ScalingToReal/10, 0 );
								new_y = 0.9;
								*/
							}
						} else if ( name.contains("janela", Qt::CaseInsensitive) ){
							if( new_y > 1.5){
								selectedItem->shear( -mt.m21() , 0  );
								qreal angle = acos( new_x / 1.5 );
								qreal new_sh = 1.5 * sin(angle) ;
								
								if( sh > 0 )
									selectedItem->shear( -new_sh , 0  );
								new_y = 1.5;
// 							}else if( new_y < 1.2 ){
								// TODO: same here, does it change to less than?
// 								qreal less_min = 1.2 - new_y ;
// 								selectedItem->shear(0, less_min*ScalingToReal/10 );
// 								new_y = 1.2;
							}
						}
					}
			} else if( distance_moved_y != 0  ){  // moving on the y axis ..
					if(svg_list->isConnector(name)) {
						// do nothing, expect keep gcc quiet :)
						new_x = old_width;
						new_y = old_height;
					} else {
						selectedItem->shear( 0, distance_moved_y/100); // up/ down;
						// adjacent
						new_y = old_height;
						QTransform mt = selectedItem->sceneTransform();
						QRectF big_rec = msceneBoundingRect(  selectedItem );
						// opposite
						if (mt.m12() != 0 )
							new_x = metodoDosQuadrados( big_rec, new_y );
						else
							new_x = calculateOpposite( big_rec, new_y );
					}
				}else {
					// nothing happened, both distances were zero
					// wont happen but keeps gcc quiet :)
					new_y = old_height;
					// opposite
					new_x = old_width;
				}
				QString i_size;
				i_size = QString("x: %1 | y: %2").arg( new_x ).arg( new_y );
				selectedItem->setToolTip( i_size );
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
					if(underItem){
						selectedItem->setPos(mapToScene( new_pos_x, new_pos_y ));
					}
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
			case Qt::Key_Right: {
				QString i_size;
				qreal old_width =  selectedItem->data(ObjectX).toDouble();
				qreal old_height = selectedItem->data(ObjectY).toDouble();
				qreal new_y;
				
				switch(mod){
					case Qt::ControlModifier:
						selectedItem->rotate(1);
					break;
					case Qt::ShiftModifier:
						selectedItem->scale(1.01,1);
						i_size = QString("x: %1 | y: %2").arg( old_width ).arg( old_height );
						selectedItem->setToolTip( i_size );
						selectedItem->setData(ObjectX,old_width);
						selectedItem->setData(ObjectY,old_height);
					break;
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
#else
					case Qt::AltModifier:
#endif						
						selectedItem->shear(0.1,0);
						// ai :S TODO: bug!! wont shear after some point to the right!
						new_y = newShearedY(selectedItem, old_width );
						reShear( selectedItem , 0.1, old_width, new_y );
						break;

					default:
						selectedItem->translate(1,0);
				}
			break;
			}
			case Qt::Key_Left:{
				QString i_size;
				qreal old_width =  selectedItem->data(ObjectX).toDouble();
				qreal old_height = selectedItem->data(ObjectY).toDouble();
				qreal new_y;
				switch(mod){
					case Qt::ControlModifier:
						selectedItem->rotate(-1);
					break;
					case Qt::ShiftModifier:{

						selectedItem->scale(0.99,1);
						i_size = QString("x: %1 | y: %2").arg( old_width ).arg( old_height );
						selectedItem->setToolTip( i_size );
						selectedItem->setData(ObjectX,old_width);
						selectedItem->setData(ObjectY,old_height);
						break;
					}
#if !defined(Q_OS_WIN)
					case Qt::MetaModifier:
						selectedItem->shear(-0.1,0);
						new_y = newShearedY(selectedItem, old_width );
						reShear( selectedItem , -0.1, old_width, new_y );
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
			}
			case Qt::Key_Up:
				switch(mod){
					case Qt::ControlModifier:
						break;
					case Qt::ShiftModifier:{
						selectedItem->scale(1,0.99);
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
						selectedItem->scale(1,1.01);
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
		 qDebug() << "x_a:" << selectedItem->sceneBoundingRect().width();
		 qDebug() << "y_a:" << selectedItem->sceneBoundingRect().height();
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
		stream->setAutoFormatting(true);
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
			stream->writeAttribute( "new_x", QString("%1").arg(pixmap->data(ObjectX).toString()));
			stream->writeAttribute( "new_y", QString("%1").arg(pixmap->data(ObjectY).toString()));
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
		if (stream->name() == "line" ){
			qDebug() << "Linhas: \n";
			
			qreal x1 = 0.0, y1 = 0.0,x2 = 0.0,y2=0.0;
			foreach( QXmlStreamAttribute attribute, stream->attributes() ){
				qDebug() << "att: " << attribute.name().toString();
				if ( attribute.name() == "x1" ) x1 = attribute.value().toString().toDouble();
				if ( attribute.name() == "y1" ) y1 = attribute.value().toString().toDouble();
				if ( attribute.name() == "x1" ) x2 = attribute.value().toString().toDouble();
				if ( attribute.name() == "y1" ) y2 = attribute.value().toString().toDouble();
			}
			scene.addLine(QLineF(x1,y1,x2,y2),QPen(Qt::SolidLine));
		}
	
		if (stream->name() == "pixmap" )
		{
			qreal m11 = 0.0;
			qreal m12 = 0.0;
			qreal m21 = 0.0;
			qreal m22 = 0.0;
			qreal dx = 0.0;
			qreal dy = 0.0;
			qreal z = 0.0;
			qreal new_x = 0.0;
			qreal new_y = 0.0;
			int child = 0;
			QString id;
			foreach( QXmlStreamAttribute attribute, stream->attributes() )
			{
				qDebug() << "att: " << attribute.name().toString();
				if ( attribute.name() == "id" ) id = attribute.value().toString();
				if ( attribute.name() == "m11" ) m11 = attribute.value().toString().toDouble();
				if ( attribute.name() == "m12" ) m12 = attribute.value().toString().toDouble();
				if ( attribute.name() == "m21" ) m21 = attribute.value().toString().toDouble();
				if ( attribute.name() == "m22" ) m22 = attribute.value().toString().toDouble();
				if ( attribute.name() == "dx" ) dx = attribute.value().toString().toDouble();
				if ( attribute.name() == "dy" ) dy = attribute.value().toString().toDouble();
				if ( attribute.name() == "zValue" ) z = attribute.value().toString().toDouble();
				if ( attribute.name() == "child" ) child = attribute.value().toString().toInt();
				if ( attribute.name() == "new_x" ) new_x = attribute.value().toString().toDouble();
				if ( attribute.name() == "new_y" ) new_y = attribute.value().toString().toDouble();
			}
			qDebug() << id;
		
			QMatrix * m = new QMatrix(m11,m12,m21,m22,dx,dy);
			// we need to add the pixmap here
		
			ScenePixmapItem * pixmap = new ScenePixmapItem(NULL,&scene);
		
			pixmap->setPixmap(svg_list->getPixmapByName(id));
			pixmap->setData(ObjectID,id);
			pixmap->setData(ObjectX,new_x);
			pixmap->setData(ObjectY,new_y);
			pixmap->setToolTip( QString("x: %1 | y: %2").arg(new_x).arg(new_y));
			pixmap->setZValue(z);
			pixmap->setMatrix(*m,false);
	  
			if(child) {
				pixmap->setParentItem(itemAt(pixmap->pos().toPoint()));
			}
		}
	
	}if (stream->hasError()) {
		qDebug() << "oops: " << stream->errorString();
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



