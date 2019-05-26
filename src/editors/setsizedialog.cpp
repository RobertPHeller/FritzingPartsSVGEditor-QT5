// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun May 26 09:27:59 2019
//  Last Modified : <190526.1033>
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
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QDebug>

#include "../support/TitledDialog.h"
#include "feedit.h"
#include "../support/debug.h"
#include "setsizedialog.h"

SetSizeDialog::SetSizeDialog(QWidget *parent)
      : TitledDialog(TitledDialog::Question,parent)
{
    setTitle(tr("Set size and viewport"));
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGroupBox *unitsGroup = new QGroupBox(tr("Units"));
    mmRB = new QRadioButton("mm");
    inRB = new QRadioButton("inch");
    mmRB->setChecked(true);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(mmRB);
    hbox->addWidget(inRB);
    hbox->addStretch(1);
    unitsGroup->setLayout(hbox);
    mainLayout->addWidget(unitsGroup);
    QGroupBox *sizeGroup = new QGroupBox(tr("Size"));
    QGridLayout *gbox = new QGridLayout;
    QLabel *wLabel = new QLabel(tr("Width"));
    widthSB = new QDoubleSpinBox;
    widthSB->setRange(1.0,10000.0);
    widthSB->setSingleStep(0.1);
    widthSB->setDecimals(4);
    wLabel->setBuddy(widthSB);
    gbox->addWidget(wLabel,0,0);
    gbox->addWidget(widthSB,0,1);
    QLabel *hLabel = new QLabel(tr("Height"));
    heightSB = new QDoubleSpinBox;
    heightSB->setRange(1.0,10000.0);
    heightSB->setSingleStep(0.1);
    heightSB->setDecimals(4);
    hLabel->setBuddy(heightSB);
    gbox->addWidget(hLabel,1,0);
    gbox->addWidget(heightSB,1,1);
    sizeGroup->setLayout(gbox);
    mainLayout->addWidget(sizeGroup);
    QGroupBox *viewportGroup = new QGroupBox(tr("Viewport"));
    gbox = new QGridLayout;
    QLabel *vpxLabel = new QLabel(tr("X"));
    vp_xSB = new QDoubleSpinBox;
    vp_xSB->setRange(0.0,10000.0);
    vp_xSB->setSingleStep(0.1);
    vp_xSB->setDecimals(4);
    vpxLabel->setBuddy(vp_xSB);
    gbox->addWidget(vpxLabel,0,0);
    gbox->addWidget(vp_xSB,0,1);
    QLabel *vpyLabel = new QLabel(tr("Y"));
    vp_ySB = new QDoubleSpinBox;
    vp_ySB->setRange(0.0,10000.0);
    vp_ySB->setSingleStep(0.1);
    vp_ySB->setDecimals(4);
    vpyLabel->setBuddy(vp_ySB);
    gbox->addWidget(vpyLabel,1,0);
    gbox->addWidget(vp_ySB,1,1);
    QLabel *vpwidthLabel = new QLabel(tr("Width"));
    vp_widthSB = new QDoubleSpinBox;
    vp_widthSB->setRange(1.0,10000.0);
    vp_widthSB->setSingleStep(0.1);
    vp_widthSB->setDecimals(4);
    vpwidthLabel->setBuddy(vp_widthSB);
    gbox->addWidget(vpwidthLabel,2,0);
    gbox->addWidget(vp_widthSB,2,1);
    QLabel *vpheightLabel = new QLabel(tr("Height"));
    vp_heightSB = new QDoubleSpinBox;
    vp_heightSB->setRange(1.0,10000.0);
    vp_heightSB->setSingleStep(0.1);
    vp_heightSB->setDecimals(4);
    vpheightLabel->setBuddy(vp_heightSB);
    gbox->addWidget(vpheightLabel,3,0);
    gbox->addWidget(vp_heightSB,3,1);
    viewportGroup->setLayout(gbox);
    mainLayout->addWidget(viewportGroup);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptCheck()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

void SetSizeDialog::acceptCheck()
{
    if (widthSB->value() > 0 && heightSB->value() > 0 &&
        vp_widthSB->value() > 0 && vp_heightSB->value() > 0 &&
        vp_xSB->value() >= 0 && vp_ySB->value() >= 0) {
        done(QDialog::Accepted);
    }
}

bool SetSizeDialog::draw(double &width, double &height, QRectF &viewport, 
                         SizeAndVP::UnitsType &units)
{
    widthSB->setValue(width);
    heightSB->setValue(height);
    vp_xSB->setValue(viewport.x());
    vp_ySB->setValue(viewport.y());
    vp_widthSB->setValue(viewport.width());
    vp_heightSB->setValue(viewport.height());
    switch (units) {
    case SizeAndVP::mm: mmRB->setChecked(true); break;
    case SizeAndVP::in: inRB->setChecked(true); break;
    }
    if (exec() == QDialog::Accepted) {
        width = widthSB->value();
        height = heightSB->value();
        viewport.setX(vp_xSB->value());
        viewport.setY(vp_ySB->value());
        viewport.setWidth(vp_widthSB->value());
        viewport.setHeight(vp_heightSB->value());
        if (mmRB->isChecked()) units = SizeAndVP::mm;
        else if (inRB->isChecked()) units = SizeAndVP::in;
        return true;
    } else {
        return false;
    }
}
