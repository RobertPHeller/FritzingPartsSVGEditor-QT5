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
//  Last Modified : <190522.1334>
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
#include "../installedfonts.h"

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
        //stdError << "FEBreadboardEditor::addPin(): gid = " << gid << '\n';
        //stdError << "FEBreadboardEditor::addPin(): xpos = " << xpos << ", ypos = " << ypos << '\n';
        //stdError << "FEBreadboardEditor::addPin(): diameter = " << diameter << '\n';
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
        //stdError << "FEBreadboardEditor::addPin(): x = " << x << ", y = " << y 
        //      << ", w = " << w << ", h = " << h << '\n';
        QGraphicsItem *item = canvas->addEllipse(x,y,w,h,QPen(Qt::NoPen),QBrush(color));
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Pinno,QVariant(pinno));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Pin));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Breadboard));
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
    stdError << "FEBreadboardEditor::editPin(): xpos = " << xpos << ", ypos = " << ypos << ", diameter = " << diameter << '\n';
    color = eitem->brush().color();
    editpinno = eitem->data((int)FEGraphicsScene::Pinno).toInt();
    
    if (addPinDialog->draw(xpos, ypos, diameter, color, editpinno, true, tr("Edit Pin"), tr("Edit"))) {
        canvas->removeItem(eitem);
        delete eitem;
        if (editpinno > pinno) pinno = editpinno;
        gid++;
        double radius=diameter/2.0;
        qreal x = xpos-radius;
        qreal y = ypos-radius;
        qreal w = diameter;
        qreal h = diameter;
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::editRect(int gid)
{
    double xpos = 0, ypos = 0, width = 1, height = 1, linethickness = 1;
    QColor color("black");
    bool filled = false;
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEBreadboardEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEBreadboardEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Rect) {
        stdError << "FEBreadboardEditor::editRect(): item is not a Rect: " << gid << '\n';
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::addLine()
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::editLine(int gid)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0, linethickness = 1;
    QColor color("black");
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEBreadboardEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEBreadboardEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Line) {
        stdError << "FEBreadboardEditor::editRect(): item is not a Line: " << gid << '\n';
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::addCirc()
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::editCirc(int gid)
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::addArc()
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
            //stdError << "FEBreadboardEditor::addArc() at angle " << start << " dx = " << dx << " and dy = " << dy << '\n';
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
        //stdError << "FEBreadboardEditor::addArc() path is " << path << '\n';
        QGraphicsItem *item = canvas->addPath(path,pen,brush);
        item->setData((int)FEGraphicsScene::Gid,QVariant(gid));
        item->setData((int)FEGraphicsScene::Type,
                      QVariant((int)FEGraphicsScene::Arc));
        item->setData((int)FEGraphicsScene::Group1,
                      QVariant((int)FEGraphicsScene::Breadboard));
        item->setData((int)FEGraphicsScene::ArcXPos,QVariant(xpos));
        item->setData((int)FEGraphicsScene::ArcYPos,QVariant(ypos));
        item->setData((int)FEGraphicsScene::ArcDiameter,QVariant(diameter));
        item->setData((int)FEGraphicsScene::ArcCenter,QVariant(center));
        item->setData((int)FEGraphicsScene::ArcStart,QVariant(start));
        item->setData((int)FEGraphicsScene::ArcExtent,QVariant(extent));
                                                       
    }
}

void FEBreadboardEditor::editArc(int gid)
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
            //stdError << "FEBreadboardEditor::editArc() at angle " << start << " dx = " << dx << " and dy = " << dy << '\n';
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
                      QVariant((int)FEGraphicsScene::Breadboard));
        item->setData((int)FEGraphicsScene::ArcXPos,QVariant(xpos));
        item->setData((int)FEGraphicsScene::ArcYPos,QVariant(ypos));
        item->setData((int)FEGraphicsScene::ArcDiameter,QVariant(diameter));
        item->setData((int)FEGraphicsScene::ArcCenter,QVariant(center));
        item->setData((int)FEGraphicsScene::ArcStart,QVariant(start));
        item->setData((int)FEGraphicsScene::ArcExtent,QVariant(extent));
    }
}

void FEBreadboardEditor::addPoly()
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::editPoly(int gid)
{
    QPolygonF points;
    double linethickness;
    bool filled;
    QColor color;
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEBreadboardEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEBreadboardEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Poly) {
        stdError << "FEBreadboardEditor::editPoly(): item is not a Poly: " << gid << '\n';
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
                      QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::addText()
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
                       QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::editText(int gid)
{
    double xpos = 0, ypos = 0;
    int size = 0;
    QString text(""), font("DroidSans");
    QColor color("black");
    
    ItemList items = canvas->withtagEQ(FEGraphicsScene::Gid,QVariant(gid)); 
    if (items.size() == 0) {
        stdError << "FEBreadboardEditor::editRect(): no items matching GID: " << gid << '\n';
        return;
    }
    if (items.size() > 1) {
        stdError << "FEBreadboardEditor::editRect(): multiple items matching GID: " << gid << '\n';
        return;
    }
    QGraphicsItem *item = items[0];
    if ((FEGraphicsScene::ItemType)(item->data((int)FEGraphicsScene::Type).toInt()) != FEGraphicsScene::Text) {
        stdError << "FEBreadboardEditor::editText(): item is not a Text: " << gid << '\n';
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
                       QVariant((int)FEGraphicsScene::Breadboard));
    }
}

void FEBreadboardEditor::loadFile(const QString &filename)
{
}

void FEBreadboardEditor::saveFile(const QString &filename)
{
}


