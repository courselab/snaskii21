/* utils.h - Some handy tools.

   Copyright (c) 2021 - Monaco F. J. <monaco@usp.br> 

   This file is part of Real-Time Sand Box - TexTronSnake

   TexTronSnake is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

/* Report a system error and exit. */

#define sysfatal(expression) \
  do { if ((expression)) {fprintf (stderr, "%s: %d: %s: %s\n",\
    __FILE__, __LINE__, __func__, strerror (errno)); \
      exit (EXIT_FAILURE);}} while (0)


/* Subtract the ‘struct timeval’ values X and Y, storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */

int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y);

/* Add the ‘struct timeval’ values X and Y, storing the result in RESULT. */

void
timeval_add (struct timeval *result, struct timeval *x, struct timeval *y);

/* Shows help screen. Exit code is -1 if isError is set to true */

void show_help(char isError);

#endif /* UTILS_H */
