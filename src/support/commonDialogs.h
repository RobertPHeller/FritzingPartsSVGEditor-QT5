// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Wed May 22 11:49:35 2019
//  Last Modified : <190522.1332>
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

#ifndef __COMMONDIALOGS_H
#define __COMMONDIALOGS_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>

#include "TitledDialog.h"

class CommonDialog : public TitledDialog
{
    Q_OBJECT
public:
    enum DialogType {AbortRetryIgnore, Ok, OkCancel, RetryCancel, YesNo, YesNoCancel};
    enum ResponseType {OK, YES, NO, ABORT, RETRY, IGNORE, CANCEL};
    CommonDialog(const QString &message, DialogType dtype = Ok, TitledDialog::IconType itype = TitledDialog::Info, QWidget *parent = NULL);
    CommonDialog(const QString &message, const QIcon & userIcon, DialogType dtype = Ok, QWidget *parent = NULL);
    CommonDialog(const QString &message, const QPixmap & userPixmap, DialogType dtype = Ok, QWidget *parent = NULL);
    inline void setMessage(const QString &message) {setTitle(message); }
    inline const QString message() const {return title();}
    ResponseType draw(const QString &message);
    static CommonDialog *YesNoDialog;
    static CommonDialog *OkDialog;
    static void InitCommonDialogs();
private:
    void _createDialog(const QString &message, DialogType dtype);
    QDialogButtonBox *buttons;
private slots:
    void handleClick(QAbstractButton *button);
};


#endif // __COMMONDIALOGS_H

