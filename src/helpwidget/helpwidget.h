// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun May 26 15:17:47 2019
//  Last Modified : <190526.1948>
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

#ifndef __HELPWIDGET_H
#define __HELPWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QMouseEvent>
class QTextBrowser;
class QLineEdit;

#include "../support/debug.h"

class ContentsBrowser : public QTextEdit
{
    Q_OBJECT
public:
    ContentsBrowser(QWidget *parent = Q_NULLPTR) : QTextEdit(parent)
    {
        setReadOnly(true);
        setCursor(Qt::ArrowCursor);
    }
    
protected:
    void mousePressEvent(QMouseEvent *e)
    {
        QString anchor = anchorAt(QPoint(e->x(),e->y()));
        if (anchor.isEmpty()) return;
        //stdError << "ContentsBrowser::mousePressEvent(): anchor is " << anchor << "\n";
        emit anchorClicked(QUrl(anchor));
    }
    void mouseMoveEvent(QMouseEvent *e)
    {
        QString anchor = anchorAt(QPoint(e->x(),e->y()));
        if (anchor.isEmpty()) {
            viewport()->setCursor(Qt::ArrowCursor);
        } else {
            viewport()->setCursor(Qt::PointingHandCursor);
        }
    }
signals:
    void anchorClicked(const QUrl &link);
};

class HelpWidget : public QDialog
{
    Q_OBJECT
public:
    HelpWidget(QWidget *parent = NULL);
    static void Help(QString topic);
    void helpTopic(QString topic);
    static void setDefaults(QString helpdir, QString toc);
private:
    QTextEdit *contents;
    QTextBrowser *content;
    QLineEdit    *searchBox;
    static HelpWidget *defaultHelpWindow;
    static QString defaultHelpDirectory;
    static QString defaultTableOfContents;
private slots:
    void help();
    void search();
};



#endif // __HELPWIDGET_H

