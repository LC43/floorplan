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

#include "configdialog.h"
#include "pages.h"

ConfigDialog::ConfigDialog()
{
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(12);

	// le o valor da escala das settings
	leEscala();
	
    pagesWidget = new QStackedWidget;
	ConfigurationPage *zoomPage = new ConfigurationPage();
	pagesWidget->addWidget( /*new ConfigurationPage()*/ zoomPage );


	// buttons
    QPushButton *closeButton = new QPushButton(tr("&Fechar"));
	closeButton->setDefault(true);
	okButton = new QPushButton(tr("&OK"));
	aplicarButton = new QPushButton(tr("&Aplicar"));

	// greyed ate que seja feita alguma alteracao
	if( m_escala == zoomPage->getCurrentText() ) {
		okButton->setEnabled(false);
		aplicarButton->setEnabled(false);
	}
    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	connect(zoomPage, SIGNAL(escalaChanged(QString)),this,SLOT(escalaAlteradaSlot(QString)));

	

	connect(okButton, SIGNAL(clicked()), this, SLOT(guardaEscala()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

	
	// aplica
	connect(aplicarButton, SIGNAL(clicked()), this, SLOT(guardaEscala()));
	
	
	// main ao receber o signal, actualiza a status bar.



    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);

	buttonsLayout->addWidget( okButton );
	buttonsLayout->addWidget( aplicarButton );
	buttonsLayout->addWidget( closeButton );
	
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(trUtf8("Preferências"));
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Zoom"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigDialog::showConfigDialog() {

	this->show();

}


// este sinal e' activado qdo a pagina de escolha de escala foi alterada
void ConfigDialog::escalaAlteradaSlot( QString escala ){

	if( escala != m_escala ){
		m_escala = escala;
		okButton->setEnabled(true);
		aplicarButton->setEnabled(true);
	}
}


// fazer conhecer que a escala foi alterada. (status bar, imprimir, etc)
void ConfigDialog::guardaEscala(){

	QSettings settings("CodePoets", "Floor Plan Preferencias");
	settings.setValue("escala", m_escala);
	emit escalaAlteradaSignal(m_escala);
	
}

void ConfigDialog::leEscala(){
	QSettings settings("CodePoets", "Floor Plan Preferencias");
	m_escala = settings.value("escala").toString();
}

QString ConfigDialog::getEscala(){
	return m_escala;
}


	
