// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 17:49:56 2019
//  Last Modified : <190517.1412>
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

#include <QtWidgets>
#include <QToolBar>
#include <QToolButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollArea>
#include <QMenu>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "feedit.h"

ToolMenuButton::ToolMenuButton(QWidget *parent) : QToolButton(parent)
{
    setPopupMode(QToolButton::MenuButtonPopup);
    setArrowType(Qt::DownArrow);
    setToolButtonStyle(Qt::ToolButtonTextOnly);
    QObject::connect(this, SIGNAL(triggered(QAction*)),this, 
                     SLOT(setDefaultAction(QAction*)));
}

FEEdit::FEEdit(SizeAndVP::UnitsType units, double width, 
               double height, 
               const SizeAndVP::ViewportType &viewport, 
               QWidget *parent) 
{
    _zoomScale = 1.0;
    _vpscale   = 1.0;
    canvas = new QGraphicsScene;
    canvasView = new QGraphicsView(canvas,this);
    canvasView->setMouseTracking(true);
    toolbuttons = new QToolBar(this);
    toolbuttons->setOrientation(Qt::Vertical);
    toolbuttons->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sizeAndVP = new SizeAndVP(units,width,height,viewport,this);
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(canvasView,0,0);
    layout->addWidget(toolbuttons,0,1);
    layout->addWidget(sizeAndVP,1,0,1,2);
    setLayout(layout);
    createToolButtons();
    sizeAndVP->updateZoom(_zoomScale);
}

FEEdit::~FEEdit()
{
}

void FEEdit::createToolButtons()
{
    addPinAct = new QAction(QIcon(":/resources/images/small_addpin.png"), tr("Add Pin"), this);
    addPinAct->setStatusTip(tr("Add a pin"));
    connect(addPinAct, SIGNAL(triggered()), this, SLOT(addPin()));
    toolbuttons->addAction(addPinAct);
    
    addRectAct = new QAction(QIcon(":/resources/images/small_addrect.png"), tr("Add Rect"), this);
    addRectAct->setStatusTip(tr("Add a rectangle"));
    connect(addRectAct, SIGNAL(triggered()), this, SLOT(addRect()));
    toolbuttons->addAction(addRectAct);
    
    addLineAct = new QAction(QIcon(":/resources/images/small_addline.png"), tr("Add Line"), this);
    addLineAct->setStatusTip(tr("Add a line"));
    connect(addLineAct, SIGNAL(triggered()), this, SLOT(addLine()));
    toolbuttons->addAction(addLineAct);
    
    addCircAct = new QAction(QIcon(":/resources/images/small_addcirc.png"), tr("Add Circ"), this);
    addCircAct->setStatusTip(tr("Add a circle"));
    connect(addCircAct, SIGNAL(triggered()), this, SLOT(addCirc()));
    toolbuttons->addAction(addCircAct);
    
    addArcAct = new QAction(QIcon(":/resources/images/small_addarc.png"), tr("Add Arc"), this);
    addArcAct->setStatusTip(tr("Add a arc"));
    connect(addArcAct, SIGNAL(triggered()), this, SLOT(addArc()));
    toolbuttons->addAction(addArcAct);
    
    addPolyAct = new QAction(/*QIcon(":/resources/images/small_addpoly.png"),*/ tr("Add Poly"), this);
    addPolyAct->setStatusTip(tr("Add a polyline/polygon"));
    connect(addPolyAct, SIGNAL(triggered()), this, SLOT(addPoly()));
    toolbuttons->addAction(addPolyAct);
    
    addTextAct = new QAction(QIcon(":/resources/images/small_addtext.png"), tr("Add Text"), this);
    addTextAct->setStatusTip(tr("Add a text"));
    connect(addTextAct, SIGNAL(triggered()), this, SLOT(addText()));
    toolbuttons->addAction(addTextAct);
    
    setsizeAct = new QAction(QIcon(":/resources/images/small_setsize.png"), tr("Size"), this);
    setsizeAct->setStatusTip(tr("Change the size"));
    connect(setsizeAct, SIGNAL(triggered()), this, SLOT(setsize()));
    toolbuttons->addAction(setsizeAct);
    
    shrinkWrapAct = new QAction(QIcon(":/resources/images/small_shrinkwrap.png"), tr("Shrink Wrap"), this);
    shrinkWrapAct->setStatusTip(tr("Shrink Wrap"));
    connect(shrinkWrapAct, SIGNAL(triggered()), this, SLOT(shrinkwrap()));
    toolbuttons->addAction(shrinkWrapAct);
    
    createZoomMenu();
    
    zoomButton = new ToolMenuButton;
    zoomButton->setMenu(zoomMenu);
    zoomButton->setDefaultAction(setZoom_1Act);
    toolbuttons->addWidget(zoomButton);
}

