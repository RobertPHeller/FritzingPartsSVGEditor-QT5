// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat May 18 23:56:36 2019
//  Last Modified : <190519.0049>
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

#ifndef __FBEDIALOGS_H
#define __FBEDIALOGS_H

#include <QDialog>

class QColor;
class QDoubleSpinBox;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QComboBox;

#include "../support/ColorSelector.h"

namespace Breadboard {

class AddPinDialog : public QDialog {
    Q_OBJECT
public:
    AddPinDialog(QWidget *parent = 0);
    bool draw(double &xpos, double &ypos, double &diameter, QColor &color, int &pinno);
private:
    QDoubleSpinBox *xposSB;
    QDoubleSpinBox *yposSB;
    QDoubleSpinBox *diameterSB;
    ColorSelector *colorCS;
    QSpinBox *pinnoSB;
    QPushButton *addButton;
private slots:
    void acceptCheck();
};

class AddRectDialog : public QDialog {
    Q_OBJECT
public:
    AddRectDialog(QWidget *parent = 0);
    bool draw(double &xpos, double &ypos, double &width, double &height, double &linethickness, QColor &color, bool &filled);
private:
    QDoubleSpinBox *xposSB;
    QDoubleSpinBox *yposSB;
    QDoubleSpinBox *widthSB;
    QDoubleSpinBox *heightSB;
    QDoubleSpinBox *linethicknessSB;
    ColorSelector *colorCS;
    QCheckBox *filledCKB;
    QPushButton *addButton;
private slots:                                                                  
    void acceptCheck();
};

class AddLineDialog : public QDialog {
    Q_OBJECT
public:
    AddLineDialog(QWidget *parent = 0);
    bool draw(double &x1, double &y1, double &x2, double &y2, double &linethickness, QColor &color);
private:
    QDoubleSpinBox *x1SB;
    QDoubleSpinBox *y1SB;
    QDoubleSpinBox *x2SB;
    QDoubleSpinBox *y2SB;
    QDoubleSpinBox *linethicknessSB;
    ColorSelector *colorCS;
    QPushButton *addButton;
private slots:                                                                  
    void acceptCheck();
};

class AddCircDialog : public QDialog {
    Q_OBJECT
public:
    AddCircDialog(QWidget *parent = 0);
    bool draw(double &xpos, double &ypos, double &diameter, double &center, QColor &color);
private:
    QDoubleSpinBox *xposSB;
    QDoubleSpinBox *yposSB;
    QDoubleSpinBox *diameterSB;
    QDoubleSpinBox *centerSB;
    ColorSelector *colorCS;
    QPushButton *addButton;
private slots:
    void acceptCheck();
};

class AddArcDialog : public QDialog {
    Q_OBJECT
public:
    AddArcDialog(QWidget *parent = 0);
    bool draw(double &xpos, double &ypos, double &diameter, double &center, double &start, double &extent, QColor &color);
private:
    QDoubleSpinBox *xposSB;
    QDoubleSpinBox *yposSB;
    QDoubleSpinBox *diameterSB;
    QDoubleSpinBox *centerSB;
    QDoubleSpinBox *startSB;
    QDoubleSpinBox *extentSB;
    ColorSelector *colorCS;
    QPushButton *addButton;
private slots:
    void acceptCheck();
};

class AddTextDialog : public QDialog {
    Q_OBJECT
public:
    AddTextDialog(QWidget *parent = 0);
    bool draw(double &xpos, double &ypos, QString &text, QString &font, double &size, QColor &color);
private:
    QDoubleSpinBox *xposSB;
    QDoubleSpinBox *yposSB;
    QLineEdit *textLE;
    QComboBox *fontCB;
    QDoubleSpinBox *sizeSB;
    ColorSelector *colorCS;
    QPushButton *addButton;
private slots:
    void acceptCheck();
};


}



#endif // __FBEDIALOGS_H

