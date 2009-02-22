#ifndef _content_item
#define _content_item

#include <QGraphicsPixmapItem>

class ScenePixmapItem : public QGraphicsPixmapItem {

public:
	ScenePixmapItem(QGraphicsItem * parent, QGraphicsScene * scene);
	QRectF boundingRect ();
};


#endif
