dnl    configure.ac - Autoconf script 
dnl    
dnl    Copyright (c) 2021 - Monaco F. J. <monaco@usp.br>
dnl
dnl    This file is part of TexTronSnake. 
dnl
dnl    TexTronSnake is free software: you can redistribute it and/or modify
dnl    it under the terms of the GNU General Public License as published by
dnl    the Free Software Foundation, either version 3 of the License, or
dnl    (at your option) any later version.
dnl
dnl    TexTronSnake is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl    GNU General Public License for more details.
dnl
dnl    You should have received a copy of the GNU General Public License
dnl    along with .  If not, see <http://www.gnu.org/licenses/>.
dnl

AC_INIT([TexTronSnake], [0.0.2], [monaco@usp.br]) dnl Init Autoconf
AC_PREREQ([2.65])                              dnl Required autoconf version    
AC_CONFIG_AUX_DIR([build-aux])                 dnl Where to write auxiliary files                                                                              
AC_CONFIG_MACRO_DIR([m4])                      dnl m4 files here       
AC_CONFIG_HEADERS([config.h])                  dnl Generate config.h   

AM_INIT_AUTOMAKE([-Wall 1.14.1 subdir-objects])	dnl Init Automake options

AM_PROG_AR                      dnl Archiver for static lib                     
LT_INIT                         dnl Libtool support in 'configure'  

AC_PROG_CC 		        dnl Defines CC

dnl Alternative short name used for directories, binary prefixes etc.

AC_DEFINE(ALT_SHORT_NAME,["ttsnake"],[Define alternative short name.])
AC_SUBST([alt_short_name],[ttsnake])

AC_DEFINE(BIN_NAME,["ttsnake"],[Define the program binary name.])
AC_SUBST([bin_name],[ttsnake])


dnl Replace missing functions (see also GNUlib)

dnl AC_CONFIG_LIBOBJ_DIR([replace])
dnl AC_SEARCH_LIBS([qux], [xpto], [], AC_MSG_WARN([*** Symbol xpto not found in libxpto]),[])
dnl AC_SEARCH_LIBS([baz], [xpto], [], [AC_LIBOBJ(mybaz)],[])

dnl Check for needed libraries

AC_SEARCH_LIBS([clear], [ncurses], [], AC_MSG_ERROR([*** Can't find libncurses5]),[])

dnl Define variables to be used by Automake

AC_SUBST([CPP_FLAGS],"-ansi -D_POSIX_C_SOURCE=200809L -Wall -Wextra") 
AC_SUBST([C_FLAGS])
AC_SUBST([LD_FLAGS])


dnl Canocial check for POSIX threads.

AX_PTHREAD([AC_DEFINE(HAVE_PTHREAD,1,
            [Define to 1 if POSIX threads libraries and headers are found.]) ], 
	    AC_MSG_ERROR([POSIX threads support not detected.]))

dnl AC_DEFINE_UNQUOTED([DATADIR], [$datarootdir"],
dnl   [Define to the read-only architecture-independent
dnl    data directory.])

AC_CONFIG_FILES([Makefile
		src/Makefile
		scenes/Makefile])	dnl Output files
AC_OUTPUT			        dnl Do it

