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


#include "svglistwidget.h"



SvgListWidget::SvgListWidget(QWidget *parent)
	: QFrame(parent)
{

    setAcceptDrops(true);
    createSvgList();
	createConnectorsList();
    int num_blocks = qpixmap_list.size();
    setMinimumSize(200, 260*num_blocks);
	
	numberOfConnectors = connectors.size();
	numberOfSpaces = svgs_filenames.size();
}
void SvgListWidget::createConnectorsList(){

	QString connectors_dir = QString(":/images/connectors/");
	QDir resources = QDir( connectors_dir );
	QStringList filters;
	filters << "*.svg";
	resources.setNameFilters(filters);
    connectors = QStringList(resources.entryList());

    //for each in dir
    for (int i = 0; i < connectors.size(); ++i) {
		QString name = connectors.at(i);
		qDebug() << "Connector: " << name << " : " << i;
		// load svg
		//qDebug() << "SvgListWidget: " << "svg height" << picSvg->height() << "width" << picSvg->width();
		//convert to pixmap
		QPixmap * svgPixmap = new QPixmap(connectors_dir + name);
		//qDebug() << "SvgListWidget: " << "pixmap height" << svgPixmap->height() << "width" << svgPixmap->width();
		qpixmap_list.push_back(*svgPixmap);
		//showSvg( &svgPixmap, svgs_filenames.size()+1, i );
    }
	qDebug() << "Connectors List: done";
}

QList<QPixmap> SvgListWidget::createSvgList(){

	QString resources_dir = QString(":/images/");
	QDir resources = QDir(resources_dir);
	QStringList filters;
	filters << "*.svg";

	resources.setNameFilters(filters);
	svgs_filenames = QStringList(resources.entryList());
    // get resource dir
    //FIXME: read it from a .rc file ?
    if (!resources.exists())
	qWarning("ummm.. no resources?");

    resources.setNameFilters(filters);

    //for each in dir
    for (int i = 0; i < svgs_filenames.size(); ++i) {
		QString name = svgs_filenames.at(i);
		qDebug() << "SvgListWidget: " << name << " : " << i;
		// load svg
		//qDebug() << "SvgListWidget: " << "svg height" << picSvg->height() << "width" << picSvg->width();
		//convert to pixmap
		QPixmap * svgPixmap = new QPixmap(resources_dir + name);
		//qDebug() << "SvgListWidget: " << "pixmap height" << svgPixmap->height() << "width" << svgPixmap->width();
		qpixmap_list.push_back(*svgPixmap);
		//showSvg( &svgPixmap, svgs_filenames.size()+1, i );
    }
	qDebug() << "SvgListWidget: done";
    return qpixmap_list;
}


void SvgListWidget::showSvgs(){

    int num_blocks = qpixmap_list.size();
    for (int i = 0; i < num_blocks; ++i) {
		showSvg( qpixmap_list.at(i), i);
    }
}


void SvgListWidget::showSvg(QPixmap svgPixmap, int c_block ){

    //do alot of stuff :D
    QLabel *svgIcon = new QLabel(this);
    svgIcon->resize(svgPixmap.width(),svgPixmap.height());
    //qDebug() << "SvgListWidget: " << "svgIconnew height" << svgIcon->height() << "width" << svgIcon->width();
    svgIcon->setPixmap(svgPixmap);
	//qDebug() << "SvgListWidget: " << "svgIcon height" << svgIcon->height() << "width" << svgIcon->width();
    svgIcon->setScaledContents ( true );
    svgIcon->setMinimumSize(20,20);
	svgIcon->setFrameStyle(QFrame::StyledPanel | QFrame::Raised | QFrame::Shadow_Mask );

	QString legenda_texto;

	qDebug() << "c_block " << c_block << " numberOfSpaces " << numberOfSpaces;  
	if(c_block < numberOfSpaces)
		legenda_texto = svgs_filenames.at(c_block);
	else legenda_texto = connectors.at(c_block - numberOfSpaces);


	svgIcon->setToolTip( legenda_texto );

	legenda_texto.replace(0,1,(legenda_texto.at(0).toUpper()));
	legenda_texto.remove(QRegExp(".[a-z]*$"));

    //FIXME: this.size()
    svgIcon->move( 20, 20+150*c_block );
    
    svgIcon->show();
    svgIcon->setAttribute(Qt::WA_DeleteOnClose);

	QLabel *legenda = new QLabel(this);

	legenda->setText( legenda_texto );
	legenda->move(20, 20+150*c_block-20 );
	legenda->show();
}


// This event handler is called when a drag is in progress and the mouse enters this widget. The event is passed in the event parameter.
// If the event is ignored, the widget won't receive any drag move events.
void SvgListWidget::dragEnterEvent(QDragEnterEvent *event)
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
// This event handler is called if a drag is in progress, and when any of the following conditions occur:
// * the cursor enters this widget,
// * the cursor moves within this widget,
// * or a modifier key is pressed on the keyboard while this widget has the focus.
// The event is passed in the event parameter.
void SvgListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->ignore();
        }
    } else {
        event->ignore();
    }
}
// This event handler is called when the drag is dropped on this widget. The event is passed in the event parameter.
void SvgListWidget::dropEvent(QDropEvent *event)
{
	
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {

		if (event->source() == this) {
			event->ignore();
		} else {
			event->setDropAction(Qt::MoveAction);
			event->accept();
        }
    } else {
        event->ignore();
    }
}


void SvgListWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;
	QPixmap pixmap;
	const QPixmap * p = child->pixmap();
 
	if(!p)
		return;
	else
		pixmap = *p;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos());


    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
	mimeData->setText(child->toolTip());

	if(isConnector(child->toolTip()))
		isConnectorBeingDragged=true;
	else
		isConnectorBeingDragged=false;
	
	QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());


    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        child->close();
    else {
        child->show();
        child->setPixmap(pixmap);
    }
}

void SvgListWidget::paintEvent(QPaintEvent *){

	//do i need a painter :/
	//QPainter painter(this);

}
 void SvgListWidget::resizeEvent(QResizeEvent *event)
{
	showSvgs();
	QWidget::resizeEvent(event);
}


QPixmap SvgListWidget::getPixmapByName(QString name) {
	int i;
	int size = svgs_filenames.size ();
	i = svgs_filenames.indexOf(name);
	if(i == -1){
		i = connectors.indexOf(name);
	}
	else{
		return qpixmap_list.at(i);
	}
	return qpixmap_list.at(i+size);
}

bool SvgListWidget::isConnector(QString name){
	int i = connectors.indexOf(name);
// 	qDebug() << "Connector " << name << " value " << i << " bool " << (i != -1);
	return (i != -1);
}
