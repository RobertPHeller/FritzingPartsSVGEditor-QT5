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
//  Last Modified : <190525.2037>
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
#include <QStyleOptionGraphicsItem>
#include <QDomDocument>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "../support/debug.h"
#include "fseedit.h"
#include "fsedialogs.h"
#include "../installedfonts.h"
#include "../svgsupport/svgelement.h"

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
    double cr = 0, ir = 0, px2 = 0, py2 = 0, lx = 0, ly = 0, nx = 0, ny = 0, 
          ix = 0, iy = 0;
    bool inverted = false;
    QGraphicsEllipseItem *connection = NULL, *invertCircle = NULL;
    QGraphicsLineItem *leadline = NULL;
    QGraphicsSimpleTextItem *pinnoLabel = NULL, *pinnameLabel = NULL;
    QString pinnoText(QString::number(pinno));
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
        ly  = ypos - (size/2.0);
        nx  = xpos + (length / 2.0) - ((.7*size)*pinnoText.length());
        ny  = (ypos - (linethickness * 2)) - (1.2*size);
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
        lx  = px2 - ((size*.7)*(pinname.length()+1));
        ly  = ypos - (.5*size);
        nx  = xpos - (length / 2.0);
        ny  = ypos - (linethickness * 2.0);
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
        lx  = px2 - ((size*.7)*(pinname.length()/2.0));
        ly  = py2;
        nx  = xpos + (linethickness * 2);
        ny  = ypos + (length / 2.0) - size;
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
        lx  = px2 - ((size*.7)*(pinname.length()/2.0));
        ly  = py2 - (size*1.5);
        nx  = xpos + (linethickness * 2);
        ny  = ypos - (length / 2.0) - (size*1.5);
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
    pinnoLabel = canvas->addSimpleText(pinnoText,QFont(font,size));
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
        stdError << "FESchematicEditor::editPin(): item is " << item << "\n";
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
    if (connection == NULL || leadline == NULL || pinnoLabel == NULL) return;
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
        _addPin_helper(xpos,ypos,orientation,length,linethickness,editpinno,
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
    pinSVGList.clear();
    QDomNodeList children = svg.childNodes();
    for (int i = 0; i < children.length(); i++) {
        QDomNode n = children.at(i);
        if (n.isElement()) {
            QDomElement e = n.toElement();
            if (e.tagName() == "g" && e.attribute("id") == "schematic") {
                processSVGGroup(e);
            }
        }
    }
    pinSVGList.finalize();
    infile.close();
    setClean();
}

void FESchematicEditor::processSVGTag(QDomElement &element, QDomElement &parentGroup)
{
    QString parentGroupId = parentGroup.attribute("id");
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

void FESchematicEditor::processCircle(QDomElement &element, QString parentID)
{
    double xpos = 0, ypos = 0, diameter = 1;
     
    gid++;
    if (parentID == "pin_connections" || parentID == "pins") {
        pinSVGList.addElement(gid,element,parentID,canvas);
        return;
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
    //stdError << "FESchematicEditor::processCircle(): gid = " << gid << "\n";
    item->setData((int)FEGraphicsScene::Group1,
                  QVariant((int)FEGraphicsScene::Schematic));
    item->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Circ));
}

void FESchematicEditor::processRect(QDomElement &element, QString /*parentID*/)
{
    double xpos = 0, ypos = 0, width = 1, height = 1;
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    gid++;
    xpos = element.attribute("x").toFloat();
    ypos = element.attribute("y").toFloat();
    width = element.attribute("width").toFloat();
    height = element.attribute("height").toFloat();
    //stdError << "FESchematicEditor::processRect(): element.attribute(\"fill\") " << element.attribute("fill") << "\n"; 
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
    //stdError << "FESchematicEditor::processRect(): gid = " << gid << "\n";
    item->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Rect));
    item->setData((int)FEGraphicsScene::Group1,
                  QVariant((int)FEGraphicsScene::Schematic));
}

