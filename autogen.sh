#!/bin/sh
#
#  autogen.sh - Autootols boostrap for developers
#    
#    Copyright (c) 2021 - Monaco F. J. <monaco@usp.br>
#
#    This file is part of TexTronSnake. 
#
#    TexTronSnake is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Phoo is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with .  If not, see <http://www.gnu.org/licenses/>.
#

RECOMMENDED_FILES="NEWS README AUTHORS ChangeLog" 

AUTORECONF=$(which autoreconf)
if test -z "$AUTORECONF"; then
  echo "Program autoreconf not found"
  exit 1
fi

echo -n "Bootstrapping project..."

for i in $RECOMMENDED_FILES; do
    if test ! -f "$i" ; then 
	touch "$i"
    fi
done

$AUTORECONF --install

echo "Done"
