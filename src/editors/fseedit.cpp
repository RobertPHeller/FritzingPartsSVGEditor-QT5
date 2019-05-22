// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:50:41 2019
//  Last Modified : <190522.1229>
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
#include "fseedit.h"
#include "fsedialogs.h"
#include "../installedfonts.h"                                                  

#include "../support/debug.h"

FESchematicEditor::FESchematicEditor(SizeAndVP::UnitsType units,double width,double height,const QRectF &viewport,QWidget *parent)
      : FEEdit(units,width,height,viewport,parent)
{
    addPinDialog = new Schematic::AddPinDialog(this);
    addRectDialog = new Breadboard::AddRectDialog(this);
    addLineDialog = new Breadboard::AddLineDialog(this);
    addCircDialog = new Breadboard::AddCircDialog(this);
    addArcDialog = new Breadboard::AddArcDialog(this);
    addPolyDialog = new Breadboard::AddPolyDialog(this);
    addTextDialog = new Breadboard::AddTextDialog(this);
}

FESchematicEditor::~FESchematicEditor()
{
}


void FESchematicEditor::addPin()
{
    double xpos = 0, ypos = 0, length = 1, linethickness = 1;
    Schematic::PinOrientationAndInversion::PinOrient 
          orientation(Schematic::PinOrientationAndInversion::LeftNormal);
    QString pinname(""), font("DroidSans");
    int size = 1;
    QColor color("black");
    
    pinno++;
    if (addPinDialog->draw(xpos, ypos, orientation, length, linethickness, 
                           pinno, pinname, font, size, color, false,
                           tr("Add Pin"), tr("Add"))) {
        stdError << "FESchematicEditor::addPin(): xpos is " << xpos <<
              " ypos is " << ypos << " orientation " << orientation <<
              " length is " << length << " linethickness is " << linethickness
              << " pinno is " << pinno << " pinname is " << pinname << 
              " font is " << font << " size is " << size << " color is " <<
              color << '\n';
        _addPin_helper(xpos,ypos,orientation,length,linethickness,pinno,
                       pinname,font,size,color);
    }
}

