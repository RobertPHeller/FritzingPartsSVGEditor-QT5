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
//  Last Modified : <190519.0046>
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
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "../support/ColorSelector.h"
#include "fbedialogs.h"

namespace Breadboard {

AddPinDialog::AddPinDialog(QWidget *parent) : QDialog(parent)
{
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

bool AddPinDialog::draw(double &xpos, double &ypos, double &diameter, QColor &color, int &pinno)
{
    xposSB->setValue(xpos);
    yposSB->setValue(ypos);
    diameterSB->setValue(diameter);
    colorCS->setValue(color);
    pinnoSB->setValue(pinno);
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
    if (colorCS->isValid() && diameterSB->value() > 0) {
        done(QDialog::Accepted);
    }
}

AddRectDialog::AddRectDialog(QWidget *parent) : QDialog(parent)
{
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

bool AddRectDialog::draw(double &xpos, double &ypos, double &width, double &height, double &linethickness, QColor &color, bool &filled)
{
    xposSB->setValue(xpos);
    yposSB->setValue(ypos);
    widthSB->setValue(width);
    heightSB->setValue(height);
    linethicknessSB->setValue(linethickness);
    colorCS->setValue(color);
    filledCKB->setChecked(filled);
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

}
