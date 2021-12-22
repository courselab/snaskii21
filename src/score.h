/* score.c - Headers of TexTronSnake score system.

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

#ifndef __SCORE_TEXTRONSNAKE__
#define __SCORE_TEXTRONSNAKE__

#define MAX_NICKNAME 4
#define MAX_SCORES 9

#define SCORES_FILE "scores.bin"


void add_score(const char nickname[], int points);
void print_scores(WINDOW* mainWindow, int NROWS, int NCOLS);

#endif