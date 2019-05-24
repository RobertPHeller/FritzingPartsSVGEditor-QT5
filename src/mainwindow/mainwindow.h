// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 11:58:51 2019
//  Last Modified : <190522.1621>
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

#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QStatusBar>
#include <QMessageBox>

#include "../editors/feedit.h"

class FEBreadboardEditor;
class FESchematicEditor;
class FEPCBEditor;
#include <QRectF>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &fileName);
    SizeAndVP::UnitsType Units() const {return _units;}
    double Width() const {return _width;}
    double Height() const {return _height;}
    void Viewport (QRectF &vp) {
        vp = _viewport;
    }
        
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    
    void editPrefs() {}
    void loadPrefs() {readSettings();}
    void savePrefs() {writeSettings();}
    
    void help_Help() {help("Help");}
    void help_Version() {help("Version");}
    void help_Warranty() {help("Warranty");}
    void help_Copying() {help("Copying");}
    void help_FritzingPartsSVGEditor() {help("Fritzing Parts SVG Editor");}
    void help_MainGUI() {help("Main GUI");}
    void help_Preferences() {help("Preferences");}
    void help_CommonGUIelements() {help("Common GUI elements");}
    void help_BreadboardEditor() {help("Breadboard Editor");}
    void help_SchematicEditor() {help("Schematic Editor");}
    void help_PCBEditor() {help("PCB Editor");}
    

    void documentWasModified(bool dirty);
private:
    SizeAndVP::UnitsType _units;
    double _width, _height;
    QRectF _viewport;
    
    void createActions();
    void createMenus();
    //void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    
    void help(const QString &helpIndexText);
    
    QTabWidget *notebook; // Tab notebook
    FEBreadboardEditor *breadboardeditor; // Breadboard Editor tab
    FESchematicEditor *schematiceditor; // Schematic Editor tab
    FEPCBEditor *pcbeditor; // PCB Editor tab
    
    const QString getOpenPrefix (const QString& defaultdir = QString());
    const QString getSavePrefix (const QString& defaultdir = QString());
    
    QString curFile;
    
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;
    //QToolBar *fileToolBar;
    //QToolBar *editToolBar;
    // File menu
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    // Edit Menu
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    // Options Menu
    QAction *editPrefsAct;
    QAction *loadPrefsAct;
    QAction *savePrefsAct;
    // Help menu
    QAction *helpHelpAct;
    QAction *helpVersionAct;
    QAction *helpWarrantyAct;
    QAction *helpCopyingAct;
    QAction *helpShellAct;
    QAction *helpMainGUIAct;
    QAction *helpPreferencesAct;
    QAction *helpCommonGUIelementsAct;
    QAction *helpBreadboardEditorAct;
    QAction *helpSchematicEditorAct;
    QAction *helpPCBEditorAct;
};

#endif // __MAINWINDOW_H

