// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat May 18 23:58:19 2019
//  Last Modified : <190519.1956>
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

#include <QDialog>
#include <QColor>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QFrame>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "../support/ColorSelector.h"
#include "../support/TitledDialog.h"
#include "fbedialogs.h"

namespace Breadboard {

AddPinDialog::AddPinDialog(QWidget *parent) 
      : TitledDialog(QIcon(":/resources/images/addpin.png"),parent)
{
    setTitle(tr("Add Pin"));
    
    QLabel *xposLabel = new QLabel(tr("X Position"));
    xposSB    = new QDoubleSpinBox;
    xposSB->setRange(0.0,1000000.0);
    xposSB->setSingleStep(0.1);
    xposLabel->setBuddy(xposSB);
    
    QLabel *yposLabel = new QLabel(tr("Y Position"));
    yposSB    = new QDoubleSpinBox;
    yposSB->setRange(0.0,1000000.0);
    yposSB->setSingleStep(0.1);
    yposLabel->setBuddy(yposSB);
    
    QLabel *diameterLabel = new QLabel(tr("Diameter"));
    diameterSB    = new QDoubleSpinBox;
    diameterSB->setRange(0.0,1000000.0);
    diameterSB->setSingleStep(0.1);
    diameterLabel->setBuddy(diameterSB);
    
    QLabel *colorLabel    = new QLabel(tr("Color"));
    colorCS       = new ColorSelector;
    colorLabel->setBuddy(colorCS);
    
    QLabel *pinnoLabel    = new QLabel(tr("Pin Number"));
    pinnoSB       = new QSpinBox;
    pinnoSB->setRange(1,1000);
    pinnoSB->setSingleStep(1);
    pinnoLabel->setBuddy(pinnoSB);
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(xposLabel, 0, 0);
    mainLayout->addWidget(xposSB, 0, 1);
    mainLayout->addWidget(yposLabel, 1, 0);
    mainLayout->addWidget(yposSB, 1, 1);
    mainLayout->addWidget(diameterLabel, 2, 0);
    mainLayout->addWidget(diameterSB, 2, 1);
    mainLayout->addWidget(colorLabel, 3, 0);
    mainLayout->addWidget(colorCS, 3, 1);
    mainLayout->addWidget(pinnoLabel, 4, 0);
    mainLayout->addWidget(pinnoSB, 4, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 5, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}

bool AddPinDialog::draw(double &xpos, double &ypos, double &diameter, QColor &color, int &pinno, bool editing, QString title, QString button)
{
    if (editing) {
        xposSB->setValue(xpos);
        yposSB->setValue(ypos);
        diameterSB->setValue(diameter);
        colorCS->setValue(color);
    }
    pinnoSB->setValue(pinno);
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        xpos = xposSB->value();
        ypos = yposSB->value();
        diameter = diameterSB->value();
        color = colorCS->value();
        pinno = pinnoSB->value();
        return true;
    } else {
        return false;
    }
}

void AddPinDialog::acceptCheck()
{
    if (colorCS->isValid() && diameterSB->value() > 0 && pinnoSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

AddRectDialog::AddRectDialog(QWidget *parent) 
      : TitledDialog(QIcon(":/resources/images/addrect.png"),parent)
{
    setTitle("Add Rectangle");
    
    QLabel *xposLabel = new QLabel(tr("X Position"));
    xposSB    = new QDoubleSpinBox;
    xposSB->setRange(0.0,1000000.0);
    xposSB->setSingleStep(0.1);
    xposLabel->setBuddy(xposSB);
    
    QLabel *yposLabel = new QLabel(tr("Y Position"));
    yposSB    = new QDoubleSpinBox;
    yposSB->setRange(0.0,1000000.0);
    yposSB->setSingleStep(0.1);
    yposLabel->setBuddy(yposSB);
    
    QLabel *widthLabel = new QLabel(tr("Width"));
    widthSB    = new QDoubleSpinBox;
    widthSB->setRange(0.0,1000000.0);
    widthSB->setSingleStep(0.1);
    widthLabel->setBuddy(widthSB);
    
    QLabel *heightLabel = new QLabel(tr("Height"));
    heightSB    = new QDoubleSpinBox;
    heightSB->setRange(0.0,1000000.0);
    heightSB->setSingleStep(0.1);
    heightLabel->setBuddy(heightSB);
    
    QLabel *linethicknessLabel = new QLabel(tr("Line Thickness"));
    linethicknessSB    = new QDoubleSpinBox;
    linethicknessSB->setRange(0.0,1000000.0);
    linethicknessSB->setSingleStep(0.1);
    linethicknessLabel->setBuddy(linethicknessSB);
    
    QLabel *colorLabel    = new QLabel(tr("Color"));
    colorCS       = new ColorSelector;
    colorLabel->setBuddy(colorCS);
    
    filledCKB = new QCheckBox(tr("Filled?"));
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(xposLabel, 0, 0);
    mainLayout->addWidget(xposSB, 0, 1);
    mainLayout->addWidget(yposLabel, 1, 0);
    mainLayout->addWidget(yposSB, 1, 1);
    mainLayout->addWidget(widthLabel, 2, 0);
    mainLayout->addWidget(widthSB, 2, 1);
    mainLayout->addWidget(heightLabel, 3, 0);
    mainLayout->addWidget(heightSB, 3, 1);
    mainLayout->addWidget(linethicknessLabel, 4, 0);
    mainLayout->addWidget(linethicknessSB, 4, 1);
    mainLayout->addWidget(colorLabel, 5, 0);
    mainLayout->addWidget(colorCS, 5, 1);
    mainLayout->addWidget(filledCKB, 6, 0, 1, 2);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 7, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}

bool AddRectDialog::draw(double &xpos, double &ypos, double &width, double &height, double &linethickness, QColor &color, bool &filled, bool editing, QString title, QString button)
{
    if (editing) {
        xposSB->setValue(xpos);
        yposSB->setValue(ypos);
        widthSB->setValue(width);
        heightSB->setValue(height);
        linethicknessSB->setValue(linethickness);
        colorCS->setValue(color);
        filledCKB->setChecked(filled);
    }
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        xpos = xposSB->value();
        ypos = yposSB->value();
        width = widthSB->value();
        height = heightSB->value();
        linethickness = linethicknessSB->value();
        color = colorCS->value();
        filled = filledCKB->isChecked();
        return true;
    } else {
        return false;
    }
}

void AddRectDialog::acceptCheck()
{
    if (colorCS->isValid() && widthSB->value() > 0 && heightSB->value() > 0 &&
        linethicknessSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

AddLineDialog::AddLineDialog(QWidget *parent)
      : TitledDialog(QIcon(":/resources/images/addline.png"),parent)
{
    setTitle("Add Line");
    
    QLabel *x1Label = new QLabel(tr("X1"));
    x1SB    = new QDoubleSpinBox;
    x1SB->setRange(0.0,1000000.0);
    x1SB->setSingleStep(0.1);
    x1Label->setBuddy(x1SB);
    
    QLabel *y1Label = new QLabel(tr("Y1"));
    y1SB    = new QDoubleSpinBox;
    y1SB->setRange(0.0,1000000.0);
    y1SB->setSingleStep(0.1);
    y1Label->setBuddy(y1SB);
    
    QLabel *x2Label = new QLabel(tr("X2"));
    x2SB    = new QDoubleSpinBox;
    x2SB->setRange(0.0,1000000.0);
    x2SB->setSingleStep(0.1);
    x2Label->setBuddy(x2SB);
    
    QLabel *y2Label = new QLabel(tr("Y2"));
    y2SB    = new QDoubleSpinBox;
    y2SB->setRange(0.0,1000000.0);
    y2SB->setSingleStep(0.1);
    y2Label->setBuddy(y2SB);
    
    QLabel *linethicknessLabel = new QLabel(tr("Line Thickness"));
    linethicknessSB    = new QDoubleSpinBox;
    linethicknessSB->setRange(0.0,1000000.0);
    linethicknessSB->setSingleStep(0.1);
    linethicknessLabel->setBuddy(linethicknessSB);
    
    QLabel *colorLabel    = new QLabel(tr("Color"));
    colorCS       = new ColorSelector;
    colorLabel->setBuddy(colorCS);
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(x1Label, 0, 0);
    mainLayout->addWidget(x1SB, 0, 1);
    mainLayout->addWidget(y1Label, 1, 0);
    mainLayout->addWidget(y1SB, 1, 1);
    mainLayout->addWidget(x2Label, 2, 0);
    mainLayout->addWidget(x2SB, 2, 1);
    mainLayout->addWidget(y2Label, 3, 0);
    mainLayout->addWidget(y2SB, 3, 1);
    mainLayout->addWidget(linethicknessLabel, 4, 0);
    mainLayout->addWidget(linethicknessSB, 4, 1);
    mainLayout->addWidget(colorLabel, 5, 0);
    mainLayout->addWidget(colorCS, 5, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 6, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
    
}

bool AddLineDialog::draw(double &x1, double &y1, double &x2, double &y2, double &linethickness, QColor &color, bool editing, QString title, QString button)
{
    if (editing) {
        x1SB->setValue(x1);
        y1SB->setValue(y1);
        x2SB->setValue(x2);
        y2SB->setValue(y2);
        linethicknessSB->setValue(linethickness);
        colorCS->setValue(color);
    }
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        x1 = x1SB->value();
        y1 = y1SB->value();
        x2 = x2SB->value();
        y2 = y2SB->value();
        linethickness = linethicknessSB->value();
        color = colorCS->value();
        return true;
    } else {
        return false;
    }
}

void AddLineDialog::acceptCheck()
{
    if (colorCS->isValid() && linethicknessSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

AddCircDialog::AddCircDialog(QWidget *parent)
      : TitledDialog(QIcon(":/resources/images/addcirc.png"),parent)
{
    setTitle(tr("Add Circle"));
    
    QLabel *xposLabel = new QLabel(tr("X Position"));
    xposSB    = new QDoubleSpinBox;
    xposSB->setRange(0.0,1000000.0);
    xposSB->setSingleStep(0.1);
    xposLabel->setBuddy(xposSB);
    
    QLabel *yposLabel = new QLabel(tr("Y Position"));
    yposSB    = new QDoubleSpinBox;
    yposSB->setRange(0.0,1000000.0);
    yposSB->setSingleStep(0.1);
    yposLabel->setBuddy(yposSB);
    
    QLabel *diameterLabel = new QLabel(tr("Diameter"));
    diameterSB    = new QDoubleSpinBox;
    diameterSB->setRange(0.0,1000000.0);
    diameterSB->setSingleStep(0.1);
    diameterLabel->setBuddy(diameterSB);
    
    QLabel *centerLabel = new QLabel(tr("Center"));
    centerSB    = new QDoubleSpinBox;
    centerSB->setRange(0.0,1000000.0);
    centerSB->setSingleStep(0.1);
    centerLabel->setBuddy(centerSB);
    
    QLabel *colorLabel    = new QLabel(tr("Color"));
    colorCS       = new ColorSelector;
    colorLabel->setBuddy(colorCS);
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(xposLabel, 0, 0);
    mainLayout->addWidget(xposSB, 0, 1);
    mainLayout->addWidget(yposLabel, 1, 0);
    mainLayout->addWidget(yposSB, 1, 1);
    mainLayout->addWidget(diameterLabel, 2, 0);
    mainLayout->addWidget(diameterSB, 2, 1);
    mainLayout->addWidget(centerLabel, 3, 0);
    mainLayout->addWidget(centerSB, 3, 1);
    mainLayout->addWidget(colorLabel, 4, 0);
    mainLayout->addWidget(colorCS, 4, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 5, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}

bool AddCircDialog::draw(double &xpos, double &ypos, double &diameter, double &center, QColor &color, bool editing, QString title, QString button)
{
    if (editing) {
        xposSB->setValue(xpos);
        yposSB->setValue(ypos);
        diameterSB->setValue(diameter);
        centerSB->setValue(center);
        colorCS->setValue(color);
    }
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        xpos = xposSB->value();
        ypos = yposSB->value();
        diameter = diameterSB->value();
        color = colorCS->value();
        center = centerSB->value();
        return true;
    } else {
        return false;
    }
}

void AddCircDialog::acceptCheck()
{
    if (colorCS->isValid() && diameterSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

AddArcDialog::AddArcDialog(QWidget *parent)
      : TitledDialog(QIcon(":/resources/images/addarc.png"),parent)
{
    setTitle(tr("Add Arc"));
    
    QLabel *xposLabel = new QLabel(tr("X Position"));
    xposSB    = new QDoubleSpinBox;
    xposSB->setRange(0.0,1000000.0);
    xposSB->setSingleStep(0.1);
    xposLabel->setBuddy(xposSB);
    
    QLabel *yposLabel = new QLabel(tr("Y Position"));
    yposSB    = new QDoubleSpinBox;
    yposSB->setRange(0.0,1000000.0);
    yposSB->setSingleStep(0.1);
    yposLabel->setBuddy(yposSB);
    
    QLabel *diameterLabel = new QLabel(tr("Diameter"));
    diameterSB    = new QDoubleSpinBox;
    diameterSB->setRange(0.0,1000000.0);
    diameterSB->setSingleStep(0.1);
    diameterLabel->setBuddy(diameterSB);
    
    QLabel *centerLabel = new QLabel(tr("Center"));
    centerSB    = new QDoubleSpinBox;
    centerSB->setRange(0.0,1000000.0);
    centerSB->setSingleStep(0.1);
    centerLabel->setBuddy(centerSB);
    
    QLabel *startLabel = new QLabel(tr("Starting Angle"));
    startSB  = new QDoubleSpinBox;
    startSB->setRange(0.0,360.0);
    startSB->setSingleStep(0.1);
    startLabel->setBuddy(startSB);
    
    QLabel *extentLabel = new QLabel(tr("Extent Angle"));
    extentSB  = new QDoubleSpinBox;
    extentSB->setRange(0.0,360.0);
    extentSB->setSingleStep(0.1);
    extentLabel->setBuddy(extentSB);
    
    QLabel *colorLabel    = new QLabel(tr("Color"));
    colorCS       = new ColorSelector;
    colorLabel->setBuddy(colorCS);
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(xposLabel, 0, 0);
    mainLayout->addWidget(xposSB, 0, 1);
    mainLayout->addWidget(yposLabel, 1, 0);
    mainLayout->addWidget(yposSB, 1, 1);
    mainLayout->addWidget(diameterLabel, 2, 0);
    mainLayout->addWidget(diameterSB, 2, 1);
    mainLayout->addWidget(centerLabel, 3, 0);
    mainLayout->addWidget(centerSB, 3, 1);
    mainLayout->addWidget(startLabel, 4, 0);
    mainLayout->addWidget(startSB, 4, 1);
    mainLayout->addWidget(extentLabel, 5, 0);
    mainLayout->addWidget(extentSB, 5, 1);
    mainLayout->addWidget(colorLabel, 6, 0);
    mainLayout->addWidget(colorCS, 6, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 7, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}


bool AddArcDialog::draw(double &xpos, double &ypos, double &diameter, double &center, double &start, double &extent, QColor &color, bool editing, QString title, QString button)
{
    if (editing) {
        xposSB->setValue(xpos);
        yposSB->setValue(ypos);
        diameterSB->setValue(diameter);
        centerSB->setValue(center);
        startSB->setValue(start);
        extentSB->setValue(extent);
        colorCS->setValue(color);
    }
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        xpos = xposSB->value();
        ypos = yposSB->value();
        diameter = diameterSB->value();
        center = centerSB->value();
        start = startSB->value();
        extent = extentSB->value();
        color = colorCS->value();
        return true;
    } else {
        return false;
    }
}

void AddArcDialog::acceptCheck()
{
    if (colorCS->isValid() && diameterSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

AddPolyDialog::AddPolyDialog(QWidget *parent)
      : TitledDialog(QIcon(":/resources/images/addpoly.png"),parent)
{
    setTitle(tr("Add Polygon"));
    
    QFrame *pointsFrame = new QFrame;
    pointsFrame->setFrameShadow(QFrame::Sunken);
    pointsFrame->setFrameShape(QFrame::Box);
    QGridLayout *pointsFrameLayout = new QGridLayout;
    QLabel *pointsLabel = new QLabel(tr("Poly Points:"));
    pointsFrameLayout->addWidget(pointsLabel, 0, 0);
    pointsLW = new QListWidget;
    pointsLW->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pointsFrameLayout->addWidget(pointsLW, 1, 0, 1, -1);
    newx = new QDoubleSpinBox;
    newx->setRange(0.0, 1000000.0);
    newx->setSingleStep(0.1);
    pointsFrameLayout->addWidget(newx, 2, 0);
    newy = new QDoubleSpinBox;
    newy->setRange(0.0, 1000000.0);
    newy->setSingleStep(0.1);
    pointsFrameLayout->addWidget(newy, 2, 1);
    addPoint = new QPushButton(tr("Add Point"));
    pointsFrameLayout->addWidget(addPoint, 2, 2);
    connect(addPoint,SIGNAL(clicked()),this,SLOT(addPointToList()));
    delPoints = new QPushButton(tr("Delete Selected Points"));
    connect(delPoints,SIGNAL(clicked()),this,SLOT(deletePointsFromList()));
    pointsFrameLayout->addWidget(delPoints,3, 0, 1, -1);
    pointsFrame->setLayout(pointsFrameLayout);
                        
    QLabel *linethicknessLabel = new QLabel(tr("Line Thickness"));
    linethicknessSB = new QDoubleSpinBox;
    linethicknessSB->setRange(0.0, 1000000.0);
    linethicknessSB->setSingleStep(0.1);
    linethicknessLabel->setBuddy(linethicknessSB);
    
    filledCK = new QCheckBox(tr("Filled?"));
    filledCK->setChecked(false);
    
    closedCK = new QCheckBox(tr("Closed?"));
    closedCK->setChecked(false);
    
    QLabel *colorLabel = new QLabel(tr("Color"));
    colorCS = new ColorSelector;
    colorCS->setValue(QColor("black"));
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(pointsFrame, 0, 0, 1, 2);
    mainLayout->addWidget(linethicknessLabel, 1, 0);
    mainLayout->addWidget(linethicknessSB, 1, 1);
    mainLayout->addWidget(filledCK, 2, 0, 1, 2);
    mainLayout->addWidget(closedCK, 3, 0, 1, 2);
    mainLayout->addWidget(colorLabel, 4, 0);
    mainLayout->addWidget(colorCS, 4, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole); 
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 5, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}

bool AddPolyDialog::draw(QPolygonF &points, double &linethickness, bool &filled, bool &closed, QColor &color, bool editing, QString title, QString button)
{
    if (editing) {
        while (pointsLW->count() > 0) {
            delete pointsLW->item(0);
        }
        for (QPolygonF::const_iterator ip = points.begin();
             ip !=  points.end();
             ip++) {
            QPointF p = *ip;
            QString s = QString("%1,%2").arg(p.x()).arg(p.y());
            QListWidgetItem *ele = new QListWidgetItem(s);
            ele->setData(Qt::UserRole,  QVariant(p));
            pointsLW->addItem(ele);
        }
        linethicknessSB->setValue(linethickness);
        filledCK->setChecked(filled);
        closedCK->setChecked(closed);
        colorCS->setValue(color);
    }
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        points.clear();
        for (int i = 0; i < pointsLW->count(); i++) {
            QListWidgetItem *ele = pointsLW->item(i);
            points.push_back(ele->data(Qt::UserRole).toPointF());
        }
        linethickness = linethicknessSB->value();
        filled = filledCK->isChecked();
        closed = closedCK->isChecked();
        color  = colorCS->value();
        return true;
    } else {
        return false;
    }
}

void AddPolyDialog::acceptCheck()
{
    if (colorCS->isValid() && pointsLW->count() >= 3) {
        done(QDialog::Accepted);
    }
}

void AddPolyDialog::addPointToList()
{
    double x = newx->value();
    double y = newy->value();
    QPointF newPoint(x,y);
    QString s = QString("%1,%2").arg(newPoint.x()).arg(newPoint.y());
    QListWidgetItem *ele = new QListWidgetItem(s);
    ele->setData(Qt::UserRole,  QVariant(newPoint));
    pointsLW->addItem(ele);
}

typedef QList<QListWidgetItem *> ItemList;
typedef ItemList::const_iterator ItemList_ConstIterator;
typedef ItemList::iterator ItemList_Iterator;

void AddPolyDialog::deletePointsFromList()
{
    ItemList selection = pointsLW->selectedItems();
    for (ItemList_ConstIterator is=selection.begin();
         is !=selection.end();
         is++) {
        delete *is;
    }
}


AddTextDialog::AddTextDialog(QWidget *parent)
      : TitledDialog(QIcon(":/resources/images/addtext.png"),parent)
{
    setTitle(tr("Add Text"));
    
    QLabel *xposLabel = new QLabel(tr("X Position"));
    xposSB    = new QDoubleSpinBox;
    xposSB->setRange(0.0,1000000.0);
    xposSB->setSingleStep(0.1);
    xposLabel->setBuddy(xposSB);
    
    QLabel *yposLabel = new QLabel(tr("Y Position"));
    yposSB    = new QDoubleSpinBox;
    yposSB->setRange(0.0,1000000.0);
    yposSB->setSingleStep(0.1);
    yposLabel->setBuddy(yposSB);
    
    QLabel *textLabel = new QLabel(tr("Text"));
    textLE    = new QLineEdit;
    textLabel->setBuddy(textLE);
    
    QLabel *fontLabel = new QLabel(tr("Font"));
    fontCB    = new QComboBox;
    fontCB->addItems(QString("DroidSans,DroidSans-Bold,DroidSansMono,OCRA").split(","));
    fontCB->setEditable(false);
    fontLabel->setBuddy(fontCB);
    
    QLabel *sizeLabel = new QLabel(tr("Size"));
    sizeSB    = new QDoubleSpinBox;
    sizeSB->setRange(0.0,1000.0);
    sizeSB->setSingleStep(0.1);
    sizeLabel->setBuddy(sizeSB);
    
    QLabel *colorLabel    = new QLabel(tr("Color"));
    colorCS       = new ColorSelector;
    colorLabel->setBuddy(colorCS);
    
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(xposLabel, 0, 0);
    mainLayout->addWidget(xposSB, 0, 1);
    mainLayout->addWidget(yposLabel, 1, 0);
    mainLayout->addWidget(yposSB, 1, 1);
    mainLayout->addWidget(textLabel, 2, 0);
    mainLayout->addWidget(textLE, 2, 1);
    mainLayout->addWidget(fontLabel, 3, 0);
    mainLayout->addWidget(fontCB, 3, 1);
    mainLayout->addWidget(sizeLabel, 4, 0);
    mainLayout->addWidget(sizeSB, 4, 1);
    mainLayout->addWidget(colorLabel, 5, 0);
    mainLayout->addWidget(colorCS, 5, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 6, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}

bool AddTextDialog::draw(double &xpos, double &ypos, QString &text, QString &font, double &size, QColor &color, bool editing, QString title, QString button)
{
    if (editing) {
        xposSB->setValue(xpos);
        yposSB->setValue(ypos);
        textLE->setText(text);
        fontCB->setCurrentText(font);
        sizeSB->setValue(size);
        colorCS->setValue(color);
    }
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        xpos = xposSB->value();
        ypos = yposSB->value();
        text = textLE->text();
        font = fontCB->currentText();
        size = sizeSB->value();
        color = colorCS->value();
        return true;
    } else {
        return false;
    }
}

void AddTextDialog::acceptCheck()
{
    if (colorCS->isValid() && sizeSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

}