void FESchematicEditor::processLine(QDomElement &element, QString parentID)
{
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    QPen pen(Qt::SolidLine);
    
    gid++;
    if (parentID == "pins") {
        pinSVGList.addElement(gid,element,parentID,canvas);
        return;
    }              
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
    item->setData((int)FEGraphicsScene::Group1,
                  QVariant((int)FEGraphicsScene::Schematic));
}

void FESchematicEditor::processPath(QDomElement &element, QString parentID)
{
}

void FESchematicEditor::processPolygon(QDomElement &element, QString /*parentID*/)
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
    item->setData((int)FEGraphicsScene::Group1,
                  QVariant((int)FEGraphicsScene::Schematic));
    
}

void FESchematicEditor::processEllipse(QDomElement &element, QString parentID)
{
    double xpos = 0, ypos = 0, rx = 0, ry = 0;
     
    gid++;
    if (parentID == "pin_connections" || parentID == "pins") {
        pinSVGList.addElement(gid,element,parentID,canvas);
        return;
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
    //stdError << "FESchematicEditor::processEllipse(): gid = " << gid << "\n";
    item->setData((int)FEGraphicsScene::Group1,
                  QVariant((int)FEGraphicsScene::Schematic));
    item->setData((int)FEGraphicsScene::Type,
                  QVariant((int)FEGraphicsScene::Circ));
}

void FESchematicEditor::processText(QDomElement &element, QString parentID)
{
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::NoBrush);
    QFont font;
    gid++;
    if (parentID == "pin_labels" || parentID == "pin_numbers") {
        pinSVGList.addElement(gid,element,parentID,canvas);
        return;
    }              
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
    //stdError << "FESchematicEditor::processText(): gid = " << gid << "\n";
    titem->setData((int)FEGraphicsScene::Type,
                   QVariant((int)FEGraphicsScene::Text));
    titem->setData((int)FEGraphicsScene::Group1,
                   QVariant((int)FEGraphicsScene::Schematic));
    
}

void FESchematicEditor::saveFile(const QString &filename)
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
    QDomElement schematicGroup = fbeSVGDocument.createElement("g");
    svg.appendChild(schematicGroup);
    schematicGroup.setAttribute("id","schematic");
    
    QDomElement pinsGroup = fbeSVGDocument.createElement("g");
    schematicGroup.appendChild(pinsGroup);
    pinsGroup.setAttribute("id","pins");
    
    QDomElement pinConnectionsGroup = fbeSVGDocument.createElement("g");
    pinsGroup.appendChild(pinConnectionsGroup);
    pinConnectionsGroup.setAttribute("id","pin_connections");
    
    QDomElement pinLabelsGroup = fbeSVGDocument.createElement("g");
    pinsGroup.appendChild(pinLabelsGroup);
    pinLabelsGroup.setAttribute("id","pin_labels");
    
    QDomElement pinNumbersGroup = fbeSVGDocument.createElement("g");
    pinsGroup.appendChild(pinNumbersGroup);
    pinNumbersGroup.setAttribute("id","pin_numbers");
    
    ItemList schematicItems = canvas->withtagEQ(FEGraphicsScene::Group1,QVariant((int)FEGraphicsScene::Schematic));
    for (items_Iterator ii = schematicItems.begin(); ii != schematicItems.end(); ii++) {
        QGraphicsItem *item = *ii;
        FESvgElement newElement(item);
        int pinno = item->data((int)FEGraphicsScene::Pinno).toInt();
        if (pinno > 0) {
            newElement.setId(QString("connector%1pin").arg(pinno));
        } else {
            newElement.setId("");
        }
        FEGraphicsScene::GroupIdType g2 = (FEGraphicsScene::GroupIdType)(item->data((int)FEGraphicsScene::Group2).toInt());
        if (g2 == FEGraphicsScene::PinConnections) {
            newElement.appendElement(pinConnectionsGroup);
        } else if (g2 == FEGraphicsScene::PinLabels) {
            newElement.appendElement(pinLabelsGroup);
        } else if (g2 == FEGraphicsScene::PinNumbers) {
            newElement.appendElement(pinNumbersGroup);
        } else if (g2 == FEGraphicsScene::Pins) {
            newElement.appendElement(pinsGroup);
        } else {
            newElement.appendElement(schematicGroup);
        }
    }
    QFile outfile(filename);
    outfile.open( QIODevice::WriteOnly );
    QString XML = fbeSVGDocument.toString();
    //stdError << "FESchematicEditor::saveFile(): " << XML << "\n";
    outfile.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" );
    outfile.write( XML.toUtf8() );
    outfile.close();
    setClean();
}


