// -!- C++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Thu May 16 12:20:12 2019
//  Last Modified : <190522.1110>
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

#include <QtWidgets>
#include <QRectF>

#include "mainwindow.h"
#include "../editors/fbeedit.h"
#include "../editors/fseedit.h"
#include "../editors/fpeedit.h"
#include "../support/debug.h"

MainWindow::MainWindow(const QString &fileName)
{

    notebook = new QTabWidget;
    setCentralWidget(notebook);
    
    createActions();
    createMenus();
    //createToolBars();
    createStatusBar();
    
    readSettings();
    
    breadboardeditor = new FEBreadboardEditor(_units,_width,_height,_viewport);
    notebook->addTab(breadboardeditor,QIcon(":/resources/images/clean.png"),"Breadboard View");
    schematiceditor  = new FESchematicEditor(_units,_width,_height,_viewport);
    notebook->addTab(schematiceditor,QIcon(":/resources/images/clean.png"),"Schematic View");
    pcbeditor        = new FEPCBEditor(_units,_width,_height,_viewport);
    notebook->addTab(pcbeditor,QIcon(":/resources/images/clean.png"),"PCB View");
    
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    } else {
        setCurrentFile("");
    }
    setUnifiedTitleAndToolBarOnMac(true);
    connect(breadboardeditor,SIGNAL(dirtyFlagChanged(bool)),this,SLOT(documentWasModified(bool)));
    connect(schematiceditor,SIGNAL(dirtyFlagChanged(bool)),this,SLOT(documentWasModified(bool)));
    connect(pcbeditor,SIGNAL(dirtyFlagChanged(bool)),this,SLOT(documentWasModified(bool)));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        //breadboardeditor->clean();
        //schematiceditor->clean();
        //pcbeditor->clean();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        //QString fileName = QFileDialog::getOpenFileName(this);
        //if (!fileName.isEmpty())
        //    loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    //QFileDialog dialog(this);
    //dialog.setWindowModality(Qt::WindowModal);
    //dialog.setAcceptMode(QFileDialog::AcceptSave);
    //dialog.exec();
    //QStringList files = dialog.selectedFiles();
    
    //if (files.isEmpty())
    //    return false;
    
    //return saveFile(files.at(0));
    return false;
}