void FEEdit::createZoomMenu()
{
    zoomMenu = new QMenu(toolbuttons);
    setZoom_16Act = new QAction(tr("Zoom 16:1"), this);
    zoomMenu->addAction(setZoom_16Act);
    connect(setZoom_16Act, SIGNAL(triggered()), this, SLOT(setZoom_16()));
    setZoom_8Act = new QAction(tr("Zoom 8:1"), this);
    zoomMenu->addAction(setZoom_8Act);
    connect(setZoom_8Act, SIGNAL(triggered()), this, SLOT(setZoom_8()));
    setZoom_4Act = new QAction(tr("Zoom 4:1"), this);
    zoomMenu->addAction(setZoom_4Act);
    connect(setZoom_4Act, SIGNAL(triggered()), this, SLOT(setZoom_4()));
    setZoom_2Act = new QAction(tr("Zoom 2:1"), this);
    zoomMenu->addAction(setZoom_2Act);
    connect(setZoom_2Act, SIGNAL(triggered()), this, SLOT(setZoom_2()));
    setZoom_1Act = new QAction(tr("Zoom 1:1"), this);
    zoomMenu->addAction(setZoom_1Act);
    connect(setZoom_1Act, SIGNAL(triggered()), this, SLOT(setZoom_1()));
    setZoom__5Act = new QAction(tr("Zoom 1:2"), this);
    zoomMenu->addAction(setZoom__5Act);
    connect(setZoom__5Act, SIGNAL(triggered()), this, SLOT(setZoom__5()));
    setZoom__25Act = new QAction(tr("Zoom 1:4"), this);
    zoomMenu->addAction(setZoom__25Act);
    connect(setZoom__25Act, SIGNAL(triggered()), this, SLOT(setZoom__25()));
    setZoom__125Act = new QAction(tr("Zoom 1:8"), this);
    zoomMenu->addAction(setZoom__125Act);
    connect(setZoom__125Act, SIGNAL(triggered()), this, SLOT(setZoom__125()));
    setZoom__0625Act = new QAction(tr("Zoom 1:16"), this);
    zoomMenu->addAction(setZoom__0625Act);
    connect(setZoom__0625Act, SIGNAL(triggered()), this, SLOT(setZoom__0625()));
}

void FEEdit::deleteItem (int gid,  QString label)
{
}

void FEEdit::canvasContextMenu()
{
}

void FEEdit::editItems()
{
}

void FEEdit::deleteItems()
{
}

void FEEdit::xyposition()
{
}

void FEEdit::setsize()
{
}

void FEEdit::setZoom(double zoomFactor)
{
    if (_zoomScale != 1.0) {
        double inv = 1.0 / _zoomScale;
        canvasView->scale(inv,inv);
    }
    canvasView->scale(zoomFactor,zoomFactor);
    _zoomScale = zoomFactor;
    sizeAndVP->updateZoom(_zoomScale);
    //$toolbuttons itemconfigure zoom -text "[formatZoom $_zoomScale]"
    updateSR();
}

void FEEdit::zoomBy(double zoomFactor)
{
    canvasView->scale(zoomFactor,zoomFactor);
    _zoomScale *= zoomFactor;
    sizeAndVP->updateZoom(_zoomScale);
    //$toolbuttons itemconfigure zoom -text "[formatZoom $_zoomScale]"
    updateSR();
}

void FEEdit::updateSR()
{
    bool newSR = false;
    QRectF curSR = canvas->sceneRect();
    QRectF bbox = canvas->itemsBoundingRect();
    if (curSR.width() != bbox.width()) {
        curSR.setWidth(bbox.width());
        newSR = true;
    }
    if (curSR.height() != bbox.height()) {
        curSR.setHeight(bbox.height());
        newSR = true;
    }
    if (newSR) {
        canvas->setSceneRect(curSR);
    }
}

