// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 16:33:47 2019
//  Last Modified : <190516.1756>
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

#ifndef __FEEDIT_H
#define __FEEDIT_H

#include <QWidget>
#include <math.h>

class QButtonGroup;
class QGraphicsScene;
class QScrollArea;
class QMenu;

class FEEdit : public QWidget 
{
    Q_OBJECT
public:
    FEEdit(QWidget *parent = 0);
    ~FEEdit();
    typedef enum {pin, rect, line, circle, arc, poly, text} ItemTypes;
    
protected:
    int gid; // GID to uniquely identify objects (including grouped objects)
    int pinno; // Pin numbers
    bool isdirty; // Dirty flag
    
    inline double radians (double degrees) const {
        return (degrees/180.0)*M_PI;
    }
    inline double degrees (double radians) const {
        return (radians/M_PI) * 180.0;
    }
    inline double square (double x) const {
        return x*x;
    }
    inline bool findCenter (double x1, double y1, double x2, double y2, double radius, double &xc, double &yc, double &a1, double &a2)
    {
        double a = x1;
        double b = y1;
        double c = x2;
        double d = y2;
        double r = radius;
        
        double J = 2.0*(a-c);
        double G = 2.0*(b-d);
        double T = (square(a)+square(b)) - (square(c)+square(d));
        
        double u = (1.0 + (square(J) / square(G)));
        double v = (-2.0*a) - ((2.0*J*T)/square(G)) + ((2.0*J*b)/G);
        double w = square(a)+square(b) + square(T)/square(G) - 2*b*T/G - square(r);
        
        double temp = square(v)-4.0*u*w;
        if (temp < 0) {
            return false;// can't take the sqrt of a negative number!
        }
        double _sqrt = sqrt(temp);
        
        double m1 = (-v + _sqrt)/(2.0*u);
        double n1 = (T-J*m1)/G;
        
        double m2 = (-v - _sqrt)/(2.0*u);
        double n2 = (T-J*m2)/G;
        
        double at1 = atan2(c-m1,d-n1);
        double at2 = atan2(a-m1,b-n1);
        
        double sn = sin(at1 - at2);
        
        double m, n;
        if (sn > 0) {
            m = m1;
            n = n1;
        } else {
            m = m2;
            n = n2;
        }
        
        xc = m;
        yc = n;
        
        a1 = degrees(-atan2(y1-yc,x1-xc));
        a2 = degrees(-atan2(y2-yc,x2-xc));
        if (a2 < 0) {
            a2 += 360;
        }
        return true;
    }
    
    QMenu *itemcontextmenu;
    QMenu *canvascontextmenu;
    int itemContext_gid;
    QString itemContext_label;
    ItemTypes itemContext_itemtype;
    
    void deleteItem (int gid,  QString label = "");
    inline void setDirty () {
        isdirty = true;
    }
    inline void setClean () {
        isdirty = false;
    }
    inline bool isDirty () const {return isdirty;}
    QButtonGroup *toolbuttons;
    QGraphicsScene *canvas;
    
private slots:
private:
    QScrollArea *scroll;
    QMenu     *zoomMenu;
};

#endif // __FEEDIT_H

