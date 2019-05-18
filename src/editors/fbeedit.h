// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:45:04 2019
//  Last Modified : <190517.2227>
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

#ifndef __FBEEDIT_H
#define __FBEEDIT_H

#include "feedit.h"
#include <QRectF>

class FEBreadboardEditor : public FEEdit {
public:
    FEBreadboardEditor(SizeAndVP::UnitsType units = SizeAndVP::mm, 
           double width=25.4, double height=25.4, 
           const QRectF &viewport = QRectF(0,0,254,254),
           QWidget *parent = 0);
    virtual ~FEBreadboardEditor();
protected slots:
    virtual void addPin();
    virtual void editPin(int gid);
    virtual void addRect();
    virtual void editRect(int gid);
    virtual void addLine();
    virtual void editLine(int gid);
    virtual void addCirc();
    virtual void editCirc(int gid);
    virtual void addArc();
    virtual void editArc(int gid);
    virtual void addPoly();
    virtual void editPoly(int gid);
    virtual void addText();
    virtual void editText(int gid);
};

#endif // __FBEEDIT_H

