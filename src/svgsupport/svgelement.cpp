// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri May 24 10:09:00 2019
//  Last Modified : <190524.1125>
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
#include <QDomDocument>
#include <QDomElement>
#include <QGraphicsItem>
#include <QString>
#include <QPen>

#include "svgelement.h"

void FESvgElement::appendElement(QDomElement parent)
{
    switch (_item->type()) {
    case QGraphicsEllipseItem::Type: 
        {
            const QGraphicsEllipseItem *eitem = (QGraphicsEllipseItem *) _item;
            QRectF rect = eitem->rect();
            if (rect.width() == rect.height()) {
                qreal cx = rect.x() + (rect.width()/2.0);
                qreal cy = rect.y() + (rect.height()/2.0);
                qreal r  = rect.width()/2.0;
                QDomElement e = parent.ownerDocument().createElement("circle");
                e.setAttribute("cx",cx);
                e.setAttribute("cy",cy);
                e.setAttribute("r",r);
                if (eitem->brush().style() == Qt::NoBrush) {
                    e.setAttribute("fill","none");
                } else {
                    e.setAttribute("fill",eitem->brush().color().name());
                }
                if (eitem->pen().style() != Qt::NoPen) {
                    e.setAttribute("stroke",eitem->pen().color().name());
                    e.setAttribute("stroke-width",eitem->pen().width());
                }
                if (!(_id.isEmpty())) e.setAttribute("id",_id);
                parent.appendChild(e);
            } else {
                qreal cx = rect.x() + (rect.width()/2.0);
                qreal cy = rect.y() + (rect.height()/2.0);
                qreal rx = rect.width()/2.0;
                qreal ry = rect.height()/2.0;
                QDomElement e = parent.ownerDocument().createElement("ellipse");
                e.setAttribute("cx",cx);
                e.setAttribute("cy",cy);
                e.setAttribute("rx",rx);
                e.setAttribute("ry",ry);
                if (eitem->brush().style() == Qt::NoBrush) {
                    e.setAttribute("fill","none");
                } else {
                    e.setAttribute("fill",eitem->brush().color().name());
                }
                if (eitem->pen().style() != Qt::NoPen) {
                    e.setAttribute("stroke",eitem->pen().color().name());
                    e.setAttribute("stroke-width",eitem->pen().width());
                }
                if (!(_id.isEmpty())) e.setAttribute("id",_id);
                parent.appendChild(e);
            }
            break;
        }
    case QGraphicsRectItem::Type:
        {
            const QGraphicsRectItem *eitem = (QGraphicsRectItem *) _item;
            QRectF rect = eitem->rect();
            QDomElement e = parent.ownerDocument().createElement("rect");
            e.setAttribute("x",rect.x());
            e.setAttribute("y",rect.y());
            e.setAttribute("width",rect.width());
            e.setAttribute("height",rect.height());
            if (eitem->brush().style() == Qt::NoBrush) {
                e.setAttribute("fill","none");
            } else {
                e.setAttribute("fill",eitem->brush().color().name());
            }
            if (eitem->pen().style() != Qt::NoPen) {
                e.setAttribute("stroke",eitem->pen().color().name());
                e.setAttribute("stroke-width",eitem->pen().width());
            }
            if (!(_id.isEmpty())) e.setAttribute("id",_id);
            parent.appendChild(e);
            break;
        }
    case QGraphicsLineItem::Type:
        {
            const QGraphicsLineItem *eitem = (QGraphicsLineItem *) _item;
            QLineF line = eitem->line();
            QDomElement e = parent.ownerDocument().createElement("line");
            e.setAttribute("x1",line.x1());
            e.setAttribute("y1",line.y1());
            e.setAttribute("x2",line.x2());
            e.setAttribute("y2",line.y2());
            e.setAttribute("stroke",eitem->pen().color().name());
            e.setAttribute("stroke-width",eitem->pen().width());
            if (!(_id.isEmpty())) e.setAttribute("id",_id);
            parent.appendChild(e);
            break;
        }
    case QGraphicsPathItem::Type:
        {
            const QGraphicsPathItem *eitem = (QGraphicsPathItem *) _item;
            const QPainterPath path = eitem->path();
            QString dataString;
            QString space;
            for (int i = 0; i < path.elementCount(); i++) {
                QPainterPath::Element pEle = path.elementAt(i);
                switch (pEle.type) {
                case QPainterPath::MoveToElement:
                    dataString += space+"M"+QString::number(pEle.x)+","+QString::number(pEle.y);
                    space = " ";
                    break;
                case QPainterPath::LineToElement:
                    dataString += space+"L"+QString::number(pEle.x)+","+QString::number(pEle.y);
                    space = " ";
                    break;
                case QPainterPath::CurveToElement:
                    {
                        qreal xF = pEle.x;
                        qreal yF = pEle.y;
                        dataString += space+"C";
                        while (path.elementAt(i+1).type == QPainterPath::CurveToDataElement) {
                            i++;
                            pEle = path.elementAt(i);
                            QString::number(pEle.x)+","+QString::number(pEle.y)+" ";
                        }
                        dataString += QString::number(xF)+","+QString::number(yF);
                        space = " ";
                    }
                }
            }
            QDomElement e = parent.ownerDocument().createElement("path");
            e.setAttribute("d",dataString);
            if (eitem->brush().style() == Qt::NoBrush) {
                e.setAttribute("fill","none");
            } else {
                e.setAttribute("fill",eitem->brush().color().name());
            }
            if (eitem->pen().style() != Qt::NoPen) {
                e.setAttribute("stroke",eitem->pen().color().name());
                e.setAttribute("stroke-width",eitem->pen().width());
            }
            if (!(_id.isEmpty())) e.setAttribute("id",_id);
            parent.appendChild(e);
            break;
        }
    case QGraphicsPolygonItem::Type:
        {
            const QGraphicsPolygonItem *eitem = (QGraphicsPolygonItem *) _item;
            const QPolygonF points = eitem->polygon();
            QString pointString;
            QString space;
            for (QPolygonF::const_iterator ip = points.begin();
                 ip !=  points.end();
                 ip++) {
                const QPointF p = *ip;
                pointString += space+QString::number(p.x())+","+QString::number(p.y());
                space = " ";
            }
            QDomElement e = parent.ownerDocument().createElement("polygon");
            e.setAttribute("points",pointString);
            if (eitem->brush().style() == Qt::NoBrush) {
                e.setAttribute("fill","none");
            } else {
                e.setAttribute("fill",eitem->brush().color().name());
            }
            if (eitem->pen().style() != Qt::NoPen) {
                e.setAttribute("stroke",eitem->pen().color().name());
                e.setAttribute("stroke-width",eitem->pen().width());
            }
            if (!(_id.isEmpty())) e.setAttribute("id",_id);
            parent.appendChild(e);
            break;
        }
    case QGraphicsSimpleTextItem::Type:
        {
            const QGraphicsSimpleTextItem *titem = (QGraphicsSimpleTextItem *) _item;
            const QPointF p = titem->pos();
            const QFont f = titem->font();
            QDomElement t = parent.ownerDocument().createElement("text");
            t.setAttribute("x",p.x());
            t.setAttribute("y",p.y());
            if (titem->brush().style() == Qt::NoBrush) {
                t.setAttribute("fill","none");
            } else {
                t.setAttribute("fill",titem->brush().color().name());
            }
            if (titem->pen().style() != Qt::NoPen) {
                t.setAttribute("stroke",titem->pen().color().name());
                t.setAttribute("stroke-width",titem->pen().width());
            }
            t.setAttribute("font-family",f.family());
            t.setAttribute("font-size",f.pointSize());
            if (!(_id.isEmpty())) t.setAttribute("id",_id);
            QDomText txt = parent.ownerDocument().createTextNode(titem->text());
            t.appendChild(txt);
            parent.appendChild(t);
            break;
        }
    }
}
