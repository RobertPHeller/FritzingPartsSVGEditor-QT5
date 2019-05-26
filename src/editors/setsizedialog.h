// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun May 26 09:16:50 2019
//  Last Modified : <190526.1011>
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

#ifndef __SETSIZEDIALOG_H
#define __SETSIZEDIALOG_H

#include <QWidget>
class QDoubleSpinBox;
class QRadioButton;

#include "../support/TitledDialog.h"
#include "feedit.h"

class SetSizeDialog : public TitledDialog
{
    Q_OBJECT
public:
    SetSizeDialog(QWidget *parent = 0);
    bool draw(double &width, double &height, QRectF &viewport, SizeAndVP::UnitsType &units);
private:
    QDoubleSpinBox *widthSB, *heightSB, *vp_xSB,  *vp_ySB,  *vp_widthSB,  
          *vp_heightSB;
    QRadioButton *mmRB, *inRB;
private slots:
    void acceptCheck();
};


#endif // __SETSIZEDIALOG_H

