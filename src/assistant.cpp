/****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
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

#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QLibraryInfo>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>

#include "assistant.h"

Assistant::Assistant()
    : proc(0)
{
}


Assistant::~Assistant()
{
    if (proc && proc->state() == QProcess::Running) {
        proc->terminate();
        proc->waitForFinished(3000);
    }
}

void Assistant::showDocumentation(const QString &page)
{
    if (!startAssistant())
        return;
    QTextStream str(proc);
    str << QLatin1String("SetSource qthelp://codepoets.floorplan/doc/")
        << page << QLatin1Char('\0') << endl;
}

bool Assistant::startAssistant()
{
    if (!proc)
        proc = new QProcess();

    if (proc->state() != QProcess::Running) {

#if !defined(Q_OS_WIN)
		QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#else
		QString app = QString();
#endif
#if !defined(Q_OS_MAC)
		app += QLatin1String("assistant");
#else
		app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

		QString collection_file;
		collection_file.append("docs");
		collection_file.append(QDir::separator());
		collection_file.append("floorplan.qhc");
		QStringList args;
        args << QLatin1String("-collectionFile")
		//<< QLatin1String("docs/floorplan.qhc")
		<<  collection_file
		<< QLatin1String("-enableRemoteControl");
		
        proc->start(app, args);
		qDebug() << " lido a coleccao";
        if (!proc->waitForStarted()) {
            QMessageBox::critical(0, QObject::tr("Assistente"),
                QObject::trUtf8("Não foi possível lançar o Assistante (%1)").arg(app));
            return false;
        }    
    }
    return true;
}