void FEEdit::shrinkwrap()
{
}

SizeAndVP::SizeAndVP(UnitsType units, double width, double height,            
                     const ViewportType &viewport, QWidget *parent)
      : _units(units), _width(width), _height(height), _viewport(viewport)
{
    //QLabel *li, *l2, *vp, *posl, *l3;
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *l1 = new QLabel("Width: ",this);
    layout->addWidget(l1); // check
    w = new QLineEdit(this);
    w->setReadOnly(true);
    w->setMaxLength(4);
    w->setText(QString::number(_width,'f'));
    layout->addWidget(w); // check
    wUnits = new QLabel(((_units==mm)?"mm":"in"),this);
    layout->addWidget(wUnits);
    QLabel *l2 = new QLabel(" Height: ",this);
    layout->addWidget(l2);
    h = new QLineEdit(this);
    h->setReadOnly(true);
    h->setMaxLength(4);
    h->setText(QString::number(_height,'f'));
    layout->addWidget(h);
    hUnits = new QLabel(((_units==mm)?"mm":"in"),this);
    layout->addWidget(hUnits);
    QLabel *vp = new QLabel(" Viewport: ",this);
    layout->addWidget(vp);
    x1 = new QLineEdit(this);
    x1->setReadOnly(true);
    x1->setMaxLength(4);
    x1->setText(QString::number(_viewport.x,'f'));
    layout->addWidget(x1);
    y1 = new QLineEdit(this);
    y1->setReadOnly(true);
    y1->setMaxLength(4);
    y1->setText(QString::number(_viewport.y,'f'));
    layout->addWidget(y1);
    x2 = new QLineEdit(this);
    x2->setReadOnly(true);
    x2->setMaxLength(4);
    x2->setText(QString::number(_viewport.width,'f'));
    layout->addWidget(x2);
    y2 = new QLineEdit(this);
    y2->setReadOnly(true);
    y2->setMaxLength(4);
    y2->setText(QString::number(_viewport.height,'f'));
    layout->addWidget(y2);
    QLabel *posl = new QLabel(" Position: ",this);
    layout->addWidget(posl);
    xpos = new QLineEdit(this);
    xpos->setReadOnly(true);
    xpos->setMaxLength(4);
    xpos->setText(QString::number(_xpos,'f'));
    layout->addWidget(xpos);
    ypos = new QLineEdit(this);
    ypos->setReadOnly(true);
    ypos->setMaxLength(4);
    ypos->setText(QString::number(_ypos,'f'));
    layout->addWidget(ypos);
    QLabel *zlab = new QLabel(" Zoom: ",this);
    layout->addWidget(zlab);
    z = new QLineEdit(this);
    z->setReadOnly(true);
    z->setMaxLength(4); 
    z->setText(formatZoom(_zoom));
    layout->addWidget(z);
}

SizeAndVP::~SizeAndVP()
{
}

void SizeAndVP::setUnits(SizeAndVP::UnitsType u)
{
    _units = u;
    switch (u) {
    case mm:
        wUnits->setText("mm");
        hUnits->setText("mm");
        break;        
    case in:
        wUnits->setText("in");
        hUnits->setText("in");
        break;
    }
}

void SizeAndVP::setViewport(const SizeAndVP::ViewportType &vp)
{
    _viewport = vp;
    x1->setText(QString::number(_viewport.x,'f'));
    y1->setText(QString::number(_viewport.y,'f'));
    x2->setText(QString::number(_viewport.width,'f'));
    y2->setText(QString::number(_viewport.height,'f'));
}

void SizeAndVP::setWidth(double ww)
{
    _width = ww;
    w->setText(QString::number(_width,'f'));
}

void SizeAndVP::setHeight(double hh)
{
    _height = hh;
    h->setText(QString::number(_height,'f'));
}

void SizeAndVP::updatexyposition(double x, double y)
{
    // Scaling magic...
    
}

void SizeAndVP::updateZoom(double zoom)
{
    _zoom = zoom;
    z->setText(formatZoom(_zoom));
}

QString SizeAndVP::formatZoom(double z)
{
    if (z > 1) {
        return QString("%1:1").arg(z,0,'f',0);
    } else {
        return QString("1:%1").arg(1.0/z,0,'f',0);
    }
}
