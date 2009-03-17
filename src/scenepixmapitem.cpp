#include "scenepixmapitem.h"


ScenePixmapItem::ScenePixmapItem(QGraphicsItem * parent, QGraphicsScene * scene) 
	: QGraphicsPixmapItem(parent,scene){};

QRectF ScenePixmapItem::boundingRect (){
	QPixmap p = pixmap();
//  	qDebug() << "ScenePixmapItem::pixmap width" << (qreal)p.width(); // obrigado por testares :)
	return QRectF(0.0,0.0,(qreal)p.width(),(qreal)p.height());
}

QRectF ScenePixmapItem::sceneBoundingRect() {
	if( !parentItem() ){
// 		if (d_ptr->parent || d_ptr->hasTransform)
// 		qDebug() << "ScenePixmapItem::has parent";
		return sceneTransform().mapRect(boundingRect());
	}
	else {
// 		qDebug() << "ScenePixmapItem::has not parent"; // totally random choice for scenePos() :S
		return boundingRect().translated(scenePos());
	}
}
