// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:48:23 2019
//  Last Modified : <190525.1826>
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

#ifndef __FSEEDIT_H
#define __FSEEDIT_H

#include <QRectF>
#include <QDomElement>
#include "feedit.h"
#include "fsedialogs.h"
#include "fbedialogs.h"

class FESchematicEditor : public FEEdit {
public:
    FESchematicEditor(SizeAndVP::UnitsType units = SizeAndVP::mm, 
           double width=25.4, double height=25.4, 
           const QRectF &viewport = QRectF(0,0,254,254),
           QWidget *parent = 0);
    virtual ~FESchematicEditor();
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
    void _addPin_helper(double xpos, double ypos, 
                        Schematic::PinOrientationAndInversion::PinOrient orientation,
                        double length, double linethickness, int pinno, 
                        QString pinname, QString font, int size, QColor color);
    Schematic::AddPinDialog *addPinDialog;
    Breadboard::AddRectDialog *addRectDialog;
    Breadboard::AddLineDialog *addLineDialog;
    Breadboard::AddCircDialog *addCircDialog;
    Breadboard::AddArcDialog *addArcDialog;
    Breadboard::AddPolyDialog *addPolyDialog;
    Breadboard::AddTextDialog *addTextDialog;
    
    class PinSVGList {
    public:
        struct Element {
            Element() {
                x = 0; y = 0; length = 0;
                gid = 0; pinnumber = 0;
                color = QColor("black");
                orientation = Schematic::PinOrientationAndInversion::LeftNormal;
                connectionGI = NULL;
                invertGI = NULL;
                leadGI = NULL;
                numberGI = NULL;
                labelGI = NULL;
            }
            qreal x, y, length;
            QColor color;
            Schematic::PinOrientationAndInversion::PinOrient orientation;
            QDomElement connection, lead, invert, number, label;
            QGraphicsEllipseItem *connectionGI, *invertGI;
            QGraphicsLineItem *leadGI;
            QGraphicsSimpleTextItem *numberGI, *labelGI;
            int gid, pinnumber;
        };
        typedef QVector<Element> ElementVector;
        typedef ElementVector::const_iterator ele_constIterator;
        typedef ElementVector::iterator ele_iterator;
        PinSVGList() {clear();}
        inline void clear() {_list.clear();}
        void addElement(int gid, QDomElement &element,QString groupName,
                        FEGraphicsScene *canvas);
        void finalize();
    private:
        inline int length () const {return _list.length();}
        inline Element & operator[] (int i) {return _list[i];}
        inline const Element & operator[] (int i) const {return _list[i];}
        void addConnection(int gid, QDomElement &connection,
                           FEGraphicsScene *canvas);
        void addLead(int gid, QDomElement &lead,FEGraphicsScene *canvas);
        void addInvert(int gid, QDomElement &invert,FEGraphicsScene *canvas);
        void addNumber(int gid, QDomElement &number,FEGraphicsScene *canvas);
        void addLabel(int gid, QDomElement &label, FEGraphicsScene *canvas);
        ele_iterator findByXY(qreal x, qreal y, qreal halo);
        ele_iterator findByGid(int searchgid);
        QVector<Element> _list;
    };
    
    PinSVGList pinSVGList;
              
    void processCircle(QDomElement &element, QString parentID);
    void processRect(QDomElement &element, QString parentID);
    void processLine(QDomElement &element, QString parentID);
    void processPath(QDomElement &element, QString parentID);
    void processEllipse(QDomElement &element, QString parentID);
    void processText(QDomElement &element, QString parentID);
    void processPolygon(QDomElement &element, QString parentID);
};


#endif // __FSEEDIT_H

