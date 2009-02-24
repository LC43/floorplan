#include "scenepixmapitem.h"


ScenePixmapItem::ScenePixmapItem(QGraphicsItem * parent, QGraphicsScene * scene) 
	: QGraphicsPixmapItem(parent,scene){};

QRectF ScenePixmapItem::boundingRect (){
	QPixmap p = pixmap();
	qDebug() << "mto util..."; // obrigado por testares :)
	return QRectF(0.0,0.0,(qreal)p.width(),(qreal)p.height());
}
QRectF ScenePixmapItem::sceneBoundingRect() {
// 		if (d_ptr->parent || d_ptr->hasTransform)
			return sceneTransform().mapRect(boundingRect());
// 		return boundingRect().translated(d_ptr->pos);
}
