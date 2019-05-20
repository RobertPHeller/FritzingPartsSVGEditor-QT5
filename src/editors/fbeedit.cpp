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
//  Last Modified : <190520.1611>
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
#include "../support/debug.h"
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
    addPolyDialog = new Breadboard::AddPolyDialog(this);
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
        gid++;
        stdError << "FEBreadboardEditor::addPin(): gid = " << gid << '\n';
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = x+diameter;
        qreal h = y+diameter;
        stdError << "FEBreadboardEditor::addPin(): x = " << x << ", y = " << y 
              << ", w = " << w << ", h = " << h << '\n';
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,QPen(Qt::NoPen),QBrush(color));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinno));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Group1,QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::editPin(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1;
    QColor color("black");
    int editpinno = 1;
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid));
    if (items.size() == 0) {return;}
    if (items.size() > 1) {
        // Should not happen!
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Pin) {
        // Should not happen!
        return;
    }
    QGraphicsEllipseItem *eitem = (QGraphicsEllipseItem *) item;
    QRectF coords = eitem->rect();
    stdError << "FEBreadboardEditor::editPin(): coords.x() = " << coords.x() << ", coords.y() = " << coords.y() << ", coords.width() = " << coords.width() << ", coords.height() = " << coords.height() << '\n';
    xpos = coords.x()+(coords.width()/2.0);
    ypos = coords.y()+(coords.height()/2.0);
    diameter = (coords.height()+coords.width())/2.0;
    stdError << "FEBreadboardEditor::editPin(): xpos = " << xpos << ", ypos = " << ypos << ", diameter = " << diameter << '\n';
    color = eitem->brush().color();
    editpinno = eitem->data((int)FEGraphicsScene::Pinno).toInt();
    
    if (addPinDialog->draw(xpos, ypos, diameter, color, editpinno, true, tr("Edit Pin"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        if (editpinno > pinno) pinno = editpinno;
        //gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = x+diameter;
        qreal h = y+diameter;
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,QPen(Qt::NoPen),QBrush(color));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinno));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Group1,QVariant((int)FEGraphicsScene::Breadboard));
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
    QPolygonF points;
    double linethickness;
    bool filled, closed;
    QColor color;
    if (addPolyDialog->draw(points, linethickness, filled, closed, color, false, tr("Add Polygon"), tr("Add"))) {
    }
}

void FEBreadboardEditor::editPoly(int gid)
{
    QPolygonF points;
    double linethickness;
    bool filled, closed;
    QColor color;
    if (addPolyDialog->draw(points, linethickness, filled, closed, color, true, tr("Edit Polygon"), tr("Edit"))) {
    }
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

