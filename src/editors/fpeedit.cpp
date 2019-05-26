// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:51:02 2019
//  Last Modified : <190525.2140>
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
#include <QDomDocument>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "../support/debug.h"
#include "fpeedit.h"
#include "fpedialogs.h"
#include "../svgsupport/svgelement.h"

FEPCBEditor::FEPCBEditor(SizeAndVP::UnitsType units,double width,double height,const QRectF &viewport,QWidget *parent)
      : FEEdit(units,width,height,viewport,parent)
{
    addPinDialog = new PCB::AddPinDialog(this);
    addRectDialog = new PCB::AddRectDialog(this);
    addLineDialog = new PCB::AddLineDialog(this);
    addCircDialog = new PCB::AddCircDialog(this);
    addArcDialog = new PCB::AddArcDialog(this);
    addPolyDialog = new PCB::AddPolyDialog(this);
    addTextDialog = new PCB::AddTextDialog(this);
    setBackgroundBrush(QBrush(QColor("darkgrey")));
}

FEPCBEditor::~FEPCBEditor()
{
}


void FEPCBEditor::addPin()
{
    double xpos = 0, ypos = 0, diameter = 1, drill = 0;;
    QColor color("#FFBF00");
    
    pinno++;
    if (addPinDialog->draw(xpos, ypos, diameter, drill, pinno, false, tr("Add Pin"), tr("Add"))) {
        gid++;
        stdError << "FEPCBEditor::addPin(): gid = " << gid << '\n';
        stdError << "FEPCBEditor::addPin(): xpos = " << xpos << ", ypos = " << ypos << '\n';
        stdError << "FEPCBEditor::addPin(): diameter = " << diameter << '\n';
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        QPen pen(Qt::NoPen);
        QBrush brush(Qt::NoBrush);
        if (drill == 0) {
            brush.setColor(color);
            brush.setStyle(Qt::SolidPattern);
        } else {
            pen.setColor(color);
            pen.setStyle(Qt::SolidLine);
            double dradius = drill / 2.0;
            pen.setWidth(radius-dradius);
        }
        //stdError << "FEPCBEditor::addPin(): x = " << x << ", y = " << y 
        //      << ", w = " << w << ", h = " << h << '\n';
        //stdError << "FEPCBEditor::addPin(): pen is " << pen << '\n';
        //stdError << "FEPCBEditor::addPin(): brush is " << brush << '\n';
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinno));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
}