void FESchematicEditor::PinSVGList::addElement(int gid, QDomElement &element,
                                               QString groupName,
                                               FEGraphicsScene *canvas)
{
    //stdError << "FESchematicEditor::PinSVGList::addElement() groupName = " << groupName << "\n";
    //stdError << "FESchematicEditor::PinSVGList::addElement() elementtagName() = " << element.tagName() << "\n";
    if (groupName == "pin_connections" && element.tagName() == "circle") {
        addConnection(gid,element,canvas);
    } else if (groupName == "pin_labels" && element.tagName() == "text") {
        addLabel(gid,element,canvas);
    } else if (groupName == "pin_numbers" && element.tagName() == "text") {
        addNumber(gid,element,canvas);
    } else if (element.tagName() == "circle") {
        addInvert(gid,element,canvas);
    } else if (element.tagName() == "line") {
        addLead(gid,element,canvas);
    }
}

void FESchematicEditor::PinSVGList::addConnection(int gid, 
                                                  QDomElement &connection,
                                                  FEGraphicsScene *canvas)
{
    //stdError << "FESchematicEditor::PinSVGList::addConnection()\n";
    QRegularExpression connectorExpr("^connector([[:digit:]]+)pin$");
    QRegularExpressionMatch match;
    qreal x = connection.attribute("cx").toFloat();
    qreal y = connection.attribute("cy").toFloat();
    qreal cr = connection.attribute("r").toFloat();
    //stdError << "FESchematicEditor::PinSVGList::addConnection() x = " << x << " y = " << y << " cr = " << cr << "\n";
    ele_iterator eleI = findByGid(gid);
    if (eleI == _list.end()) {
        eleI = findByXY(x,y,cr*10.5);
        if (eleI == _list.end()) {
            _list.push_back(Element());
            eleI = _list.end()-1;
            eleI->gid = gid;
    
        }
    }
    //stdError << "FESchematicEditor::PinSVGList::addConnection() eleI->gid = " << eleI->gid << "\n";
    if (connection.attribute("id").contains(connectorExpr,&match)) {
        eleI->pinnumber = match.captured(1).toInt();
    }
    //stdError << "FESchematicEditor::PinSVGList::addConnection() eleI->pinnumber = " << eleI->pinnumber << "\n";
    eleI->x = x;
    eleI->y = y;
    eleI->length = cr*10;    
    eleI->color = QColor(connection.attribute("fill"));
    eleI->connection = connection;
    //stdError << "FESchematicEditor::PinSVGList::addConnection() eleI->x = " << eleI->x << "eleI->y = " << eleI->y << "eleI->length = " << eleI->length << "\n";
    qreal x0 = eleI->x - cr;
    qreal y0 = eleI->y - cr;
    qreal w = cr*2;
    qreal h = cr*2;
    eleI->connectionGI = canvas->addEllipse(x0,y0,w,h,QPen(Qt::NoPen),QBrush(eleI->color));
    //stdError << "FESchematicEditor::PinSVGList::addConnection() eleI->connectionGI = " << eleI->connectionGI << ":\n" << *(eleI->connectionGI) << "\n";
    eleI->connectionGI->setData((int)FEGraphicsScene::Gid,QVariant(eleI->gid));
    eleI->connectionGI->setData((int)FEGraphicsScene::Pinno,QVariant(eleI->pinnumber));
    eleI->connectionGI->setData((int)FEGraphicsScene::Type, 
                                QVariant((int)FEGraphicsScene::Pin));
    eleI->connectionGI->setData((int)FEGraphicsScene::Group1,
                                QVariant((int)FEGraphicsScene::Schematic));
    eleI->connectionGI->setData((int)FEGraphicsScene::Group2,
                                QVariant((int)FEGraphicsScene::PinConnections));
}