void FESchematicEditor::_addPin_helper(double xpos, double ypos, 
                                       Schematic::PinOrientationAndInversion::PinOrient orientation,
                                       double length, double linethickness, int pinno, 
                                       QString pinname, QString font, int size, QColor color)
{
    double cr, ir, px2, py2, lx, ly, nx, ny, ix, iy;
    bool inverted = false;
    QGraphicsEllipseItem *connection = NULL, *invertCircle = NULL;
    QGraphicsLineItem *leadline = NULL;
    QGraphicsSimpleTextItem *pinnoLabel = NULL, *pinnameLabel = NULL;
    gid++;
    cr = .1*length;
    ir = .2*length;
    connection = canvas->addEllipse(xpos-cr, ypos-cr, cr*2, cr*2, QPen(Qt::NoPen), QBrush(color));
    switch (orientation) {
    case Schematic::PinOrientationAndInversion::LeftInverted:
        inverted = true;
    case Schematic::PinOrientationAndInversion::LeftNormal:
        px2 = xpos+length;
        py2 = ypos;
        lx  = px2+linethickness*2;
        ly  = ypos;
        nx  = xpos + (length / 2.0);
        ny  = ypos - (linethickness * 2);
        if (inverted) {
            px2 -= ir*2;
            ix   = px2+ir;
            iy   = ypos;
        }
        break;
    case Schematic::PinOrientationAndInversion::RightInverted:
        inverted = true;
    case Schematic::PinOrientationAndInversion::RightNormal:
        px2 = xpos - length;
        py2 = ypos;
        lx  = px2;
        ly  = ypos-(linethickness*2);
        nx  = xpos - (length / 2.0);
        ny  = xpos - (linethickness * 2.0);
        if (inverted) {
            px2 += ir*2;
            ix   = px2 - ir;
            iy   = ypos;
        }
        break;
    case Schematic::PinOrientationAndInversion::TopInverted:
        inverted = true;
    case Schematic::PinOrientationAndInversion::TopNormal:
        px2 = xpos;
        py2 = ypos + length;
        lx  = px2;
        ly  = py2;
        nx  = xpos + (linethickness * 2);
        ny  = ypos + (length / 2.0);
        if (inverted) {
            py2 -= ir*2;
            ix   = xpos;
            iy   = py2 + ir;
        }
        break;
    case Schematic::PinOrientationAndInversion::BottomInverted:
        inverted = true;
    case Schematic::PinOrientationAndInversion::BottomNormal:
        px2 = xpos;
        py2 = ypos - length;
        lx  = px2;
        ly  = py2;
        nx  = xpos + (linethickness * 2);
        ny  = ypos - (length / 2.0);
        if (inverted) {
            py2 += ir*2;
            ix   = xpos;
            iy   = py2 - ir;
        }
        break;
    }
    leadline = canvas->addLine(xpos, ypos, px2, py2, QPen(color,linethickness));
    if (inverted) {
        invertCircle = canvas->addEllipse(ix-ir, iy-ir, ir*2, ir*2, QPen(color,linethickness), QBrush(Qt::NoBrush));
    }
    if (!pinname.isEmpty()) {
        pinnameLabel = canvas->addSimpleText(pinname, QFont(font,size));
        pinnameLabel->setBrush(color);
        pinnameLabel->setPos(lx,ly);
    }
    pinnoLabel = canvas->addSimpleText(QString::number(pinno),QFont(font,size));
    pinnoLabel->setBrush(color);
    pinnoLabel->setPos(nx,ny);
    connection->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    connection->setData((int)FEGraphicsScene::Pinno,QVariant(pinno));
    connection->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Pin));
    connection->setData((int)FEGraphicsScene::Group1,
                  QVariant((int)FEGraphicsScene::Schematic));
    connection->setData((int)FEGraphicsScene::Group2,
                  QVariant((int)FEGraphicsScene::PinConnections));
    leadline->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    leadline->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
    leadline->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    leadline->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Pins));
    leadline->setData((int)FEGraphicsScene::Orientation,
                      QVariant((int)orientation));
    leadline->setData((int)FEGraphicsScene::Length,
                      QVariant(length));
    if (invertCircle != NULL) {
        invertCircle->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        invertCircle->setData((int)FEGraphicsScene::Type,
                          QVariant((int)FEGraphicsScene::Pin));
        invertCircle->setData((int)FEGraphicsScene::Group1,
                          QVariant((int)FEGraphicsScene::Schematic));
        invertCircle->setData((int)FEGraphicsScene::Group2,
                          QVariant((int)FEGraphicsScene::Pins));
    }
    pinnoLabel->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    pinnoLabel->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
    pinnoLabel->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    pinnoLabel->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::PinNumbers));
    if (pinnameLabel != NULL) {
        pinnameLabel->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        pinnameLabel->setData((int)FEGraphicsScene::Type,
                          QVariant((int)FEGraphicsScene::Pin));
        pinnameLabel->setData((int)FEGraphicsScene::Group1,
                          QVariant((int)FEGraphicsScene::Schematic));
        pinnameLabel->setData((int)FEGraphicsScene::Group2,
                          QVariant((int)FEGraphicsScene::PinLabels));
    }
}