void FEPCBEditor::editPin(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, drill = 0;
    QColor color("#FFBF00");
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
    stdError << "FEPCBEditor::editPin(): coords.x() = " << coords.x() << ", coords.y() = " << coords.y() << ", coords.width() = " << coords.width() << ", coords.height() = " << coords.height() << '\n';
    xpos = coords.x()+(coords.width()/2.0);
    ypos = coords.y()+(coords.height()/2.0);
    diameter = (coords.height()+coords.width())/2.0;
    stdError << "FEPCBEditor::editPin(): xpos = " << xpos << ", ypos = " << ypos << ", diameter = " << diameter << '\n';
    stdError << "FEPCBEditor::editPin(): xpos = " << xpos << ", ypos = " << ypos << ", diameter = " << diameter << '\n';
    if (eitem->pen().style() != Qt::NoPen) {
        double w = eitem->pen().width();
        double dradius = (diameter/2.0)-w;
        drill = dradius*2;
    }
    editpinno = eitem->data((int)FEGraphicsScene::Pinno).toInt();
    
    if (addPinDialog->draw(xpos, ypos, diameter, drill, editpinno, true, tr("Edit Pin"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        if (editpinno > pinno) pinno = editpinno;
        gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        QPen pen(Qt::NoPen);
        QBrush brush(Qt::NoBrush);
        if (drill == 0) {
            brush.setColor(color);
            brush.setStyle(Qt::SolidPattern);
        } else {
            pen.setColor(color);
            pen.setStyle(Qt::SolidLine);
            double dradius = drill / 2.0;
            pen.setWidth(radius-dradius);
        }
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinno));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
}

void FEPCBEditor::addRect()
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("white");
    
    if (addRectDialog->draw(xpos, ypos, width, height, linethickness, false, "Add Rectangle", "Add")) {
        gid++;
        QGraphicsItem *item;
        item = canvas->addRect(xpos,ypos,width,height,QPen(color,linethickness),QBrush(Qt::NoBrush));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Rect));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::editRect(int gid)
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("white");
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEPCBEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEPCBEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Rect) {
        stdError << "FEPCBEditor::editRect(): item is not a Rect: " << gid << '\n';
        return;
    }
    QGraphicsRectItem *eitem = (QGraphicsRectItem *) item;
    QRectF coords = eitem->rect();
    xpos = coords.x();
    ypos = coords.y();
    width = coords.width();
    height = coords.height();
    QPen pen = eitem->pen();
    linethickness = pen.widthF();
    if (addRectDialog->draw(xpos, ypos, width, height, linethickness, true, "Edit Rectangle", "Edit")) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        QGraphicsItem *item;
        item = canvas->addRect(xpos,ypos,width,height,QPen(color,linethickness),QBrush(Qt::NoBrush));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Rect));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::addLine()
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("white");
    
    if (addLineDialog->draw(x1, y1, x2, y2, linethickness, false, tr("Add Line"), tr("Add"))) {
        gid++;
        QGraphicsItem *item = canvas->addLine(x1, y1, x2, y2, QPen(QBrush(color),linethickness));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Line));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::editLine(int gid)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("white");
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEPCBEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEPCBEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Line) {
        stdError << "FEPCBEditor::editRect(): item is not a Line: " << gid << '\n';
        return;
    }
    QGraphicsLineItem *eitem = (QGraphicsLineItem *) item;
    QLineF coords = eitem->line();
    x1 = coords.x1();
    y1 = coords.y1();
    x2 = coords.x2();
    y2 = coords.y2();
    QPen pen = eitem->pen();
    linethickness = pen.width();
    if (addLineDialog->draw(x1, y1, x2, y2, linethickness, true, tr("Add Line"), tr("Add"))) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        QGraphicsItem *item = canvas->addLine(x1, y1, x2, y2, QPen(QBrush(color),linethickness));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Line));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::addCirc()
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0;
    QColor color("white");
    
    if (addCircDialog->draw(xpos, ypos, diameter, center, false, tr("Add Circle"), tr("Add"))) {
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
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::editCirc(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0;
    QColor color("white");
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
        center = diameter - (pen.widthF()*2.0);
    } else {
        center = 0.0;
    }
    if (addCircDialog->draw(xpos, ypos, diameter, center, true, tr("Edit Circle"), tr("Edit"))) {
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
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::addArc()
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0, start = 0, extent = 0;
    QColor color("white");
    if (addArcDialog->draw(xpos, ypos, diameter, center, start, extent, false, tr("Add Arc"), tr("Add"))) {
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
            //stdError << "FEPCBEditor::addArc() at angle " << start << " dx = " << dx << " and dy = " << dy << '\n';
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
        //stdError << "FEPCBEditor::addArc() path is " << path << '\n';
        QGraphicsItem *item = canvas->addPath(path,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Arc));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
        item->setData((int)FEGraphicsScene::ArcXPos,QVariant(xpos));
        item->setData((int)FEGraphicsScene::ArcYPos,QVariant(ypos));
        item->setData((int)FEGraphicsScene::ArcDiameter,QVariant(diameter));
        item->setData((int)FEGraphicsScene::ArcCenter,QVariant(center));
        item->setData((int)FEGraphicsScene::ArcStart,QVariant(start));
        item->setData((int)FEGraphicsScene::ArcExtent,QVariant(extent));
                                                       
    }
}

void FEPCBEditor::editArc(int gid)
{
    double xpos = 0, ypos = 0, diameter = 1, center = 0, start = 0, extent = 0;
    QColor color("white");
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
    if (addArcDialog->draw(xpos, ypos, diameter, center, start, extent, true, tr("Exit Arc"), tr("Edit"))) {
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
            //stdError << "FEPCBEditor::editArc() at angle " << start << " dx = " << dx << " and dy = " << dy << '\n';
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
                      QVariant((int)FEGraphicsScene::Silkscreen));
        item->setData((int)FEGraphicsScene::ArcXPos,QVariant(xpos));
        item->setData((int)FEGraphicsScene::ArcYPos,QVariant(ypos));
        item->setData((int)FEGraphicsScene::ArcDiameter,QVariant(diameter));
        item->setData((int)FEGraphicsScene::ArcCenter,QVariant(center));
        item->setData((int)FEGraphicsScene::ArcStart,QVariant(start));
        item->setData((int)FEGraphicsScene::ArcExtent,QVariant(extent));
    }
}