void FESchematicEditor::PinSVGList::addLead(int gid, QDomElement &lead,
                                            FEGraphicsScene *canvas)
{
    //stdError << "FESchematicEditor::PinSVGList::addLead()\n";
    ele_iterator eleI = findByGid(gid);
    qreal x1 = lead.attribute("x1").toFloat();
    qreal y1 = lead.attribute("y1").toFloat();
    qreal x2 = lead.attribute("x2").toFloat();
    qreal y2 = lead.attribute("y2").toFloat();
    //stdError << "FESchematicEditor::PinSVGList::addConnection() x1 = " << x1 << " y1 = " << y1 << " x2 = " << x2 << " y2 = " << y2 << "\n";
    if (eleI == _list.end()) {
        qreal dx = x2-x1;
        qreal dy = y2-y1;
        qreal tentitiveLength = 1.05*sqrt((dx*dx)+(dy*dy));
        eleI = findByXY(x1,y1,tentitiveLength);
        if (eleI == _list.end()) {
            eleI = findByXY(x2,y2,tentitiveLength);
        }
        if (eleI == _list.end()) {
            _list.push_back(Element());
            eleI = _list.end()-1;
            eleI->gid = gid;
            eleI->x = x1;
            eleI->y = y1;
            eleI->length = tentitiveLength;
        }
    }
    //stdError << "FESchematicEditor::PinSVGList::addLead() eleI->gid = " << eleI->gid << "\n";
    eleI->lead = lead;
    QColor color(lead.attribute("stroke"));
    qreal linethickness = lead.attribute("stroke-width").toFloat();
    
    eleI->leadGI = canvas->addLine(x1,y1,x2,y2, QPen(color,linethickness));
    //stdError << "FESchematicEditor::PinSVGList::addLead() eleI->leadGI = " << eleI->leadGI << ":\n" << *(eleI->leadGI) << "\n";
    eleI->leadGI->setData((int)FEGraphicsScene::Gid,QVariant(eleI->gid));
    eleI->leadGI->setData((int)FEGraphicsScene::Type,
                          QVariant((int)FEGraphicsScene::Pin));
    eleI->leadGI->setData((int)FEGraphicsScene::Group1,
                          QVariant((int)FEGraphicsScene::Schematic));
    eleI->leadGI->setData((int)FEGraphicsScene::Group2,
                          QVariant((int)FEGraphicsScene::Pins));
}

void FESchematicEditor::PinSVGList::addInvert(int gid, QDomElement &invert,
                                              FEGraphicsScene *canvas)
{
    //stdError << "FESchematicEditor::PinSVGList::addInvert()\n";
    qreal cx = invert.attribute("cx").toFloat();
    qreal cy = invert.attribute("cy").toFloat();
    qreal ir = invert.attribute("r").toFloat();
    ele_iterator eleI = findByGid(gid);
    if (eleI == _list.end()) {
        qreal tentitiveLength = 5.1*ir;
        eleI = findByXY(cx,cy,tentitiveLength);
        if (eleI == _list.end()) {
            _list.push_back(Element());
            eleI->gid = gid;
            eleI->x = cx;
            eleI->y = cy;
            eleI->length = tentitiveLength;
        }
    }
    eleI->invert = invert;
    QColor color(invert.attribute("stroke"));
    qreal linethickness = invert.attribute("stroke-width").toFloat();
    qreal x0 = cx-ir;
    qreal y0 = cy-ir;
    qreal w  = ir*2;
    qreal h  = ir*2;
    eleI->invertGI = canvas->addEllipse(x0,y0,w,h,QPen(color,linethickness),QBrush(Qt::NoBrush));
    eleI->invertGI->setData((int)FEGraphicsScene::Gid,QVariant(eleI->gid));
    eleI->invertGI->setData((int)FEGraphicsScene::Type,
                            QVariant((int)FEGraphicsScene::Pin));
    eleI->invertGI->setData((int)FEGraphicsScene::Group1,
                            QVariant((int)FEGraphicsScene::Schematic));
    eleI->invertGI->setData((int)FEGraphicsScene::Group2,
                            QVariant((int)FEGraphicsScene::Pins));
}

