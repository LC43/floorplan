#include "ScenePixmapItem.h"


ScenePixmapItem::ScenePixmapItem(QGraphicsItem * parent, QGraphicsScene * scene) 
	: QGraphicsPixmapItem(parent,scene){};

QRectF ScenePixmapItem::boundingRect (){
	QPixmap p = pixmap();
	return QRectF(0.0,0.0,(qreal)p.width(),(qreal)p.height());
}