void FEPCBEditor::addPoly()
{
    QPolygonF points;
    double linethickness;
    QColor color("white");
    
    if (addPolyDialog->draw(points, linethickness, false, tr("Add Polygon"), tr("Add"))) {
        gid++;
        QGraphicsItem *item;
        item = canvas->addPolygon(points,QPen(QBrush(color),linethickness),QBrush(Qt::NoBrush));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Poly));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::editPoly(int gid)
{
    QPolygonF points;
    double linethickness;
    QColor color("white");
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEPCBEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEPCBEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Poly) {
        stdError << "FEPCBEditor::editPoly(): item is not a Poly: " << gid << '\n';
        return;
    }
    QGraphicsPolygonItem *eitem = (QGraphicsPolygonItem *) item;
    points = eitem->polygon();
    QPen pen = eitem->pen();
    QBrush brush = eitem->brush();
    linethickness = pen.widthF();
    if (addPolyDialog->draw(points, linethickness, true, tr("Edit Polygon"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        gid++;
        QGraphicsItem *item;
        item = canvas->addPolygon(points,QPen(QBrush(color),linethickness),QBrush(Qt::NoBrush));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Poly));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::addText()
{
    double xpos = 0, ypos = 0;
    int size = 0;;
    QString text(""), font("DroidSans");
    QColor color("white");
    
    if (addTextDialog->draw(xpos, ypos, text, font, size, false, tr("Add Text"), tr("Add"))) {
        gid++;
        QGraphicsSimpleTextItem *titem = canvas->addSimpleText(text,QFont(font,size));
        titem->setBrush(color);
        titem->setPos(xpos,ypos);
        titem->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        titem->setData((int)FEGraphicsScene::Type,
                       QVariant((int)FEGraphicsScene::Text));
        titem->setData((int)FEGraphicsScene::Group1,
                       QVariant((int)FEGraphicsScene::Silkscreen));
    }
}

void FEPCBEditor::editText(int gid)
{
    double xpos = 0, ypos = 0;
    int size = 0;
    QString text(""), font("DroidSans");
    QColor color("white");
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEPCBEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEPCBEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Text) {
        stdError << "FEPCBEditor::editText(): item is not a Text: " << gid << '\n';
        return;
    }
    QGraphicsSimpleTextItem *titem = (QGraphicsSimpleTextItem *) item;
    QPointF pos = titem->pos();
    xpos = pos.x(); 
    ypos = pos.y();
    text = titem->text();
    QFont f = titem->font();
    font = f.family();
    size = f.pointSize();
    if (addTextDialog->draw(xpos, ypos, text, font, size, true, tr("Edit Text"), tr("Edit"))) {
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
                       QVariant((int)FEGraphicsScene::Silkscreen));
    }
}



void FEPCBEditor::loadFile(const QString &filename)
{
    QFile infile(filename);
    infile.open( QIODevice::ReadOnly );
    QDomDocument fbeSVGDocument;
    QString errorMessage;
    int errorLine, errorColumn;
    if (!fbeSVGDocument.setContent(&infile, true, &errorMessage, &errorLine, &errorColumn)) {
        // error report...
        infile.close();
        return;
    }
    QDomNodeList svgList = fbeSVGDocument.elementsByTagName("svg");
    if (svgList.length() == 0) {
        // error report: missing <svg></svg>
        return;
    } else if (svgList.length() > 1) {
        // error report: multiple <svg></svg> (?)
        return;
    }
    QDomElement svg = svgList.item(0).toElement();
    QRegularExpression floatUnits("^([[:digit:]Ee.+-]+)(.*)$");
    QRegularExpressionMatch match;
    qreal width = 0, height = 0;
    QRectF vp;
    if (svg.attribute("width").contains(floatUnits,&match)) {
        width = match.captured(1).toFloat();
        if (match.captured(2) == "in") {
            width *= 25.4;
        }
    }
    if (svg.attribute("height").contains(floatUnits,&match)) {
        height = match.captured(1).toFloat();
        if (match.captured(2) == "in") {
            height *= 25.4;
        }
    }
    QRegularExpression fourFloats("^([[:digit:]Ee.+-]+)[[:space:]]+([[:digit:]Ee.+-]+)[[:space:]]+([[:digit:]Ee.+-]+)[[:space:]]+([[:digit:]Ee.+-]+)$");
    if (svg.attribute("viewBox").contains(fourFloats,&match)) {
        vp.setX(match.captured(1).toFloat());
        vp.setY(match.captured(2).toFloat());
        vp.setWidth(match.captured(3).toFloat());
        vp.setHeight(match.captured(4).toFloat());
    }
    updateSize(vp,width,height,SizeAndVP::mm);
    
    QDomNodeList children = svg.childNodes();
    for (int i = 0; i < children.length(); i++) {
        QDomNode n = children.at(i);
        if (n.isElement()) {
            QDomElement e = n.toElement();
            if (e.tagName() == "g" && (e.attribute("id") == "silkscreen" ||
                                       e.attribute("id") == "copper0" ||
                                       e.attribute("id") == "copper1")) {
                processSVGGroup(e);
            }
        }
    }
    
    
    infile.close();
    setClean();
}

