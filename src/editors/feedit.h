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
//  Last Modified : <190517.2335>
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

class QMenu;
class QGraphicsScene;
class QLineEdit;
class QLabel;
class QToolBar;
class QGraphicsRectItem;

#include <QGraphicsView>

class FEGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit FEGraphicsView(QGraphicsScene *canvas = 0, QWidget *parent = 0)
                : QGraphicsView(canvas,parent) 
    {
        this->setMouseTracking(true);
    }
protected:
    virtual void mouseMoveEvent(QMouseEvent * event)
    {
        emit mouseMoved(event);
    }
    virtual void mousePressEvent(QMouseEvent * event)
    {
        emit mousePressed(event);
    }
    virtual void keyPressEvent(QKeyEvent * event)
    {
        emit keyPressed(event);
    }
    virtual void resizeEvent(QResizeEvent * event)
    {
        emit resized(event);
    }
signals:
    void mouseMoved(QMouseEvent * event);
    void mousePressed(QMouseEvent * event);
    void keyPressed(QKeyEvent * event);
    void resized(QResizeEvent * event);
};
          
#include <QToolButton>

class ToolMenuButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolMenuButton(QWidget *parent = 0);
};


class SizeAndVP : public QWidget {
    Q_OBJECT
public:
    typedef enum unitsType {mm, in} UnitsType;
    SizeAndVP(UnitsType units, double width, double height, 
              const QRectF &viewport, QWidget *parent = 0);
    ~SizeAndVP();
    inline UnitsType Units() const {return _units;}
    inline void  Viewport(QRectF &vp) const {
        vp = _viewport;
    }
    inline double Width() const {return _width;}
    inline double Height() const {return _height;}
    void setUnits(UnitsType u);
    void setViewport(const QRectF &vp);
    void setWidth(double w);
    void setHeight(double h);
    void updatexyposition(double x, double y);
    void updateZoom(double zoom);
private:
    UnitsType _units;
    double _width, _height;
    QRectF _viewport;
    double _xpos, _ypos;
    double _zoom;
    QLineEdit *w, *h, *x1, *y1, *x2, *y2, *xpos, *ypos;
    QLabel    *wUnits, *hUnits;
    QLineEdit *z;
    QString formatZoom(double zoom);
};

class FEEdit : public QWidget 
{
    Q_OBJECT
public:
    FEEdit(SizeAndVP::UnitsType units = SizeAndVP::mm, 
           double width=25.4, double height=25.4, 
           const QRectF &viewport = QRectF(0,0,254,254), 
           QWidget *parent = 0);
    virtual ~FEEdit();
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
    QGraphicsScene *canvas;
protected slots:
    virtual void addPin() = 0;
    virtual void editPin(int gid) = 0;
    virtual void addRect() = 0;
    virtual void editRect(int gid) = 0;
    virtual void addLine() = 0;
    virtual void editLine(int gid) = 0;
    virtual void addCirc() = 0;
    virtual void editCirc(int gid) = 0;
    virtual void addArc() = 0;
    virtual void editArc(int gid) = 0;
    virtual void addPoly() = 0;
    virtual void editPoly(int gid) = 0;
    virtual void addText() = 0;
    virtual void editText(int gid) = 0;
    
private slots:
    void setZoom_16()    {setZoom(16.0);}
    void setZoom_8()     {setZoom(8.0);}
    void setZoom_4()     {setZoom(4.0);}
    void setZoom_2()     {setZoom(2.0);}
    void setZoom_1()     {setZoom(1.0);}
    void setZoom__5()    {setZoom(.5);}
    void setZoom__25()   {setZoom(.25);}
    void setZoom__125()  {setZoom(.125);}
    void setZoom__0625() {setZoom(.0625);}
    void zoomBy_2()      {zoomBy(2.0);}
    void zoomBy__5()     {zoomBy(.5);}
    void canvasContextMenu();
    void editItems();
    void deleteItems();
    void xyposition();
    void setsize();
    void shrinkwrap();
    void mouseMoved(QMouseEvent * event);
    void mousePressed(QMouseEvent * event);
    void keyPressed(QKeyEvent * event);
    void resized(QResizeEvent * event);
private:
    QToolBar *toolbuttons;
    QGraphicsView  *canvasView;
    QAction *addPinAct;
    QAction *editPinAct;
    QAction *addRectAct;
    QAction *editRectAct;
    QAction *addLineAct;
    QAction *editLineAct;
    QAction *addCircAct;
    QAction *editCircAct;
    QAction *addArcAct;
    QAction *editArcAct;
    QAction *addPolyAct;
    QAction *editPolyAct;
    QAction *addTextAct;
    QAction *editTextAct;
    QAction *setZoom_16Act;
    QAction *setZoom_8Act;
    QAction *setZoom_4Act;
    QAction *setZoom_2Act;
    QAction *setZoom_1Act;
    QAction *setZoom__5Act;
    QAction *setZoom__25Act;
    QAction *setZoom__125Act;
    QAction *setZoom__0625Act;
    void setZoom(double newZoomFactor);
    QAction *zoomBy_2Act;
    QAction *zoomBy__5Act;
    void zoomBy(double relativeZoomFactor);
    QMenu     *zoomMenu;
    ToolMenuButton *zoomButton;
    QAction *zoomAct;
    QAction *canvasContextMenuAct;
    QAction *editItemsAct;
    QAction *deleteItemsAct;
    QAction *xypositionAct;
    QAction *setsizeAct;
    QAction *shrinkWrapAct;
    SizeAndVP *sizeAndVP;
    double _zoomScale;
    double _vpscale;
    QGraphicsRectItem *_vpRect;
    void createToolButtons();
    void createZoomMenu();
    void updateSR();
    void makeVpRect();
};



#endif // __FEEDIT_H

