/* utils.c - Some handy tools.

   Copyright (c) 2021 - Monaco F. J. <monaco@usp.br> 

   This file is part of TexTronSnake

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../config.h"

/* Subtract the ‘struct timeval’ values X and Y, storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. 

   Attribution: this function is reproduced from GNU Libc Manual:
   https://www.gnu.org/software/libc/manual/html_node/Calculating-Elapsed-Time.html
*/

int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

/* Add the ‘struct timeval’ values X and Y, storing the result in RESULT. */

void
timeval_add (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Add the seconds and microseconds. */
  result->tv_sec = x->tv_sec + y->tv_sec;
  result->tv_usec = x->tv_usec + y->tv_usec;

  /* Check if microseconds complete a second or not. */
  if (result->tv_usec > 1000000) {
    result->tv_sec++;
    result->tv_usec -= 1000000;
  }
}

/* Shows help screen. Exit code is -1 if isError is set to true */

void show_help(char isError) {

    fprintf(isError? stderr : stdout, "\
Usage: " BIN_NAME " [options]\n\n\
  Options\n\n\
  -h, --help       Display this information message.\n\
  -d, --data       Selects a non-default data path\n\
  -v, --version    Outputs the program version\n") ;
    exit(isError?-1:0) ;
} 