void FEPCBEditor::processSVGTag(QDomElement &element, QDomElement &parentGroup)
{
    QString parentGroupId(parentGroup.attribute("id"));
    if (element.tagName() == "g") {
        processSVGGroup(element);
    } else if (element.tagName() == "circle") {
        processCircle(element,parentGroupId);
    } else if (element.tagName() == "rect") {
        processRect(element,parentGroupId);
    } else if (element.tagName() == "line") {
        processLine(element,parentGroupId);
    } else if (element.tagName() == "path") {
        processPath(element,parentGroupId);
    } else if (element.tagName() == "ellipse") {
        processEllipse(element,parentGroupId);
    } else if (element.tagName() == "polygon") {
        processPolygon(element,parentGroupId);
    } else if (element.tagName() == "text") {
        processText(element,parentGroupId);
    }
}

void FEPCBEditor::processCircle(QDomElement &element, QString parentID)
{
    QRegularExpression connectorExpr("^connector([[:digit:]]+)pin$");
    QRegularExpressionMatch match;
    int pinnumber = 0;
    double xpos = 0, ypos = 0, diameter = 1;
    bool isPin = false;
     
    gid++;
    if (element.attribute("id").contains(connectorExpr,&match)) {
        pinnumber = match.captured(1).toInt();
        isPin = true;
    }
    xpos = element.attribute("cx").toFloat();
    ypos = element.attribute("cy").toFloat();
    diameter = element.attribute("r").toFloat() * 2;
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    if (element.attribute("fill") == "none") {
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(element.attribute("stroke")));
        pen.setWidthF(element.attribute("stroke-width").toFloat());
    } else {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(element.attribute("fill")));
    }
    double radius=diameter/2.0;
    qreal x = xpos-radius;
    qreal y = ypos-radius;
    qreal w = diameter;
    qreal h = diameter;
    QGraphicsItem *item = canvas->addEllipse(x,y,w,h,pen,brush);
    item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    //stdError << "FEBreadboardEditor::processCircle(): gid = " << gid << "\n";
    if (parentID == "silkscreen") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    } else if (parentID == "copper0" || parentID == "copper1") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
    if (isPin) {
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinnumber));
        if (pinnumber > pinno) pinno = pinnumber;
        //stdError << "FEBreadboardEditor::processCircle(): pinnumber = " << pinnumber << "\n";
    } else {
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Circ));
    }
}

void FEPCBEditor::processRect(QDomElement &element, QString parentID)
{
    double xpos = 0, ypos = 0, width = 1, height = 1;
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    gid++;
    xpos = element.attribute("x").toFloat();
    ypos = element.attribute("y").toFloat();
    width = element.attribute("width").toFloat();
    height = element.attribute("height").toFloat();
    //stdError << "FEBreadboardEditor::processRect(): element.attribute(\"fill\") " << element.attribute("fill") << "\n"; 
    if (element.attribute("fill") != "none") {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(element.attribute("fill")));
    } else {
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(element.attribute("stroke")));
        pen.setWidthF(element.attribute("stroke-width").toFloat());
    }
    QGraphicsItem *item = canvas->addRect(xpos,ypos,width,height,pen,brush);
    item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    //stdError << "FEBreadboardEditor::processRect(): gid = " << gid << "\n";
    item->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Rect));
    if (parentID == "silkscreen") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    } else if (parentID == "copper0" || parentID == "copper1") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
}