void FESchematicEditor::editPin(int gid)
{
    double xpos = 0, ypos = 0, length = 1, linethickness = 1;
    Schematic::PinOrientationAndInversion::PinOrient 
          orientation(Schematic::PinOrientationAndInversion::LeftNormal);
    QString pinname(""), font("DroidSans");
    int size = 1, editpinno;
    QColor color("black");
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid));
    if (items.size() == 0) {return;}
    QGraphicsEllipseItem *connection = NULL, *invertCircle = NULL;
    QGraphicsLineItem *leadline = NULL;
    QGraphicsSimpleTextItem *pinnoLabel = NULL, *pinnameLabel = NULL;
    for (items_Iterator ii = items.begin();
         ii != items.end();
         ii++) {
        QGraphicsItem *item = *ii;
        if ((FEGraphicsScene::GroupIdType)(item->data((int)FEGraphicsScene::Group2).toInt()) ==
            FEGraphicsScene::PinConnections) {
            connection = (QGraphicsEllipseItem *)item;
        } else if ((FEGraphicsScene::GroupIdType)(item->data((int)FEGraphicsScene::Group2).toInt()) ==
                   FEGraphicsScene::Pins) {
            if (item->type() == QGraphicsLineItem::Type) {
                leadline = (QGraphicsLineItem*)item;
            } else if (item->type() == QGraphicsEllipseItem::Type) {
                invertCircle = (QGraphicsEllipseItem *)item;
            }
        } else if ((FEGraphicsScene::GroupIdType)(item->data((int)FEGraphicsScene::Group2).toInt()) ==
                   FEGraphicsScene::PinNumbers) {
            pinnoLabel = (QGraphicsSimpleTextItem*)item;
        } else if ((FEGraphicsScene::GroupIdType)(item->data((int)FEGraphicsScene::Group2).toInt()) ==
                   FEGraphicsScene::PinLabels) {
            pinnameLabel = (QGraphicsSimpleTextItem*)item;
        }
    }
    QRectF coords = connection->rect();
    xpos = coords.x()+(coords.width()/2.0);
    ypos = coords.y()+(coords.height()/2.0);
    color = connection->brush().color();
    linethickness = leadline->pen().width();
    editpinno = connection->data((int)FEGraphicsScene::Pinno).toInt();
    font = pinnoLabel->font().family();
    size = pinnoLabel->font().pointSize();
    orientation = (Schematic::PinOrientationAndInversion::PinOrient)
          leadline->data((int)FEGraphicsScene::Orientation).toInt();
    length = leadline->data((int)FEGraphicsScene::Length).toDouble();
    if (pinnameLabel != NULL) {
        pinname = pinnameLabel->text();
    }
    if (addPinDialog->draw(xpos, ypos, orientation, length, linethickness, 
                           editpinno, pinname, font, size, color, true,
                           tr("Edit Pin"), tr("Edit"))) {
        stdError << "FESchematicEditor::editPin(): xpos is " << xpos <<
              " ypos is " << ypos << " orientation " << orientation <<
              " length is " << length << " linethickness is " << linethickness
              << " editpinno is " << editpinno << " pinname is " << pinname << 
              " font is " << font << " size is " << size << " color is " <<
              color << '\n';
        for (items_Iterator ii = items.begin();
             ii != items.end(); 
             ii++) {
            QGraphicsItem *item = *ii;
            canvas->removeItem(item);
            delete item;
        }
        if (editpinno > pinno)  pinno = editpinno;
        _addPin_helper(xpos,ypos,orientation,length,linethickness,pinno,
                       pinname,font,size,color);
    }
    
    
}


