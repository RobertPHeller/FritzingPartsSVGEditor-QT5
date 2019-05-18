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
#  Last Modified : <190517.2124>
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
   DOXYGEN = "/usr/bin/doxygen"
   htmldoc.target = release/html/index.html
   htmldoc.commands = $$DOXYGEN Doxyfile
   pdfdoc.target  = release/latex/refman.pdf
   pdfdoc.depends = htmldoc
   pdfdoc.commands = make -C release/latex refman.pdf
   QMAKE_EXTRA_TARGETS += htmldoc pdfdoc
   PRE_TARGETDEPS += release/html/index.html release/latex/refman.pdf
} else {
   message("Doxygen not found.  Documentation will not be built.")
}
