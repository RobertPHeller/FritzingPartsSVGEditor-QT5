// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Fri May 24 08:03:03 2019
//  Last Modified : <190524.0841>
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

#ifndef __SVGTEXTSTATE_H
#define __SVGTEXTSTATE_H

#include <QGraphicsSimpleTextItem>
#include <QPaintEngineState>
#include <QTextItem>

class QBrush;
class QFont;

class FESimpleTextItemPEState : public QPaintEngineState {
public:
    FESimpleTextItemPEState(const QBrush &brush, const QFont &font)
          : _brush(brush), _font(font)
    {
    }
    QFont font() const { return _font; }
    QBrush brush() const { return _brush; }
    QPaintEngine::DirtyFlags state() const {
        return QPaintEngine::DirtyBrush|QPaintEngine::DirtyFont;
    }
private:
    QBrush _brush;
    QFont _font;
};

class FETextItem : public QTextItem
{
public:
    FETextItem(const QString &text) : _text(text) {}
    QString text() const {return _text;}
private:
    QString _text;
};
        
#endif // __SVGTEXTSTATE_H

