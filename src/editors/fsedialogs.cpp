// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue May 21 17:21:33 2019
//  Last Modified : <190521.2030>
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
#include "fsedialogs.h"

namespace Schematic {

PinOrientationAndInversion::PinOrientationAndInversion(QWidget *parent)
      : QComboBox(parent)
{
    addItem(QIcon(":/resources/images/left_normal"),"Left, not Inverted",QVariant((int) LeftNormal));
    addItem(QIcon(":/resources/images/right_normal"),"Right, not Inverted",QVariant((int) RightNormal));
    addItem(QIcon(":/resources/images/top_normal"),"Top, not Inverted",QVariant((int) TopNormal));
    addItem(QIcon(":/resources/images/bottom_normal"),"Bottom, not Inverted",QVariant((int) BottomNormal));
    addItem(QIcon(":/resources/images/left_inverted"),"Left, Inverted",QVariant((int) LeftInverted));
    addItem(QIcon(":/resources/images/right_inverted"),"Right, Inverted",QVariant((int) RightInverted));
    addItem(QIcon(":/resources/images/top_inverted"),"Top, Inverted",QVariant((int) TopInverted));
    addItem(QIcon(":/resources/images/bottom_inverted"),"Bottom, Inverted",QVariant((int) BottomInverted));
    setEditable(false);
}

void PinOrientationAndInversion::setValue(PinOrientationAndInversion::PinOrient orientation)
{
    int index = findData(QVariant((int) orientation));
    if (index < 0) {return;}
    setCurrentIndex(index);
}

PinOrientationAndInversion::PinOrient PinOrientationAndInversion::value() const
{
    return (PinOrientationAndInversion::PinOrient) itemData(currentIndex()).toInt();
}


AddPinDialog::AddPinDialog(QWidget *parent) 
      : TitledDialog(QIcon(":/resources/images/addpin.png"),parent)
{
    setTitle(tr("Add Pin"));
    
    QLabel *xposLabel = new QLabel(tr("X Position"));
    xposSB    = new QDoubleSpinBox;
    xposSB->setRange(0.0,1000000.0);
    xposSB->setSingleStep(0.1);
    xposSB->setDecimals(4);
    xposLabel->setBuddy(xposSB);
    
    QLabel *yposLabel = new QLabel(tr("Y Position"));
    yposSB    = new QDoubleSpinBox;
    yposSB->setRange(0.0,1000000.0);
    yposSB->setSingleStep(0.1);
    yposSB->setDecimals(4);
    yposLabel->setBuddy(yposSB);
    
    QLabel *orientLabel = new QLabel(tr("Orientation and Inversion"));
    orientCB = new PinOrientationAndInversion;
    orientLabel->setBuddy(orientCB);

    QLabel *lengthLabel = new QLabel(tr("Length"));
    lengthSB = new QDoubleSpinBox;
    lengthSB->setRange(0.0,1000000.0);
    lengthSB->setSingleStep(0.1);
    lengthSB->setDecimals(4);
    lengthLabel->setBuddy(lengthSB);
    
    QLabel *linethicknessLabel = new QLabel(tr("Line Thickness"));
    linethicknessSB    = new QDoubleSpinBox;
    linethicknessSB->setRange(0.0,1000000.0);
    linethicknessSB->setSingleStep(0.1);
    linethicknessSB->setDecimals(4);
    linethicknessLabel->setBuddy(linethicknessSB);
    
    QLabel *pinNameLabel = new QLabel(tr("Pin Name"));
    pinnameLE    = new QLineEdit;
    pinNameLabel->setBuddy(pinnameLE);
    
    QLabel *fontLabel = new QLabel(tr("Font"));
    fontCB    = new QComboBox;
    fontCB->addItems(QString("DroidSans,DroidSans-Bold,DroidSansMono,OCRA").split(","));
    fontCB->setEditable(false);
    fontLabel->setBuddy(fontCB);
    
    QLabel *sizeLabel = new QLabel(tr("Size"));
    sizeSB    = new QSpinBox;
    sizeSB->setRange(0,100);
    sizeSB->setSingleStep(1);
    sizeLabel->setBuddy(sizeSB);
    
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
    mainLayout->addWidget(orientLabel, 2, 0);
    mainLayout->addWidget(orientCB, 2, 1);
    mainLayout->addWidget(lengthLabel, 3, 0);
    mainLayout->addWidget(lengthSB, 3, 1);
    mainLayout->addWidget(linethicknessLabel, 4, 0);
    mainLayout->addWidget(linethicknessSB, 4, 1);
    mainLayout->addWidget(pinNameLabel, 5, 0);
    mainLayout->addWidget(pinnameLE, 5, 1);
    mainLayout->addWidget(fontLabel, 6, 0);
    mainLayout->addWidget(fontCB, 6, 1);
    mainLayout->addWidget(sizeLabel, 7, 0);
    mainLayout->addWidget(sizeSB, 7, 1);
    mainLayout->addWidget(colorLabel, 8, 0);
    mainLayout->addWidget(colorCS, 8, 1);
    mainLayout->addWidget(pinnoLabel, 9, 0);
    mainLayout->addWidget(pinnoSB, 9, 1);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);
    buttonBox->addButton(addButton,QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, 10, 0, 1, 2);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
}

bool AddPinDialog::draw(double &xpos, double &ypos, 
                        PinOrientationAndInversion::PinOrient &orientation, 
                        double &length, double & linethickness, int &pinno, 
                        QString &pinname, QString &font, int &size, 
                        QColor &color, bool editing, QString title, 
                        QString button)
{
    if (editing) {
        xposSB->setValue(xpos);
        yposSB->setValue(ypos);
        orientCB->setValue(orientation);
        lengthSB->setValue(length);
        linethicknessSB->setValue(linethickness);
        pinnameLE->setText(pinname);
        fontCB->setCurrentText(font);
        sizeSB->setValue(size);
        colorCS->setValue(color);
    }
    pinnoSB->setValue(pinno);
    setTitle(title);
    addButton->setText(button);
    if (exec() == QDialog::Accepted) {
        xpos = xposSB->value();
        ypos = yposSB->value();
        orientation = orientCB->value();
        length = lengthSB->value();
        linethickness = linethicknessSB->value();
        pinname = pinnameLE->text();
        font = fontCB->currentText();
        size = sizeSB->value();
        color = colorCS->value();
        pinno = pinnoSB->value();
        return true;
    } else {
        return false;
    }
}

void AddPinDialog::acceptCheck()
{
    if (colorCS->isValid() && pinnoSB->value() > 0 && lengthSB->value() > 0 && 
        linethicknessSB->value() > 0 && sizeSB->value() > 0) {
        done(QDialog::Accepted);
    }
}
}