void FESchematicEditor::PinSVGList::addNumber(int gid, QDomElement &number,
                                              FEGraphicsScene *canvas)
{
    //stdError << "FESchematicEditor::PinSVGList::addNumber()\n";
    qreal nx = number.attribute("x").toFloat();
    qreal ny = number.attribute("y").toFloat();
    QFont font(number.attribute("font-family"),number.attribute("font-size").toFloat());
    QColor color(number.attribute("fill"));
    QDomText txt = number.firstChild().toText();
    ele_iterator eleI = findByGid(gid);
    if (eleI == _list.end()) {
        eleI = findByXY(nx,ny,number.attribute("font-size").toFloat()*6);
        if (eleI == _list.end()) {
            _list.push_back(Element());
            eleI->gid = gid;
        }
    }
    eleI->number = number;
    eleI->numberGI = canvas->addSimpleText(txt.data(),font);
    eleI->numberGI->setBrush(color);
    eleI->numberGI->setPos(nx,ny);
    eleI->numberGI->setData((int)FEGraphicsScene::Gid,QVariant(eleI->gid));
    eleI->numberGI->setData((int)FEGraphicsScene::Type,
                            QVariant((int)FEGraphicsScene::Pin));
    eleI->numberGI->setData((int)FEGraphicsScene::Group1,
                            QVariant((int)FEGraphicsScene::Schematic));
    eleI->numberGI->setData((int)FEGraphicsScene::Group2,
                            QVariant((int)FEGraphicsScene::PinNumbers));  
}

void FESchematicEditor::PinSVGList::addLabel(int gid, QDomElement &label,
                                             FEGraphicsScene *canvas)
{
    //stdError << "FESchematicEditor::PinSVGList::addLabeladdElement()\n";
    qreal nx = label.attribute("x").toFloat();
    qreal ny = label.attribute("y").toFloat();
    QFont font(label.attribute("font-family"),label.attribute("font-size").toFloat());
    QColor color(label.attribute("fill"));
    QDomText txt = label.firstChild().toText();
    ele_iterator eleI = findByGid(gid);
    if (eleI == _list.end()) {
        eleI = findByXY(nx,ny,label.attribute("font-size").toFloat()*6);
        if (eleI == _list.end()) {
            _list.push_back(Element());
            eleI->gid = gid;
        }
    }
    eleI->label = label;
    eleI->labelGI = canvas->addSimpleText(txt.data(),font);
    eleI->labelGI->setBrush(color);
    eleI->labelGI->setPos(nx,ny);
    eleI->labelGI->setData((int)FEGraphicsScene::Gid,QVariant(eleI->gid));
    eleI->labelGI->setData((int)FEGraphicsScene::Type,
                           QVariant((int)FEGraphicsScene::Pin));
    eleI->labelGI->setData((int)FEGraphicsScene::Group1,
                           QVariant((int)FEGraphicsScene::Schematic));
    eleI->labelGI->setData((int)FEGraphicsScene::Group2,
                           QVariant((int)FEGraphicsScene::PinLabels));
}

FESchematicEditor::PinSVGList::ele_iterator FESchematicEditor::PinSVGList::findByXY(qreal x, qreal y, qreal halo)
{
    for (ele_iterator i = _list.begin(); i != _list.end(); i++) {
        Element ele = *i;
        if (ele.x >= x-halo && ele.x <= x+halo &&
            ele.y >= y-halo && ele.y <= y+halo) return i;
    }
    return _list.end();;
}

FESchematicEditor::PinSVGList::ele_iterator FESchematicEditor::PinSVGList::findByGid(int searchgid)
{
    for (ele_iterator i = _list.begin(); i != _list.end(); i++) {
        Element ele = *i;
        if (ele.gid == searchgid) return i;
    }
    return _list.end();
}


