// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Tue May 21 15:43:38 2019
//  Last Modified : <190521.1605>
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

#include <QApplication>
#include <QFontDatabase>
#include <QFileInfo>
#include <QSet>
#include <QMultiHash>
#include <QString>

#include "feapplication.h"
#include "installedfonts.h"

QSet<QString> InstalledFonts::InstalledFontsList;
QMultiHash<QString, QString> InstalledFonts::InstalledFontsNameMapper;   // family name to filename; SVG files seem to have to use filename


FEApplication::FEApplication(int & argc, char ** argv) 
      : QApplication(argc,argv)
{
    registerFonts();
    setOrganizationName("DeepSoft");
    setApplicationName("FritzingPartsSVGEditor");
}

FEApplication::~FEApplication()
{
}


void FEApplication::registerFonts() {
    registerFont(":/resources/fonts/DroidSans.ttf", true);
    registerFont(":/resources/fonts/DroidSans-Bold.ttf", false);
    registerFont(":/resources/fonts/DroidSansMono.ttf", false);
    registerFont(":/resources/fonts/OCRA.ttf", true);
}

void FEApplication::registerFont(const QString &fontFile, bool reallyRegister) {
    int id = QFontDatabase::addApplicationFont(fontFile);
    if(id > -1 && reallyRegister) {
        QStringList familyNames = QFontDatabase::applicationFontFamilies(id);
        QFileInfo finfo(fontFile);
        foreach (QString family, familyNames) {
            InstalledFonts::InstalledFontsNameMapper.insert(family, finfo.completeBaseName());
            InstalledFonts::InstalledFontsList << family;
            //DebugDialog::debug(QString("registering font family: %1 %2").arg(family).arg(finfo.completeBaseName()));
        }
    }
}