void FEPCBEditor::processLine(QDomElement &element, QString parentID)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    QPen pen(Qt::SolidLine);
    
    gid++;
    x1 = element.attribute("x1").toFloat();
    y1 = element.attribute("y1").toFloat();
    x2 = element.attribute("x2").toFloat();
    y2 = element.attribute("y2").toFloat();
    pen.setColor(QColor(element.attribute("stroke")));
    pen.setWidthF(element.attribute("stroke-width").toFloat());
    QGraphicsItem *item = canvas->addLine(x1, y1, x2, y2, pen);
    item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    item->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Line));
    if (parentID == "silkscreen") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    } else if (parentID == "copper0" || parentID == "copper1") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
}

void FEPCBEditor::processPath(QDomElement &element, QString parentID)
{
//    if (parentID == "silkscreen") {
//        item->setData((int)FEGraphicsScene::Group1,
//                      QVariant((int)FEGraphicsScene::Silkscreen));
//    } else if (parentID == "copper0" || parentID == "copper1") {
//        item->setData((int)FEGraphicsScene::Group1,
//                      QVariant((int)FEGraphicsScene::Copper0));
//        item->setData((int)FEGraphicsScene::Group2,
//                      QVariant((int)FEGraphicsScene::Copper1));
//    }
}

void FEPCBEditor::processEllipse(QDomElement &element, QString parentID)
{
    QRegularExpression connectorExpr("^connector([[:digit:]]+)pin$");
    QRegularExpressionMatch match;
    int pinnumber;
    double xpos = 0, ypos = 0, rx = 0, ry = 0;
    bool isPin = false;
     
    gid++;
    if (element.attribute("id").contains(connectorExpr,&match)) {
        pinnumber = match.captured(1).toInt();
        isPin = true;
    }
    xpos = element.attribute("cx").toFloat();
    ypos = element.attribute("cy").toFloat();
    rx = element.attribute("rx").toFloat();
    ry = element.attribute("ry").toFloat();
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    if (element.attribute("fill") == "none") {
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(element.attribute("stroke")));
        pen.setWidthF(element.attribute("stroke-width").toFloat());
    } else {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(element.attribute("fill")));
    }
    qreal x = xpos-rx;
    qreal y = ypos-ry;
    qreal w = rx*2;
    qreal h = ry*2;
    QGraphicsItem *item = canvas->addEllipse(x,y,w,h,pen,brush);
    item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    //stdError << "FEBreadboardEditor::processEllipse(): gid = " << gid << "\n";
    if (parentID == "silkscreen") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    } else if (parentID == "copper0" || parentID == "copper1") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
    if (isPin) {
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinnumber));
        if (pinnumber > pinno) pinno = pinnumber;
        //stdError << "FEBreadboardEditor::processEllipse(): pinnumber = " << pinnumber << "\n";
    } else {
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Circ));
    }
}

void FEPCBEditor::processText(QDomElement &element, QString parentID)
{
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    QFont font;
    gid++;
    if (element.attribute("fill") != "none") {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(element.attribute("fill")));
    } else {
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(element.attribute("stroke")));
        pen.setWidthF(element.attribute("stroke-width").toFloat());
    }
    font.setFamily(element.attribute("font-family"));
    font.setPointSize(element.attribute("font-size").toFloat());
    qreal x = element.attribute("x").toFloat();
    qreal y = element.attribute("y").toFloat();
    QDomText txt = element.firstChild().toText();
    QGraphicsSimpleTextItem *titem = canvas->addSimpleText(txt.data(),font);
    titem->setBrush(brush);
    titem->setPen(pen);
    titem->setPos(x,y);
    titem->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    //stdError << "FEBreadboardEditor::processText(): gid = " << gid << "\n";
    titem->setData((int)FEGraphicsScene::Type,
                   QVariant((int)FEGraphicsScene::Text));
    if (parentID == "silkscreen") {
        titem->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    } else if (parentID == "copper0" || parentID == "copper1") {
        titem->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        titem->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
}