void FESchematicEditor::addRect()
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("black");
    bool filled = false;
    if (addRectDialog->draw(xpos, ypos, width, height, linethickness, color, filled, false, "Add Rectangle", "Add")) {
        gid++;
        QGraphicsItem *item;
        if (filled) {
            item = canvas->addRect(xpos,ypos,width,height,QPen(Qt::NoPen),QBrush(color));
        } else {
            item = canvas->addRect(xpos,ypos,width,height,QPen(QBrush(color),linethickness),QBrush(Qt::NoBrush));
        }
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Rect));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::editRect(int gid)
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("black");
    bool filled = false;
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FESchematicEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FESchematicEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Rect) {
        stdError << "FESchematicEditor::editRect(): item is not a Rect: " << gid << '\n';
        return;
    }
    QGraphicsRectItem *eitem = (QGraphicsRectItem *) item;
    QRectF coords = eitem->rect();
    xpos = coords.x();
    ypos = coords.y();
    width = coords.width();
    height = coords.height();
    QPen pen = eitem->pen();
    QBrush brush = eitem->brush();
    if (brush.style() != Qt::NoBrush) {
        filled = true;
        color  = brush.color();
    } else {
        filled = false;
        color  = pen.color();
        linethickness = pen.widthF();
    }
    if (addRectDialog->draw(xpos, ypos, width, height, linethickness, color, filled, true, "Edit Rectangle", "Edit")) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        QGraphicsItem *item;
        if (filled) {
            item = canvas->addRect(xpos,ypos,width,height,QPen(Qt::NoPen),QBrush(color));
        } else {
            item = canvas->addRect(xpos,ypos,width,height,QPen(QBrush(color),linethickness),QBrush(Qt::NoBrush));
        }
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Rect));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::addLine()
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("black");
    
    if (addLineDialog->draw(x1, y1, x2, y2, linethickness, color, false, tr("Add Line"), tr("Add"))) {
        gid++;
        QGraphicsItem *item = canvas->addLine(x1, y1, x2, y2, QPen(QBrush(color),linethickness));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Line));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::editLine(int gid)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("black");
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FESchematicEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FESchematicEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Line) {
        stdError << "FESchematicEditor::editRect(): item is not a Line: " << gid << '\n';
        return;
    }
    QGraphicsLineItem *eitem = (QGraphicsLineItem *) item;
    QLineF coords = eitem->line();
    x1 = coords.x1();
    y1 = coords.y1();
    x2 = coords.x2();
    y2 = coords.y2();
    QPen pen = eitem->pen();
    color = pen.color();
    linethickness = pen.width();
    if (addLineDialog->draw(x1, y1, x2, y2, linethickness, color, true, tr("Add Line"), tr("Add"))) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        QGraphicsItem *item = canvas->addLine(x1, y1, x2, y2, QPen(QBrush(color),linethickness));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Line));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::addCirc()
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0;
    QColor color("black");
    
    if (addCircDialog->draw(xpos, ypos, diameter, center, color, false, tr("Add Circle"), tr("Add"))) {
        gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        QPen pen(Qt::NoPen);
        QBrush brush(Qt::NoBrush);
        if (center > 0) {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(color);
            pen.setWidthF((diameter-center)/2.0);
        } else {
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(color);
        }
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Circ));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::editCirc(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0;
    QColor color("black");
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid));
    if (items.size() == 0) {return;}
    if (items.size() > 1) {
        // Should not happen!
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Circ) {
        // Should not happen!
        return;
    }
    QGraphicsEllipseItem *eitem = (QGraphicsEllipseItem *) item;
    QRectF coords = eitem->rect();
    xpos = coords.x()+(coords.width()/2.0);
    ypos = coords.y()+(coords.height()/2.0);
    diameter = (coords.height()+coords.width())/2.0;
    QPen pen = eitem->pen();
    QBrush brush = eitem->brush();
    if (pen.style() != Qt::NoPen) {
        color = pen.color();
        center = diameter - (pen.widthF()*2.0);
    } else {
        color = brush.color();
        center = 0.0;
    }
    if (addCircDialog->draw(xpos, ypos, diameter, center, color, true, tr("Edit Circle"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        QPen pen(Qt::NoPen);
        QBrush brush(Qt::NoBrush);
        if (center > 0) {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(color);
            pen.setWidthF((diameter-center)/2.0);
        } else {
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(color);
        }
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Circ));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::addArc()
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0, start = 0, extent = 0;
    QColor color("black");
    if (addArcDialog->draw(xpos, ypos, diameter, center, start, extent, color, false, tr("Add Arc"), tr("Add"))) {
        gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        QPen pen(Qt::NoPen);
        QBrush brush(Qt::NoBrush);
        if (center > 0) {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(color);
            pen.setWidthF((diameter-center)/2.0);
        } else {
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(color);
        }
        QPainterPath path;
        if (brush.style() != Qt::NoBrush) {
            path = QPainterPath(QPointF(xpos,ypos));
        } else {
            double dx = cos(radians(start))*radius;
            double dy = sin(radians(start))*radius;
            //stdError << "FESchematicEditor::addArc() at angle " << start << " dx = " << dx << " and dy = " << dy << '\n';
            if (start < 90) {
                path = QPainterPath(QPointF(xpos+dx, ypos+dy));
            } else if (start < 180) {
                path = QPainterPath(QPointF(xpos+dx, ypos-dy));
            } else if (start < 270) {
                path = QPainterPath(QPointF(xpos+dx, ypos+dy));
            } else {
                path = QPainterPath(QPointF(xpos+dx, ypos-dy));
            }
        }
        path.arcTo(QRectF(x,y,w,h),start,extent);
        if (brush.style() != Qt::NoBrush) {
            path.closeSubpath();
        }
        //stdError << "FESchematicEditor::addArc() path is " << path << '\n';
        QGraphicsItem *item = canvas->addPath(path,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Arc));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
        item->setData((int)FEGraphicsScene::ArcXPos,QVariant(xpos));
        item->setData((int)FEGraphicsScene::ArcYPos,QVariant(ypos));
        item->setData((int)FEGraphicsScene::ArcDiameter,QVariant(diameter));
        item->setData((int)FEGraphicsScene::ArcCenter,QVariant(center));
        item->setData((int)FEGraphicsScene::ArcStart,QVariant(start));
        item->setData((int)FEGraphicsScene::ArcExtent,QVariant(extent));
                                                       
    }
}

