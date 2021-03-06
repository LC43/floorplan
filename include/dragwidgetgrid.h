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

#ifndef DRAGWIDGETGRID_H
#define DRAGWIDGETGRID_H


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QtSvg>
#include <QPrinter>
#include <QMatrix>
#include <qpainter.h>
#include <qpicture.h>
#include <QTransform>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <svglistwidget.h>
#include <scenepixmapitem.h>

#define ObjectName 0
#define ObjectID 1
#define ObjectX 2
#define ObjectY 3
#define Sheared 4
#define ScalingToReal 25
#define MaskSheared 1


class DragWidgetGrid : public QGraphicsView
{
	 Q_OBJECT
public:
    DragWidgetGrid(QWidget *parent=0);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
	void keyReleaseEvent( QKeyEvent * event );
	void keyPressEvent ( QKeyEvent * event );
	void setSvgList(SvgListWidget * list) {svg_list = list;};
	void LoadProject(QXmlStreamReader* stream);
	void SaveProject(QXmlStreamWriter* stream);
	void resetView();
	
public slots:
	void copyToClipboard();
	void saveToFile();
	void sendToPrinter();
	//used by mainwindow menu
	void increaseZoom();
	void decreaseZoom();
	//protected:

signals:
	void zoomChangedSignal( qreal factor );
	void modifierKeySignal( bool state, int qt_key );
	void selectedItemOn(QString name);
	void selectedItemOff();
		
private:
	QPainter *paint;
	QPicture *pic;
	QGraphicsScene scene;
	QBrush brush;
	QMatrix original;
	QGraphicsItem * selectedItem;
	QGraphicsItem * underItem;
	QPoint drag_start_pos;
	QPointF drag_distance_to_mouse;
	bool m_drawline;
	QGraphicsLineItem * line;
	SvgListWidget * svg_list;
	//QMatrix trans;
	int inicial_zoom;
	bool detectBorderCollisions(QGraphicsItem *);
	QString itemName(QGraphicsItem*);
	QString beautifyName(QString);
	QRectF msceneBoundingRect(QGraphicsItem*);

	qreal metodoDosQuadrados( QRectF , qreal );
	qreal calculateOpposite( QRectF , qreal );
	qreal calculateOppositeByReverting( QGraphicsItem *, bool vertical=true );
	qreal newShearedY( QGraphicsItem * , qreal );
	qreal newShearedX( QGraphicsItem * , qreal );
	void reShear( QGraphicsItem * item, qreal sh, qreal new_x, qreal new_y );
	void printQTransform(  QTransform mt );
	void saveXYData( ScenePixmapItem *item );

	int sheared;	
};


#endif