void FEPCBEditor::processPolygon(QDomElement &element, QString parentID)
{
    QPolygonF points;
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    gid++;
    if (element.attribute("fill") != "none") {
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(QColor(element.attribute("fill")));
    } else {
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(element.attribute("stroke")));
        pen.setWidthF(element.attribute("stroke-width").toFloat());
    }
    QRegularExpression pointExpr("^([[:digit:].Ee+-]+)[[:space:],]+([[:digit:].Ee+-]+)[[:space:],]*(.*)$");
    QRegularExpressionMatch match;
    QString pointString = element.attribute("points");
    while (pointString.contains(pointExpr,&match)) {
        QPointF p(match.captured(1).toFloat(),match.captured(2).toFloat());
        pointString = match.captured(3);
        points.push_back(p);
    }
    QGraphicsItem *item = canvas->addPolygon(points,pen,brush);
    item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
    item->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Poly));
    if (parentID == "silkscreen") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Silkscreen));
    } else if (parentID == "copper0" || parentID == "copper1") {
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Copper0));
        item->setData((int)FEGraphicsScene::Group2,
                      QVariant((int)FEGraphicsScene::Copper1));
    }
}



void FEPCBEditor::saveFile(const QString &filename)
{
    QDomDocumentType fbeSVGDocumentType;
    QDomDocument fbeSVGDocument(fbeSVGDocumentType);
    QDomElement svg = fbeSVGDocument.createElement("svg");
    fbeSVGDocument.appendChild(svg);
    svg.setAttribute("version","1.1");
    svg.setAttribute("xmlns","http://www.w3.org/2000/svg");
    svg.setAttribute("xmlns:xlink","http://www.w3.org/1999/xlink");
    
    SizeAndVP::UnitsType u = Units();
    QString usuffix;
    switch (u) {
    case SizeAndVP::mm: usuffix = "mm"; break;
    case SizeAndVP::in: usuffix = "in"; break;
    }
    svg.setAttribute("x","0.0"+usuffix);
    svg.setAttribute("y","0.0"+usuffix);
    svg.setAttribute("width",QString::number(Width())+usuffix);
    svg.setAttribute("height",QString::number(Height())+usuffix);
    QRectF vp;
    Viewport(vp);
    svg.setAttribute("viewBox",QString::number(vp.x())+" "+
                     QString::number(vp.y())+" "+
                     QString::number(vp.width())+" "+
                     QString::number(vp.height()));
    QDomElement silkscreenGroup = fbeSVGDocument.createElement("g");
    svg.appendChild(silkscreenGroup);
    silkscreenGroup.setAttribute("id","silkscreen");
    
    QDomElement copper0Group = fbeSVGDocument.createElement("g");
    svg.appendChild(copper0Group);
    copper0Group.setAttribute("id","copper0");
    
    QDomElement copper1Group = fbeSVGDocument.createElement("g");
    copper0Group.appendChild(copper1Group);
    copper1Group.setAttribute("id","copper1");
    
    ItemList silkscreenItems = canvas->withtagEQ(FEGraphicsScene::Group1,QVariant((int)FEGraphicsScene::Silkscreen));
    for (items_Iterator ii = silkscreenItems.begin(); ii != silkscreenItems.end(); ii++) {
        QGraphicsItem *item = *ii;
        FESvgElement newElement(item);
        newElement.appendElement(silkscreenGroup);
    }
    ItemList copper1Items = canvas->withtagEQ(FEGraphicsScene::Group2,QVariant((int)FEGraphicsScene::Copper1));
    for (items_Iterator ii = copper1Items.begin(); ii != copper1Items.end(); ii++) {
        QGraphicsItem *item = *ii;
        FESvgElement newElement(item);
        int pinno = item->data((int)FEGraphicsScene::Pinno).toInt();
        if (pinno > 0) {
            newElement.setId(QString("connector%1pin").arg(pinno));
        } else {
            newElement.setId("");
        }
        newElement.appendElement(copper1Group);
    }
    QFile outfile(filename);
    outfile.open( QIODevice::WriteOnly );
    QString XML = fbeSVGDocument.toString();
    //stdError << "FEPCBEditor::saveFile(): " << XML << "\n";
    outfile.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" );
    outfile.write( XML.toUtf8() );
    outfile.close();
    setClean();
}