void FESchematicEditor::editArc(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0, start = 0, extent = 0;
    QColor color("black");
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid));
    if (items.size() == 0) {return;}
    if (items.size() > 1) {
        // Should not happen!
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Arc) {
        // Should not happen!
        return;
    }
    xpos = item->data((int)FEGraphicsScene::ArcXPos).toDouble();
    ypos = item->data((int)FEGraphicsScene::ArcYPos).toDouble();
    diameter = item->data((int)FEGraphicsScene::ArcDiameter).toDouble();
    center = item->data((int)FEGraphicsScene::ArcCenter).toDouble();
    start = item->data((int)FEGraphicsScene::ArcStart).toDouble();
    extent = item->data((int)FEGraphicsScene::ArcExtent).toDouble();
    QGraphicsPathItem *eitem = (QGraphicsPathItem *) item;
    QPen pen = eitem->pen();
    QBrush brush = eitem->brush();
    if (pen.style() != Qt::NoPen) {
        color = pen.color();
    } else {
        color = brush.color();
    }
    if (addArcDialog->draw(xpos, ypos, diameter, center, start, extent, color, true, tr("Exit Arc"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        QPen pen(Qt::NoPen);
        QBrush brush(Qt::NoBrush);
        if (center > 0) {
            pen.setStyle(Qt::SolidLine);
            pen.setColor(color);
            pen.setWidthF((diameter-center)/2.0);
        } else {
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(color);
        }
        
        QPainterPath path;
        if (brush.style() != Qt::NoBrush) {
            path = QPainterPath(QPointF(xpos,ypos));
        } else {
            double dx = cos(radians(start))*radius;
            double dy = sin(radians(start))*radius;
            //stdError << "FESchematicEditor::editArc() at angle " << start << " dx = " << dx << " and dy = " << dy << '\n';
            if (start < 90) {
                path = QPainterPath(QPointF(xpos+dx, ypos+dy));
            } else if (start < 180) {
                path = QPainterPath(QPointF(xpos+dx, ypos-dy));
            } else if (start < 270) {
                path = QPainterPath(QPointF(xpos+dx, ypos+dy));
            } else {
                path = QPainterPath(QPointF(xpos+dx, ypos-dy));
            }
        }
        path.arcTo(QRectF(x,y,w,h),start,extent);
        QGraphicsItem *item = canvas->addPath(path,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Arc));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
        item->setData((int)FEGraphicsScene::ArcXPos,QVariant(xpos));
        item->setData((int)FEGraphicsScene::ArcYPos,QVariant(ypos));
        item->setData((int)FEGraphicsScene::ArcDiameter,QVariant(diameter));
        item->setData((int)FEGraphicsScene::ArcCenter,QVariant(center));
        item->setData((int)FEGraphicsScene::ArcStart,QVariant(start));
        item->setData((int)FEGraphicsScene::ArcExtent,QVariant(extent));
    }
}

