// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Mon May 20 15:53:26 2019
//  Last Modified : <190520.1651>
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

#ifndef __DEBUG_H
#define __DEBUG_H

#include <QIODevice>
#include <QDebug>
#include <iostream>

class StdError : public QIODevice {
    Q_OBJECT
public:
    explicit StdError(QObject *parent = NULL) : QIODevice(parent) 
    {
        open(QIODevice::WriteOnly);
    }
    qint64 writeData(const char *data, qint64 maxSize) 
    {
        for (int i = 0; i < maxSize; i++) {
            std::cerr << *data++;
        }
        std::cerr.flush();
        close();
        open(QIODevice::WriteOnly);
        return maxSize;
    }
    qint64 readData(char *data, qint64 maxSize) {return 0;}
};

extern QDebug stdError;


#endif // __DEBUG_H

