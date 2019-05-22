// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue May 21 17:14:12 2019
//  Last Modified : <190521.2004>
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

#ifndef __FSEDIALOGS_H
#define __FSEDIALOGS_H

#include <QDialog>
#include <QPolygonF>
#include <QComboBox>

class QColor;
class QDoubleSpinBox;
class QSpinBox;
class QPushButton;
class QCheckBox;
class QLineEdit;
class QListWidget;

#include "../support/ColorSelector.h"
#include "../support/TitledDialog.h"

namespace Schematic {

class PinOrientationAndInversion : public QComboBox {
    Q_OBJECT
public:
    enum PinOrient {LeftNormal, RightNormal, TopNormal, BottomNormal,
          LeftInverted, RightInverted, TopInverted, BottomInverted};
    PinOrientationAndInversion(QWidget *parent = Q_NULLPTR);
    void setValue(PinOrient orientation);
    PinOrient value() const;
};
                             
class AddPinDialog : public TitledDialog {
    Q_OBJECT
public:
    AddPinDialog(QWidget *parent = 0);
    bool draw(double &xpos, double &ypos, 
              PinOrientationAndInversion::PinOrient &orientation, 
              double &length, double & linethickness, int &pinno, 
              QString &pinname, QString &font, int &size, QColor &color, 
              bool editing = true, QString title=tr("Edit Pin"), 
              QString button=tr("Edit"));
private:
    QDoubleSpinBox *xposSB;
    QDoubleSpinBox *yposSB;
    PinOrientationAndInversion *orientCB;
    QDoubleSpinBox *lengthSB;
    QDoubleSpinBox *linethicknessSB;
    QSpinBox       *pinnoSB;
    QLineEdit      *pinnameLE;
    QComboBox      *fontCB;
    QSpinBox       *sizeSB;
    ColorSelector  *colorCS;
    QPushButton *addButton;
private slots:
    void acceptCheck();
};


}

#endif // __FSEDIALOGS_H

