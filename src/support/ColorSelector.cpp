// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sat May 18 14:58:55 2019
//  Last Modified : <190518.2032>
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
#include <QColor>
#include <QPushButton>
#include <QPixmap>
#include <QColorDialog>
#include <QLineEdit>
#include <QHBoxLayout>

#include "ColorSelector.h"
#include "palette.xpm"


ColorSelector::ColorSelector(QWidget *parent) : QWidget(parent)
{
    color = QColor("black");
    colorEdit = new QLineEdit;
    colorEdit->setValidator(new ColorValidator);
    colorSelButton = new QPushButton( QPixmap(_palette), "" );
    connect(colorSelButton, SIGNAL(clicked()), this, SLOT(select()));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(colorEdit);
    layout->addWidget(colorSelButton);
    setLayout(layout);
    colorSelDialog = NULL;
    colorEdit->setText(color.name());
}

bool ColorSelector::isValid() const
{
    return colorEdit->hasAcceptableInput();
}

void ColorSelector::select()
{
    if (colorSelDialog == NULL) {
        colorSelDialog = new QColorDialog(this);
    }
    if (colorSelDialog->exec() ==  QDialog::Accepted) {
        color = colorSelDialog->selectedColor();
        colorEdit->setText(color.name());
    }
}
    
