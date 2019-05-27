// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun May 26 15:18:45 2019
//  Last Modified : <190526.2026>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

static const char rcsid[] = "@(#) : $Id$";

#include <QWidget>
#include <QDialog>
#include <QTextBrowser>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "helpwidget.h"

HelpWidget *HelpWidget::defaultHelpWindow = NULL;
QString HelpWidget::defaultHelpDirectory("");
QString HelpWidget::defaultTableOfContents("");

void HelpWidget::setDefaults(QString helpdir, QString toc)
{
    defaultHelpDirectory = helpdir;
    defaultTableOfContents = toc;
}

void HelpWidget::Help(QString topic)
{
    if (defaultHelpWindow == NULL) {
        defaultHelpWindow = new HelpWidget;
    }
    
    defaultHelpWindow->helpTopic(topic);
}

void HelpWidget::helpTopic(QString topic)
{
    this->show();
    bool found = contents->find(topic);
    if (found) {
        QRect crect = contents->cursorRect();
        QPoint p(crect.x()+(crect.width()/2),crect.y()+(crect.height()/2));
        QString anchor = contents->anchorAt(p);
        if (!anchor.isEmpty()) {
            content->setSource(QUrl(anchor));
        }
    }
}

HelpWidget::HelpWidget(QWidget *parent) : QDialog(parent)
{
    setModal(false);
    setAttribute(Qt::WA_DeleteOnClose, false);
    
    QStringList directoryPath(defaultHelpDirectory);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QSplitter *splitter = new QSplitter;
    splitter->setOrientation( Qt::Horizontal );
    contents = new ContentsBrowser;
    QFile toc(defaultHelpDirectory+"/"+defaultTableOfContents);
    toc.open(QIODevice::ReadOnly);
    QString buffer(toc.readAll());
    toc.close();
    contents->setHtml(buffer);
    splitter->addWidget(contents);
    content  = new QTextBrowser;
    content->setSearchPaths(directoryPath);
    connect(contents,SIGNAL(anchorClicked(const QUrl &)),content,SLOT(setSource(const QUrl &)));
    splitter->addWidget(content);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    mainLayout->addWidget(splitter);
    searchBox = new QLineEdit;
    connect(searchBox,SIGNAL(returnPressed()),this,SLOT(search()));
    mainLayout->addWidget(searchBox);
    QDialogButtonBox *buttons = new QDialogButtonBox;
    buttons->setOrientation(Qt::Horizontal);
    buttons->addButton(QDialogButtonBox::Close);
    QPushButton *back = new QPushButton(tr("Back"));
    buttons->addButton(back,QDialogButtonBox::ActionRole);
    QPushButton *forward = new QPushButton(tr("Forward"));
    buttons->addButton(forward,QDialogButtonBox::ActionRole);
    buttons->addButton(QDialogButtonBox::Help);
    connect(buttons,SIGNAL(rejected()),this,SLOT(accept()));
    connect(buttons,SIGNAL(helpRequested()),this,SLOT(help()));
    connect(back,SIGNAL(clicked()),content,SLOT(backward()));
    connect(forward,SIGNAL(clicked()),content,SLOT(forward()));
    mainLayout->addWidget(buttons);
    setLayout(mainLayout);
}

void HelpWidget::help()
{
    helpTopic("Help");
}

void HelpWidget::search()
{
}

