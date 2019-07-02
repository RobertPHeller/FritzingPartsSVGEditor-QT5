##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Fri May 17 20:02:21 2019
#  Last Modified : <190702.1006>
#
#  Description	
#
#  Notes
#
#  History
#	
#  $Log$
#
##############################################################################
#
#    Copyright (C) 2019  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
##############################################################################

exists(/usr/bin/doxygen) {
   DISTFILES += \
   	Doxyfile \
   	src/scripts/buildqrc.sh \
   	
   DOXYGEN = "/usr/bin/doxygen"
   htmldoc.target = $(OBJECTS_DIR)html/index.html
   htmldoc.commands = $$DOXYGEN Doxyfile
   htmldoc.depends = src/main.cpp src/editors/feedit.cpp \
   		    src/editors/fbeedit.cpp \
   		    src/editors/fseedit.cpp \
   		    src/editors/fpeedit.cpp \
   		    src/extradoc/COPYING.h \
   		    src/extradoc/Help.h
   pdfdoc.target  = $(OBJECTS_DIR)latex/refman.pdf
   pdfdoc.depends = htmldoc
   pdfdoc.commands = make -C $(OBJECTS_DIR)latex refman.pdf
   htmldoc_resource.target = $(OBJECTS_DIR)qrc_htmldoc.o
   htmldoc_resource.commands = src/scripts/buildqrc.sh $(OBJECTS_DIR)html $(OBJECTS_DIR)htmldoc.qrc; \
   	/usr/lib64/qt5/bin/rcc -name htmldoc $(OBJECTS_DIR)htmldoc.qrc -o $(OBJECTS_DIR)qrc_htmldoc.cpp; \
   	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)qrc_htmldoc.o $(OBJECTS_DIR)qrc_htmldoc.cpp
   htmldoc_resource.depends = src/scripts/buildqrc.sh htmldoc
   QMAKE_EXTRA_TARGETS += htmldoc pdfdoc htmldoc_resource
   PRE_TARGETDEPS += $(OBJECTS_DIR)html/index.html $(OBJECTS_DIR)latex/refman.pdf
   OBJECTS += $(OBJECTS_DIR)qrc_htmldoc.o
} else {
   error("doxygen not found.  Please install doxygen and LaTeX: apt-get install doxygen latex")
}