void MainWindow::documentWasModified(bool dirty)
{    
    //stdError << "MainWindow::documentWasModified(" << dirty << ")\n";
    int beIndex = notebook->indexOf(breadboardeditor),
          seIndex = notebook->indexOf(schematiceditor),
          peIndex = notebook->indexOf(pcbeditor);
    
    //stdError << "MainWindow::documentWasModified() breadboardeditor dirty flag is " << breadboardeditor->isDirty() << "\n";
    //stdError << "MainWindow::documentWasModified() schematiceditor dirty flag is " << schematiceditor->isDirty() << "\n";
    //stdError << "MainWindow::documentWasModified() pcbeditor dirty flag is " << pcbeditor->isDirty() << "\n";
    if (breadboardeditor->isDirty()) {
        notebook->setTabIcon(beIndex,QIcon(":/resources/images/dirty.png"));
    } else {
        notebook->setTabIcon(beIndex,QIcon(":/resources/images/clean.png"));
    }
    if (schematiceditor->isDirty()) {
        notebook->setTabIcon(seIndex,QIcon(":/resources/images/dirty.png"));
    } else {
        notebook->setTabIcon(seIndex,QIcon(":/resources/images/clean.png"));
    }
    if (pcbeditor->isDirty()) {
        notebook->setTabIcon(peIndex,QIcon(":/resources/images/dirty.png"));
    } else {
        notebook->setTabIcon(peIndex,QIcon(":/resources/images/clean.png"));
    }
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/resources/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("New blank SVG files"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    
    openAct = new QAction(QIcon(":/resources/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open existing SVG files"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    
    saveAct = new QAction(QIcon(":/resources/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    openAct->setStatusTip(tr("Save SVG files"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    
    saveAsAct = new QAction(QIcon(":/resources/images/save.png"), tr("&Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save SVG files"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
    
    exitAct = new QAction(QIcon(":/resources/images/exit.png"), tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    
    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selection to the paste buffer"));
    //connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));
    
    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selection to the paste buffer"));
    //connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));
    
    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Past selection from the paste buffer"));
    //connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
    
    editPrefsAct = new QAction(tr("Edit Configuration"), this);
    editPrefsAct->setStatusTip(tr("Edit Configuration"));
    connect(editPrefsAct, SIGNAL(triggered()), this, SLOT(editPrefs()));
    
    loadPrefsAct = new QAction(tr("Load Configuration"), this);
    loadPrefsAct->setStatusTip(tr("Load Configuration"));
    connect(loadPrefsAct, SIGNAL(triggered()), this, SLOT(loadPrefs()));
    
    savePrefsAct = new QAction(tr("Save Configuration"), this);
    savePrefsAct->setStatusTip(tr("Save Configuration"));
    connect(savePrefsAct, SIGNAL(triggered()), this, SLOT(savePrefs()));
    
    helpHelpAct = new QAction(tr("On &Help..."), this);
    helpHelpAct->setStatusTip(tr("Help on help"));
    connect(helpHelpAct, SIGNAL(triggered()), this, SLOT(help_Help()));
    
    helpVersionAct = new QAction(tr("On &Version"), this);
    helpVersionAct->setStatusTip(tr("Version"));
    connect(helpVersionAct, SIGNAL(triggered()), this, SLOT(help_Version()));
    
    helpWarrantyAct = new QAction(tr("Warranty"), this);
    helpWarrantyAct->setStatusTip(tr("Warranty"));
    connect(helpWarrantyAct, SIGNAL(triggered()), this, SLOT(help_Warranty()));
    
    helpCopyingAct = new QAction(tr("Copying"), this);
    helpCopyingAct->setStatusTip(tr("Copying"));
    connect(helpCopyingAct, SIGNAL(triggered()), this, SLOT(help_Copying()));
    
    helpShellAct = new QAction(tr("Invoking from the shell"), this);
    helpShellAct->setStatusTip(tr("Invoking"));
    connect(helpShellAct, SIGNAL(triggered()), this, SLOT(help_FritzingPartsSVGEditor()));
    
    helpMainGUIAct = new QAction(tr("Main GUI"), this);
    helpMainGUIAct->setStatusTip(tr("Main GUI"));
    connect(helpMainGUIAct, SIGNAL(triggered()), this, SLOT(help_MainGUI()));
    
    helpPreferencesAct = new QAction(tr("Preferences"), this);
    helpPreferencesAct->setStatusTip(tr("Preferences"));
    connect(helpPreferencesAct, SIGNAL(triggered()), this, SLOT(help_Preferences()));
    
    helpCommonGUIelementsAct = new QAction(tr("Common GUI elements"), this);
    helpCommonGUIelementsAct->setStatusTip(tr("Common GUI elements"));
    connect(helpCommonGUIelementsAct, SIGNAL(triggered()), this, SLOT(help_CommonGUIelements()));
    
    helpBreadboardEditorAct = new QAction(tr("Breadboard Editor"), this);
    helpBreadboardEditorAct->setStatusTip(tr("Breadboard Editor"));
    connect(helpBreadboardEditorAct, SIGNAL(triggered()), this, SLOT(help_BreadboardEditor()));
    
    helpSchematicEditorAct = new QAction(tr("Schematic Editor"), this);
    helpSchematicEditorAct->setStatusTip(tr("Schematic Editor"));
    connect(helpSchematicEditorAct, SIGNAL(triggered()), this, SLOT(help_SchematicEditor()));
    
    helpPCBEditorAct = new QAction(tr("PCB Editor"), this);
    helpPCBEditorAct->setStatusTip(tr("PCB Editor"));
    connect(helpPCBEditorAct, SIGNAL(triggered()), this, SLOT(help_PCBEditor()));
    
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    
    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(editPrefsAct);
    optionsMenu->addAction(loadPrefsAct);
    optionsMenu->addAction(savePrefsAct);
    
    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpHelpAct);
    helpMenu->addAction(helpVersionAct);
    helpMenu->addAction(helpWarrantyAct);
    helpMenu->addAction(helpCopyingAct);
    helpMenu->addAction(helpShellAct);
    helpMenu->addAction(helpMainGUIAct);
    helpMenu->addAction(helpPreferencesAct);
    helpMenu->addAction(helpCommonGUIelementsAct);
    helpMenu->addAction(helpBreadboardEditorAct);
    helpMenu->addAction(helpSchematicEditorAct);
    helpMenu->addAction(helpPCBEditorAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings;
    _units = (SizeAndVP::UnitsType) settings.value("units", (int)SizeAndVP::mm).toInt();
    _width = settings.value("width", 25.4).toDouble();
    _height = settings.value("height", 25.4).toDouble();
    _viewport.setX(0);
    _viewport.setY(0);
    _viewport.setWidth(settings.value("vpwidth", 254.0).toDouble());
    _viewport.setHeight(settings.value("vpheight", 254.0).toDouble());
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("units", (int)_units);
    settings.setValue("width", _width);
    settings.setValue("height", _height);
    settings.setValue("vpwidth",_viewport.width());
    settings.setValue("vpheight",_viewport.height());
}

bool MainWindow::maybeSave()
{
    // check modified and save...
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    setCurrentFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
}



