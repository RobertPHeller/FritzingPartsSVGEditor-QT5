// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat May 18 14:32:13 2019
//  Last Modified : <190518.2028>
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

#ifndef __COLORSELECTOR_H
#define __COLORSELECTOR_H

#include <QWidget>
#include <QColor>
#include <QValidator>
#include <QLineEdit>

class QPushButton;
class QAction;
class QColorDialog;

class ColorValidator : public  QValidator {
    Q_OBJECT
public:
    ColorValidator(QObject *parent = Q_NULLPTR) : QValidator(parent)
    {
    }
    virtual State validate(QString &input, int &) const {
        if (QColor::isValidColor(input)) {
            return Acceptable;
        } else {
            return Intermediate;
        }
    }
};

class ColorSelector : public QWidget {
    Q_OBJECT
public:
    ColorSelector(QWidget *parent = 0);
    inline void setValue(const QColor &c) {
        color = c; 
        colorEdit->setText(color.name());}
    inline QColor value() const {return color;}
    bool isValid() const;
private:
    QLineEdit *colorEdit;
    QPushButton *colorSelButton;
    QColor color;
    QColorDialog *colorSelDialog;
private slots:
    void select();
};

#endif // __COLORSELECTOR_H

