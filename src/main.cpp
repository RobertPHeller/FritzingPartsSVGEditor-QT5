// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 07:56:55 2019
//  Last Modified : <190526.1941>
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

/** 
  * 
  * @mainpage Fritzing Parts SVG Editor
  * @anchor toc
  * @htmlonly
  * <div class="contents">
  * <div class="textblock"><ol type="1">
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html">Fritzing Parts SVG Editor</a><ol type="1">
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html#FritzingPartsSVGEditorSYNOPSIS">SYNOPSIS</a></li>
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html#FritzingPartsSVGEditorDESCRIPTION">DESCRIPTION</a></li>
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html#FritzingPartsSVGEditorOPTIONS">OPTIONS</a></li>
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html#FritzingPartsSVGEditorPARAMETERS">PARAMETERS</a></li>
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html#FritzingPartsSVGEditorFILES">FILES</a></li>
  * <li><a class="el" href="group__FritzingPartsSVGEditor.html#FritzingPartsSVGEditorAUTHOR">AUTHOR</a></li>
  * </ol></li>
  * <li><a class="el" href="mainGUI.html">Main GUI</a><ol type="a">
  * <li><a class="el" href="mainGUI.html#FileMenu">File menu</a></li>
  * <li><a class="el" href="mainGUI.html#EditMenu">Edit menu</a></li>
  * <li><a class="el" href="mainGUI.html#OptionsMenu">Options menu</a></li>
  * <li><a class="el" href="mainGUI.html#HelpMenu">Help menu</a></li>
  * <li><a class="el" href="mainGUI.html#TabNotebook">Editing tabs</a></li>
  * </ol></li>
  * <li><a class="el" href="preferences.html">Preferences</a></li>
  * <li><a class="el" href="FritzingPartsEditor.html">Common GUI elements</a><ol type="a">
  * <li><a class="el" href="FritzingPartsEditor.html#CoordinateInfo">Coordinate Information</a></li>
  * <li><a class="el" href="FritzingPartsEditor.html#ToolButtons">Tool Buttons</a></li>
  * <li><a class="el" href="FritzingPartsEditor.html#bindings">Keyboard and mouse bindings</a></li>
  * </ol></li>
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html">Breadboard Editor</a><ol type="a">
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html#BreadboardPin">Pins</a></li>
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html#BreadboardRect">Rectangles</a></li>
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html#BreadboardLine">Lines</a></li>
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html#BreadboardCirc">Circles</a></li>
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html#BreadboardArc">Arcs</a></li>
  * <li><a class="el" href="FritzingPartsBreadboardEditor.html#BreadboardText">Text</a></li>
  * </ol></li>
  * <li><a class="el" href="FritzingPartsSchematicEditor.html">Schematic Editor</a><ol type="a">
  * <li><a class="el" href="FritzingPartsSchematicEditor.html#SchematicPin">Pins</a></li>
  * <li><a class="el" href="FritzingPartsSchematicEditor.html#SchematicRect">Rectangles</a></li>
  * <li><a class="el" href="FritzingPartsSchematicEditor.html#SchematicLine">Lines</a></li>
  * <li><a class="el" href="FritzingPartsSchematicEditor.html#SchematicCirc">Circles</a></li>
  * <li><a class="el" href="FritzingPartsSchematicEditor.html#SchematicArc">Arcs</a></li>
  * <li><a class="el" href="FritzingPartsSchematicEditor.html#SchematicText">Text</a></li>
  * </ol></li>
  * <li><a class="el" href="FritzingPartsPCBEditor.html">PCB Editor</a><ol type="a">
  * <li><a class="el" href="FritzingPartsPCBEditor.html#PCBPin">Pins</a></li>
  * <li><a class="el" href="FritzingPartsPCBEditor.html#PCBRect">Rectangles</a></li>
  * <li><a class="el" href="FritzingPartsPCBEditor.html#PCBLine">Lines</a></li>
  * <li><a class="el" href="FritzingPartsPCBEditor.html#PCBCirc">Circles</a></li>
  * <li><a class="el" href="FritzingPartsPCBEditor.html#PCBArc">Arcs</a></li>
  * <li><a class="el" href="FritzingPartsPCBEditor.html#PCBText">Text</a></li>
  * </ol></li>
  * <li><a class="el" href="help.html">Help</a></li>
  * <li><a class="el" href="Version.html">Version</a></li>
  * <li><a class="el" href="Copying.html">Copying</a><ol type="a">
  * <li><a class="el" href="Copying.html#Warranty">Warranty</a></li>
  * </ol></li>
  * </ol></div></div>
  * @endhtmlonly
  * @latexonly
  * This program is meant to fill a missing feature of the Fritzing program, an
  * editor for creating the part graphics.  Inkscape or Illustrator are really
  * poor choices for creating these sorts of graphics or at least I found 
  * Inkscape frustrating to use to create breadboard or PCB layouts, esp. when 
  * compared to KiCAD's footprint editor.  Placing accurately sized graphics in
  * accurate locations is not really what Inkscape is specifically designed to do.
  * It is meant for creating artwork.  For my initial parts, I ended up hand 
  * editing the SVG files with a text editor, with a little scripting help. Then
  * I wrote this program.  Unlike "typical" graphics editing program, this 
  * program does not allow for placing or moving graphical elements with the
  * pointer (mouse, etc.).  Instead, to place or edit a graphical element, a
  * dialog box is displayed and one enters the exact numerical location, size,
  * etc.  This might seem clunky to some people, but it is designed to allow 
  * direct transfer from a datasheet drawing, which after all is loaded down with
  * actual measurements (numbers!).  For people who have an aversion to their
  * keyboards, there is always Inkscape or Illustrator.
  *
  * Fritzing should eventually have its own built-in graphical part editor (much
  * like KiCAD (and I guess EagleCAD) does.  I have no problem with the Fritzing
  * developers using this program as a basis or inspiration.
  * @endlatexonly
  *
  * @defgroup FritzingPartsSVGEditor Fritzing Parts SVG Editor
  * Create and edit SVG SVG files used as the images for the Fritzing Parts 
  * editor.
  *
  * @section FritzingPartsSVGEditorSYNOPSIS SYNOPSIS
  *
  * FritzingPartsSVGEditor [X11 Resource Options] [options] [fileprefix]
  *  
  * @section FritzingPartsSVGEditorDESCRIPTION DESCRIPTION
  *
  * The Fritzing Parts SVG Editor creates and edits the SVG files used as the 
  * images for the Fritzing Parts editor.  It is \b NOT drawing program (like
  * inkscape or Adobe Illustrator).  It is oriented towards placing graphical
  * elements precisely, using exact numberical placement values.  Be prepared to
  * make extensive use of the oblong thing with the 100 or so buttons on it and
  * expect to make little use of the other thing you move around!  There is no
  * dragging or dropping or click to place elements.  The lack of such features
  * is deliberate.  Please read the whole manual for complete documentation on
  * the use of this program.
  *
  * @section FritzingPartsSVGEditorOPTIONS OPTIONS
  *
  * None at present.
  *
  * @section FritzingPartsSVGEditorPARAMETERS PARAMETERS
  *
  * An option filename prefix.  The files the program uses are named using this 
  * prefix, with "_Breadboard.svg", "_Schematic.svg", and "_PCB.svg" appended.
  *
  * @section FritzingPartsSVGEditorFILES FILES
  *
  * Preferences file (Tcl/Tk otions format):
  *
  * \$(HOME)/.fritzingpartssvgeditor
  * or
  * \$(HOME)/fritzingpartssvgeditor.rc
  *
  * @section FritzingPartsSVGEditorAUTHOR AUTHOR
  * Robert Heller \<heller\@deepsoft.com\>
  * @page mainGUI Main GUI
  * The main gui, shown below, has standard menu bar and three tabs, one for 
  * each of the three images used in Fritzing: Breadboard view, Schematic, and
  * PCB view.
  * @image latex Typical_MainGUI.png "The main GUI of the Fritzing Parts SVG Editor program" width=4in
  * @image html  Typical_MainGUISmall.png
  * There are four menus on the top menu bar, a file menu, an edit menu, an
  * options (preferences) menu, and a help menu.
  * @section FileMenu File menu
  * @image latex FileMenu.png "The File menu"
  * @image html  FileMenu.png
  * The \b File menu has the has the standard items: \b New, which clears the 
  * current part, \b Open, which loads a part from disk, \b Save and 
  * \b Save \b As..., which saves the current part to disk, and \b Edit, which
  * exits the program.
  *
  * The \b Open and \b Save options don't take a final filename, instead they
  * take a "prefix", to which is appended "_Breadboard.svg", "_Schematic.svg",
  * and "_PCB.svg" for each of the three views.  In other words, opening and
  * saving involves three files (although one or two can be ommited on input,
  * in which case the missing files yield a blank image for those view(s)).
  * @section EditMenu Edit menu
  * @image latex EditMenu.png "The Edit menu"
  * @image html  EditMenu.png
  * The \b Edit menu has the standard editing related items.
  * @section OptionsMenu Options menu
  * @image latex OptionsMenu.png "The Options menu"
  * @image html  OptionsMenu.png
  * The \b Options menu has three items: \b Edit \b Configuration, 
  * \b Load \b Configuration, and \b Save \b Configuration.  These items allow
  * for editing, loading, and saving the configuration (preferences).  See
  * \ref preferences.
  * @section HelpMenu Help menu
  * @image latex HelpMenu.png "The Help menu" width=4in
  * @image html  HelpMenuSmall.png
  * The \b Help menu contains a top-level index into the included help pages.
  * @section TabNotebook Editing tabs
  * The main area of the main GUI contains three tabbed panes, one for  each of 
  * the three images used in Fritzing: Breadboard view, Schematic, and PCB view.
  * All three have the same layout with the same GUI elements, but are actually
  * independent.  Each tabbed pane contains an image view area, a set of tool 
  * buttons to the right and coordinate information along the bottom.  The tool
  * buttons and the coordinate information are the same for all three views, 
  * anthough the details of how the some of the tool buttons work are tab 
  * specific and the actually coordinate information is itself specific to each
  * tab.  See \ref FritzingPartsEditor for details about the common features and
  * \ref FritzingPartsBreadboardEditor, \ref FritzingPartsSchematicEditor, and
  * \ref FritzingPartsPCBEditor for the tab specific features and functions.
  *
  * @page preferences Preferences
  * The preferences are stored in a text file in the user's home directory 
  * (folder).  The file is named \c .fritzingpartssvgeditor under Linux and 
  * MacOSX and \c fritzingpartssvgeditor.rc under MS-Windows.  There are five 
  * preferences:
  * 
  *  <dl>
  *  <dt>Units</dt><dd>The units to use for the width and height. Can me either 
  *  mm (milimeters) or inch (inches)</dd>
  *  <dt>Width</dt><dd>The real world width (in Units above) of the viewport.</dd>
  *  <dt>Height</dt><dd>The real world height (in Units above) of the viewport.</dd>
  *  <dt>Viewport Width</dt><dd>The numerical width of the viewport</dd>
  *  <dt>Viewport Height</dt><dd>The numerical height of the viewport</dd>
  *  </dl>
  * @addindex Coordinate System
  *
  * These are the default initial values to use.  When loading a file, the 
  * values stored in the file are used.  The aspect ratio of the Width to Height
  * should be the same as the aspect ratio of the Viewport Width to Viewport 
  * Height to insure square pixels.  Typically the Viewport Width and Viewport 
  * Height will be a constant multiple of the Width and Height respectively.  The
  * Viewport determines the coordinate system used to place and size graphical 
  * elements.  The Viewport origin (upper left corner) is always 0,0.
  */



#include <QTextStream>
#include <QFile>
#include <QtDebug>
#include <QApplication>

#include "support/debug.h"
QDebug stdError = qDebug(); //(new StdError(NULL));
#include "mainwindow/mainwindow.h"
#include "feapplication.h"
#include "support/commonDialogs.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(FritzingPartsSVGEditor);
    Q_INIT_RESOURCE(htmldoc);
    FEApplication app(argc, argv);
    QStringList args = app.arguments();
    QString file("");
    if (args.size() > 1) {
        file = args.at(1);
    }
    CommonDialog::InitCommonDialogs();
    MainWindow mainWin(file);
    mainWin.show();
    return app.exec();
}

     
    
