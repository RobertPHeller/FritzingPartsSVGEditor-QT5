// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:49:17 2019
//  Last Modified : <190525.2129>
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

#ifndef __FPEEDIT_H
#define __FPEEDIT_H

#include <QRectF>

#include "feedit.h"
#include "fpedialogs.h"

class FEPCBEditor : public FEEdit {
public:
    FEPCBEditor(SizeAndVP::UnitsType units = SizeAndVP::mm, 
           double width=25.4, double height=25.4, 
           const QRectF &viewport = QRectF(0,0,254,254),
           QWidget *parent = 0);
    virtual ~FEPCBEditor();
    virtual void loadFile(const QString &filename);
    virtual void saveFile(const QString &filename);
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
    virtual void processSVGTag(QDomElement &element, QDomElement &parentGroup);
private:
    PCB::AddPinDialog *addPinDialog;
    PCB::AddRectDialog *addRectDialog;
    PCB::AddLineDialog *addLineDialog;
    PCB::AddCircDialog *addCircDialog;
    PCB::AddArcDialog *addArcDialog;
    PCB::AddPolyDialog *addPolyDialog;
    PCB::AddTextDialog *addTextDialog;

    void processCircle(QDomElement &element, QString parentID);
    void processRect(QDomElement &element, QString parentID);
    void processLine(QDomElement &element, QString parentID);
    void processPath(QDomElement &element, QString parentID);
    void processEllipse(QDomElement &element, QString parentID);
    void processText(QDomElement &element, QString parentID);
    void processPolygon(QDomElement &element, QString parentID);
};


#endif // __FPEEDIT_H

