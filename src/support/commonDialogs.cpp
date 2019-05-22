// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed May 22 11:49:59 2019
//  Last Modified : <190522.1331>
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
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QAction>
#include "TitledDialog.h"
#include "commonDialogs.h"
#include "debug.h"

CommonDialog::CommonDialog(const QString &message, DialogType dtype, TitledDialog::IconType itype, QWidget *parent)
      : TitledDialog(itype, parent)
{
    _createDialog(message, dtype);
}

CommonDialog::CommonDialog(const QString &message, const QIcon & userIcon, DialogType dtype, QWidget *parent)
      : TitledDialog(userIcon, parent)
{
    _createDialog(message, dtype);
}


CommonDialog::CommonDialog(const QString &message, const QPixmap & userPixmap, DialogType dtype, QWidget *parent)
      : TitledDialog(userPixmap, parent)
{
    _createDialog(message, dtype);
}

void CommonDialog::_createDialog(const QString &message, DialogType dtype)
{
    setTitle(message);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    switch (dtype) {
    case AbortRetryIgnore:
        buttons =  new QDialogButtonBox(QDialogButtonBox::Abort|
                                        QDialogButtonBox::Retry|
                                        QDialogButtonBox::Ignore,
                                        Qt::Horizontal);
        break;
    case Ok:
        buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal);
        break;
    case OkCancel:
        buttons = new QDialogButtonBox(QDialogButtonBox::Ok|
                                       QDialogButtonBox::Cancel,
                                       Qt::Horizontal);
        break;
    case RetryCancel:
        buttons = new QDialogButtonBox(QDialogButtonBox::Retry|
                                       QDialogButtonBox::Cancel,
                                       Qt::Horizontal);
        break;
    case YesNo:
        buttons = new QDialogButtonBox(QDialogButtonBox::Yes|
                                       QDialogButtonBox::No,
                                       Qt::Horizontal);
        break;
    case YesNoCancel:
        buttons = new QDialogButtonBox(QDialogButtonBox::Yes|
                                       QDialogButtonBox::No|
                                       QDialogButtonBox::Cancel,
                                       Qt::Horizontal);
        break;
    }
    connect(buttons,SIGNAL(clicked(QAbstractButton *)),
            this,SLOT(handleClick(QAbstractButton *)));
    mainLayout->addWidget(buttons);
    setLayout(mainLayout);
}


void CommonDialog::handleClick(QAbstractButton *button)
{
    switch (buttons->standardButton(button)) {
    case QDialogButtonBox::Ok:
        done(OK);
        break;
    case QDialogButtonBox::Yes:
        done(YES);
        break;
    case QDialogButtonBox::No:
        done(NO);
        break;
    case QDialogButtonBox::Abort:
        done(ABORT);
        break;
    case QDialogButtonBox::Retry:
        done(RETRY);
        break;
    case QDialogButtonBox::Ignore:
        done(IGNORE);
        break;
    case QDialogButtonBox::Cancel:
        done(CANCEL);
        break;
    default:
        done(QDialog::Rejected);
        break;
    }
}

CommonDialog::ResponseType CommonDialog::draw(const QString &message)
{
    setMessage(message);
    return (ResponseType) exec();
}

CommonDialog *CommonDialog::YesNoDialog = NULL;
CommonDialog *CommonDialog::OkDialog = NULL;

void CommonDialog::InitCommonDialogs()
{
    YesNoDialog = new CommonDialog(QString(), CommonDialog::YesNo, TitledDialog::Question);
    OkDialog = new CommonDialog(QString(), CommonDialog::Ok, TitledDialog::Question);
}