void FESchematicEditor::addPoly()
{
    QPolygonF points;
    double linethickness;
    bool filled;
    QColor color;
    if (addPolyDialog->draw(points, linethickness, filled, color, false, tr("Add Polygon"), tr("Add"))) {
        gid++;
        QGraphicsItem *item;
        if (filled) {
            item = canvas->addPolygon(points,QPen(Qt::NoPen),QBrush(color));
        } else {
            item = canvas->addPolygon(points,QPen(QBrush(color),linethickness),QBrush(Qt::NoBrush));
        }
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Poly));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::editPoly(int gid)
{
    QPolygonF points;
    double linethickness;
    bool filled;
    QColor color;
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FESchematicEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FESchematicEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Poly) {
        stdError << "FESchematicEditor::editPoly(): item is not a Poly: " << gid << '\n';
        return;
    }
    QGraphicsPolygonItem *eitem = (QGraphicsPolygonItem *) item;
    points = eitem->polygon();
    QPen pen = eitem->pen();
    QBrush brush = eitem->brush();
    if (brush.style() != Qt::NoBrush) {
        filled = true;
        color  = brush.color();
    } else {
        filled = false;
        color  = pen.color();
        linethickness = pen.widthF();
    }
    if (addPolyDialog->draw(points, linethickness, filled, color, true, tr("Edit Polygon"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        QGraphicsItem *item;
        if (filled) {
            item = canvas->addPolygon(points,QPen(Qt::NoPen),QBrush(color));
        } else {
            item = canvas->addPolygon(points,QPen(QBrush(color),linethickness),QBrush(Qt::NoBrush));
        }
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Poly));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::addText()
{
    double xpos = 0, ypos = 0;
    int size = 0;;
    QString text(""), font("DroidSans");
    QColor color("black");
    
    if (addTextDialog->draw(xpos, ypos, text, font, size, color, false, tr("Add Text"), tr("Add"))) {
        gid++;
        QGraphicsSimpleTextItem *titem = canvas->addSimpleText(text,QFont(font,size));
        titem->setBrush(color);
        titem->setPos(xpos,ypos);
        titem->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        titem->setData((int)FEGraphicsScene::Type,
                       QVariant((int)FEGraphicsScene::Text));
        titem->setData((int)FEGraphicsScene::Group1,
                       QVariant((int)FEGraphicsScene::Schematic));
    }
}

void FESchematicEditor::editText(int gid)
{
    double xpos = 0, ypos = 0;
    int size = 0;
    QString text(""), font("DroidSans");
    QColor color("black");
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FESchematicEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FESchematicEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Text) {
        stdError << "FESchematicEditor::editText(): item is not a Text: " << gid << '\n';
        return;
    }
    QGraphicsSimpleTextItem *titem = (QGraphicsSimpleTextItem *) item;
    color = titem->brush().color();
    QPointF pos = titem->pos();
    xpos = pos.x(); 
    ypos = pos.y();
    text = titem->text();
    QFont f = titem->font();
    font = f.family();
    size = f.pointSize();
    if (addTextDialog->draw(xpos, ypos, text, font, size, color, true, tr("Edit Text"), tr("Edit"))) {
        canvas->removeItem(titem);
        delete titem;
        gid++;
        QGraphicsSimpleTextItem *titem = canvas->addSimpleText(text,QFont(font,size));
        titem->setBrush(color);
        titem->setPos(xpos,ypos);
        titem->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        titem->setData((int)FEGraphicsScene::Type,
                       QVariant((int)FEGraphicsScene::Text));
        titem->setData((int)FEGraphicsScene::Group1,
                       QVariant((int)FEGraphicsScene::Schematic));
    }
}



void FESchematicEditor::loadFile(const QString &filename)
{
}

void FESchematicEditor::saveFile(const QString &filename)
{
}

