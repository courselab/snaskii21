##   Automake.am - Automake script.
##
##   Copyright (c) 2021 - Monaco F. J. <monaco@usp.br>
##
##   This file is part of TexTronSnake
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

AM_CPPFLAGS = @CPP_FLAGS@ -DDATADIR=\"@datarootdir@\" -DBINDIR=\"@bindir@\"
AM_CFLAGS =   @C_FLAGS@ 
AM_LDFLAGS =  @LD_FLAGS@   

bin_PROGRAMS = ttsnake.bin 

ttsnake_bin_SOURCES = ttsnake.c utils.c  utils.h 

ttsnake_bin_CC = @PTHREAD_CC@
ttsnake_bin_CFLAGS = @PTHREAD_CFLAGS@ 
ttsnake_bin_LDADD = -lncurses -lm $(LIBOBJS) @PTHREAD_LIBS@ 

bin_SCRIPTS = ttsnake

ttsnake: ttsnake.sh
	cp $< $@

clean-local:
	rm -f ttsnake

install-exec-hook: 
	cd $(DESTDIR)/$(bindir) && mv ttsnake.bin ttsnake

uninstall-hook:
	rm -f $(DESTDIR)/$(bindir)/ttsnake

EXTRA_DIST = ttsnake.sh
