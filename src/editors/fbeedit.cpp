// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:50:22 2019
//  Last Modified : <190519.1058>
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
#include <QRectF>
#include <QColor>
#include <QDebug>

#include "fbedialogs.h"
#include "fbeedit.h"

FEBreadboardEditor::FEBreadboardEditor(SizeAndVP::UnitsType units,double width,double height,const QRectF &viewport,QWidget *parent)
      : FEEdit(units,width,height,viewport,parent)
{
    addPinDialog = new Breadboard::AddPinDialog(this);
    addRectDialog = new Breadboard::AddRectDialog(this);
    addLineDialog = new Breadboard::AddLineDialog(this);
    addCircDialog = new Breadboard::AddCircDialog(this);
    addArcDialog = new Breadboard::AddArcDialog(this);
    addTextDialog = new Breadboard::AddTextDialog(this);
}

FEBreadboardEditor::~FEBreadboardEditor()
{
}

void FEBreadboardEditor::addPin()
{
    double xpos = 0, ypos = 0, diameter = 1;
    QColor color("black");
    
    pinno++;
    if (addPinDialog->draw(xpos, ypos, diameter, color, pinno, false, tr("Add Pin"), tr("Add"))) {
    }
}

void FEBreadboardEditor::editPin(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1;
    QColor color("black");
    
    int editpinno = 1;
    if (addPinDialog->draw(xpos, ypos, diameter, color, editpinno, false, tr("Edit Pin"), tr("Edit"))) {
    }
}

void FEBreadboardEditor::addRect()
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("black");
    bool filled = false;
    if (addRectDialog->draw(xpos, ypos, width, height, linethickness, color, filled, false, "Add Rectangle", "Add")) {
    }
}

void FEBreadboardEditor::editRect(int gid)
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("black");
    bool filled = false;
    if (addRectDialog->draw(xpos, ypos, width, height, linethickness, color, filled, true, "Edit Rectangle", "Edit")) {
    }
}

void FEBreadboardEditor::addLine()
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("black");
    if (addLineDialog->draw(x1, y1, x2, y2, linethickness, color, false, tr("Add Line"), tr("Add"))) {
    }
}

void FEBreadboardEditor::editLine(int gid)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("black");
    if (addLineDialog->draw(x1, y1, x2, y2, linethickness, color, true, tr("Add Line"), tr("Add"))) {
    }
}

void FEBreadboardEditor::addCirc()
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0;
    QColor color("black");
    if (addCircDialog->draw(xpos, ypos, diameter, center, color, false, tr("Add Circle"), tr("Add"))) {
    }
}

void FEBreadboardEditor::editCirc(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0;
    QColor color("black");
    if (addCircDialog->draw(xpos, ypos, diameter, center, color, true, tr("Edit Circle"), tr("Edit"))) {
    }
}

void FEBreadboardEditor::addArc()
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0, start = 0, extent = 0;
    QColor color("black");
    if (addArcDialog->draw(xpos, ypos, diameter, center, start, extent, color, false, tr("Add Arc"), tr("Add"))) {
    }
}

void FEBreadboardEditor::editArc(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0, start = 0, extent = 0;
    QColor color("black");
    if (addArcDialog->draw(xpos, ypos, diameter, center, start, extent, color, true, tr("Exit Arc"), tr("Edit"))) {
    }
}

void FEBreadboardEditor::addPoly()
{
}

void FEBreadboardEditor::editPoly(int gid)
{
}

void FEBreadboardEditor::addText()
{
    double xpos = 0, ypos = 0, size = 0;;
    QString text(""), font("DroidSans");
    QColor color("black");
    if (addTextDialog->draw(xpos, ypos, text, font, size, color, false, tr("Add Text"), tr("Add"))) {
    }
}

void FEBreadboardEditor::editText(int gid)
{
    double xpos = 0, ypos = 0, size = 0;
    QString text(""), font("DroidSans");
    QColor color("black");
    if (addTextDialog->draw(xpos, ypos, text, font, size, color, true, tr("Edit Text"), tr("Edit"))) {
    }
}

