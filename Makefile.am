##   Makefile.am - Main Automake script for autohello
##
##   Copyright (c) 2014 - Monaco F. J. <monaco@usp.br>
##
##   This file is part of TexTronSnake.
##
##   TexTronSnake is free software: you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, either version 3 of the License, or
##   (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public License
##   along with this program.  If not, see <http://www.gnu.org/licenses/>.

SUBDIRS = src scenes
ACLOCAL_AMFLAGS = -I m4

run: all
	./src/ttsnake --customDataDir ./scenes

.PHONY: run 