void FESchematicEditor::PinSVGList::finalize()
{
    //stdError << "FESchematicEditor::PinSVGList::finalize()\n";
    for (ele_iterator i = _list.begin(); i != _list.end(); i++) {
        Element ele = *i;
        //stdError << "FESchematicEditor::PinSVGList::finalize() ele: \n";
        //stdError << "\tx = " << ele.x << "\n";
        //stdError << "\ty = " << ele.y << "\n";
        //stdError << "\tlength = " << ele.length << "\n";
        //stdError << "\tcolor = " << ele.color << "\n";
        //stdError << "\tgid = " << ele.gid << "\n";
        //stdError << "\tpinnumber = " << ele.pinnumber << "\n";
        //stdError << "\tconnectionGI = " << ele.connectionGI << "\n";
        //if (ele.connectionGI != NULL) {
        //    int gid = ele.connectionGI->data((int)FEGraphicsScene::Gid).toInt();
        //    int pinnum = ele.connectionGI->data((int)FEGraphicsScene::Pinno).toInt();
        //    stdError << "\tconnectionGI->data(Gid) = " << gid <<"\n";
        //    stdError << "\tconnectionGI->data(Pinno) = " << pinnum <<"\n";
        //}
        //stdError << "\tinvertGI = " << ele.invertGI << "\n";
        //if (ele.invertGI != NULL) {
        //   int gid = ele.invertGI->data((int)FEGraphicsScene::Gid).toInt();
        //    stdError << "\tinvertGI->data(Gid) = " << gid <<"\n";
        //}
        //stdError << "\tleadGI = " << ele.leadGI << "\n";
        //if (ele.leadGI != NULL) {
        //    int gid = ele.leadGI->data((int)FEGraphicsScene::Gid).toInt();
        //    stdError << "\tleadGI->data(Gid) = " << gid <<"\n";
        //}
        //stdError << "\tnumberGI = " << ele.numberGI << "\n";
        //if (ele.numberGI != NULL) {
        //    int gid = ele.numberGI->data((int)FEGraphicsScene::Gid).toInt();
        //    stdError << "\tnumberGI->data(Gid) = " << gid <<"\n";
        //}
        //stdError << "\tlabelGI = " << ele.labelGI << "\n";
        //if (ele.labelGI != NULL) {
        //    int gid = ele.labelGI->data((int)FEGraphicsScene::Gid).toInt();
        //    stdError << "\tlabelGI->data(Gid) = " << gid <<"\n";
        //}
        //if (ele.leadGI == NULL) continue;
        QLineF coords = ele.leadGI->line();
        if (coords.x1() == coords.x2()) {
            // Vertical (top or bottom)
            if (coords.y2() > coords.y1()) {
                // Top
                if (ele.invertGI == NULL) {
                    ele.orientation = Schematic::PinOrientationAndInversion::TopNormal;
                } else {
                    ele.orientation = Schematic::PinOrientationAndInversion::TopInverted;
                }
            } else {
                // Bottom
                if (ele.invertGI == NULL) {
                    ele.orientation = Schematic::PinOrientationAndInversion::BottomNormal;
                } else {
                    ele.orientation = Schematic::PinOrientationAndInversion::BottomInverted;
                }
            }
        } else {
            // Horizontal (left or right)
            if (coords.x2() > coords.x1()) {
                // Left
                if (ele.invertGI == NULL) {
                    ele.orientation = Schematic::PinOrientationAndInversion::LeftNormal;
                } else {
                    ele.orientation = Schematic::PinOrientationAndInversion::LeftInverted;
                }
            } else {
                // Right
                if (ele.invertGI == NULL) {
                    ele.orientation = Schematic::PinOrientationAndInversion::RightNormal;
                } else {
                    ele.orientation = Schematic::PinOrientationAndInversion::RightInverted;
                }
            }
        }
        ele.leadGI->setData((int)FEGraphicsScene::Orientation,
                            QVariant((int)ele.orientation));
        ele.leadGI->setData((int)FEGraphicsScene::Length,
                            QVariant(ele.length));
    }
}
