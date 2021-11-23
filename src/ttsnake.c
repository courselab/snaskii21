/* asciiplay.c - A very simple ascii movie player

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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h>
#include <config.h>
#include <getopt.h>
#include <math.h>

#include "utils.h"

/* Game defaults */

#define N_GAME_SCENES   4	/* Number of frames of the gamepay scnene. */
#define N_INTRO_SCENES  485	/* Number of frames of game intro. */

#define LOWER_PANEL_ROWS 6      /* Number of rows occuped by the lower panel.*/

#define BLANK ' '		/* Blank-screen character. */

#define SCENE_DIR_INTRO "intro" /* Path to the intro animation scenes. */
#define SCENE_DIR_GAME  "game"	/* Path to the game animation scene. */

#define SNAKE_TAIL	 '.'	 /* Character to draw the snake tail. */
#define SNAKE_BODY       'x'     /* Character to draw the snake body. */
#define SNAKE_HEAD	 '0'	 /* Character to draw the snake head. */
#define ENERGY_BLOCK     '+'	 /* Character to draw the energy block. */

#define MAX_ENERGY_BLOCKS_LIMIT 50	/* How many energy blocks we can have.*/
#define MAX_SNAKE_ENERGY (NCOLS+NROWS)  /* How much energy the snake can hold.*/

#define MIN_GAME_DELAY 10200	/* Empirically set. */
#define MAX_GAME_DELAY 2.5E5	/* Empirically set. */

/* Global variables.*/

struct timeval beginning,	/* Time when game started. */
  now,				/* Time now. */
  before,			/* Time in the last frame. */
  elapsed_last,			/* Elapsed time since last frame. */
  elapsed_total;		/* Elapsed time since game baginning. */

int NROWS;                      /* Number of rows of the game board */
int NCOLS;                      /* Number of cols of the game board */

int movie_delay;		/* How long between move scenes scenes. */
int game_delay;			/* How long between game scenes. */
int go_on; 			/* Whether to continue or to exit main loop.*/
int max_energy_blocks;          /* Max number of energy blocks to display at once */

int block_count; 		/*Number of energy blocks collected */

int paused = 1; 		/* Play/Pause indicator */

WINDOW *main_window;

/* SIGINT handler. The variable go_on controls the main loop. */

void quit ()
{
  go_on=0;
}

/* The snake data structrue. */

typedef enum {up, right, left, down} direction_t;

typedef struct snake_st snake_t;

typedef struct pair_st
{
  int x, y;
} pair_t;

struct snake_st
{
  pair_t head;			 /* The snake's head. */
  int length;			 /* The snake length (including head). */
  pair_t *positions;	         /* Position of each body part of the snake. */
  direction_t direction;         /* Movement direction. */
  int energy;                    /* Snake energy. */
};

snake_t snake;			/* The snake istance. */

/* Energy blocks. */

struct
{
  int x;			/* Coordinate x of the energy block. */
  int y;			/* Coordinate y of the energy block. */
} energy_block[MAX_ENERGY_BLOCKS_LIMIT]; /* Array of energy blocks. */

/* All chars of one single scene. */

typedef char scene_t[40][90]; /* Maximum values. TODO: allocate dyamically */


/* Read all the scenes in the 'dir' directory, save it in 'scene' and
   return the number of readed scenes. */

int readscenes (char *dir, char *data_dir, scene_t** scene, int nscenes)
{
  int i, j, k;
  FILE *file;
  char scenefile[1024], c, allocate = false;

  *scene = malloc(sizeof(**scene) * nscenes);
  allocate = true;
    
  /* Read nscenes. */

  for (k=0; k<nscenes; k++)
    {
      
      sprintf (scenefile, "%s/%s/scene-%07d.txt",data_dir, dir, k+1);
      
      file = fopen (scenefile, "r");
      if (!file)
	{
	  if (allocate)
	    free(*scene);
	  endwin();
	  sysfatal (!file);
	}

      /* Write top and bottom borders. */

      for (j=0; j<NCOLS; j++)
	{
	  (*scene)[k][0][j] = '-';
	  (*scene)[k][NROWS-1][j] = '-';
	}
      
      fseek(file, sizeof(char) * NCOLS, SEEK_CUR);
      while (((c = fgetc(file)) != '\n') && (c != EOF));

      /* Iterate through NROWS. */

      for (i=1; i<NROWS-1; i++)
  	{

	  /* Write left border.  */

	  (*scene)[k][i][0] = '|';
	  fseek(file, sizeof(char), SEEK_CUR);
	  
	  /* Read NCOLS columns from row i.*/
	  
	  for (j=1; j<NCOLS-1; j++)
	    {
	      
	      /* Actual ascii text file may be smaller than NROWS x NCOLS.
		 If we read something out of the 32-127 ascii range,
		 consider a blank instead.*/
	  
	      c = (char) fgetc (file);
	      (*scene)[k][i][j] = ((c>=' ') && (c<='~')) ? c : BLANK;
	    }

	  /* Write right border and correct stream position */
	  
	  (*scene)[k][i][NCOLS-1] = '|';
	  fseek(file, sizeof(char), SEEK_CUR);
	  
      
	  /* Discard the rest of the line (if longer than NCOLS). */
      
	  while (((c = fgetc(file)) != '\n') && (c != EOF));
      
  	}

      fclose (file);

    }

  return k;
}


/* Draw a the given scene on the screen. Currently, this iterates through the
   scene matrix outputig each caracter by means of indivudal puchar calls. One
   may want to try a different approach which favour performance. For instance,
   issuing a single 'write' call for each line. Would this yield any significant
   performance improvement? */

void draw (scene_t* scene, int number)
{
  int i, j;

  wmove(main_window, 0, 0);
  for (i=0; i<NROWS; i++)
    {
      for (j=0; j<NCOLS; j++)
	{
	  waddch(main_window, scene[number][i][j]);
	}
    }
  wrefresh(main_window);
}



/* Draw scene indexed by number, get some statics and repeat.
   If menu is true, draw the game controls.*/

void showscene (scene_t* scene, int number, int menu)
{
  double fps;
  int i;

  /* Draw the scene. */

  draw (scene, number);

  memcpy (&before, &now, sizeof (struct timeval));
  gettimeofday (&now, NULL);


  fps = 1 / (elapsed_last.tv_sec + (elapsed_last.tv_usec * 1E-6));
  

  if (menu)
    {
      wprintw (main_window, "Elapsed: %5ds, fps=%5.2f\n", /* CR-LF because of ncurses. */
	       (int) elapsed_total.tv_sec, fps);
      /*Add to the menu score and blocks collected */	  
      wprintw (main_window, "Score: %.d\n", block_count);
      wprintw (main_window, "Energy: %d\n", snake.energy); 
      for(i = 0; i < snake.energy; i++)
	{
	  if(i % ((MAX_SNAKE_ENERGY/100)*5) == 0){ /*prints one bar for every 5% energy left*/
	    wprintw(main_window, "|");
	  }	 
	}
      wprintw(main_window, "\n");
      wprintw (main_window, "Controls: q: quit | r: restart | WASD: move the snake | +/-: change game speed\n");
      wprintw (main_window, "          h: help & settings | p: pause game\n");
    }
}



/* Initialize resources and counters. */

void init_game (){
	int i;
	block_count = 0;
	snake.energy = 50;
	snake.direction = right;
	snake.length = 4;
	snake.head.x = 5;
	snake.head.y = 5;

	snake.positions = (pair_t*) malloc(sizeof(pair_t) * snake.length);
	for(i = 0; i < snake.length; i++){
		snake.positions[i].x = snake.head.x - i - 1;
		snake.positions[i].y = snake.head.y - i - 1;
	}
}


/* This function moves the snake */

void move_snake(){
	int i;
	for(i = snake.length - 1; i >= 0; i--){
		if(i){
			snake.positions[i].x = snake.positions[i - 1].x;
			snake.positions[i].y = snake.positions[i - 1].y;
		} else{
			snake.positions[i].x = snake.head.x;
			snake.positions[i].y = snake.head.y;
		}
	}

	switch(snake.direction){
		case up:
			snake.head.y -= 1;
			break;
		case left:
			snake.head.x -= 1;
			break;
		case down:
			snake.head.y += 1;
			break;
		case right:
			snake.head.x += 1;
			break;
	}
}


/* This function plays the game introduction animation. */

void playmovie (scene_t* scene, int nscenes)
{

  int k;
  struct timespec how_long;
  how_long.tv_sec = 0;

  for (k=0; (k < nscenes) && (go_on); k++)
    {
      wclear (main_window);			               /* Clear screen.    */
      wrefresh (main_window);			       /* Refresh screen.  */
      showscene (scene, k, 0);                 /* Show k-th scene .*/
      how_long.tv_nsec = (movie_delay) * 1e3;  /* Compute delay. */
      nanosleep (&how_long, NULL);	       /* Apply delay. */
    }
}

void draw_settings(scene_t *scene){
  char buffer[NCOLS];
  int i;

  /* clean buffer */
  for(i = 0; i < NCOLS; i++)
    buffer[i] = ' ';

  sprintf(buffer, "< %3d >     Maximum number of blocks to display at the same time.", max_energy_blocks);
  memcpy(&scene[2][22][12], buffer, strlen(buffer));
}

/* This function implements the gameplay */

void run(scene_t* scene){
	int i; 
	int tail = snake.length - 1;
	scene[0][snake.positions[tail].y][snake.positions[tail].x] = ' ';
	move_snake();
	scene[0][snake.head.y][snake.head.x] = SNAKE_HEAD;
	for(i = 0; i < tail; i++){
		scene[0][snake.positions[i].y][snake.positions[i].x] = SNAKE_BODY;
	}
	scene[0][snake.positions[tail].y][snake.positions[tail].x] = SNAKE_TAIL;
}

/* This function implements the gameplay loop. */

void playgame (scene_t* scene)
{

  struct timespec how_long;
  how_long.tv_sec = 0;

  /* User may change delay (game speedy) asynchronously. */

  while (go_on)
    {
      clear ();                               /* Clear screen. */
      refresh ();			      /* Refresh screen. */
      
      if (paused){
      	draw_settings(scene);
      	showscene (scene, 2, 1);
      } else{
	run(scene);
	showscene(scene, 0, 1);
      }
      how_long.tv_nsec = (game_delay) * 1e3;  /* Compute delay. */
      nanosleep (&how_long, NULL);



    }

}


/* Process user input.
   This function runs in a separate thread. */

void * userinput(){
	int c;
	while (1){
		c = getchar();
		switch(c){
			case 'p':
				paused = paused ^ 1;
				break;
			case 'w':
				if(snake.direction != down){
					snake.direction = up;
				}
				break;
			case 'a':
				if(snake.direction != right){
					snake.direction = left;
				}
				break;
			case 's':
				if(snake.direction != up){
					snake.direction = down;
				}
				break;
			case 'd':
				if(snake.direction != left){
					snake.direction = right;
				}
				break;
			case 'q':
				kill (0, SIGINT);
				break;
		}
	}
	
	return NULL;
}

/* The main function. */

int main(int argc, char **argv)
{
  int rs;
  int nscenes;
  pthread_t pthread;
  scene_t* intro_scene;
  scene_t* game_scene;
  struct sigaction act;
  const struct option stoptions[] = {
				     {"data", required_argument, 0, 'd'},
				     {"help", no_argument, 0, 'h'},
				     {"version", no_argument, 0, 'v'}};
  
  /* Defaults curr_data_screen to {datarootdir}/ttsnake */
  
  char *curr_data_dir = (char *)malloc((strlen(DATADIR "/" ALT_SHORT_NAME) + 1) * sizeof(char));
  strcpy(curr_data_dir, DATADIR "/" ALT_SHORT_NAME);
  
  /* Initializes program options struct */
  
  
  char currOpt;

  /* Handles options passed as arguments */
  
  while ((currOpt = (getopt_long(argc, argv, "d:h:v", stoptions, NULL))) != -1)
    {
      switch (currOpt)
	{
	case 'd':
	  /* Changes data_dir to one passed via argument */	  
	  curr_data_dir = (char *)realloc(curr_data_dir, (strlen(optarg) + 1) * sizeof(char));
	  strcpy(curr_data_dir, optarg);
	  break;
	case 'h':
	  free(curr_data_dir);
	  show_help(false);
	  break;
	  
	case 'v':
	  free(curr_data_dir);
	  printf (PACKAGE_STRING "\n");
	  exit (EXIT_SUCCESS);
	  break;
	  
	default:
	  free(curr_data_dir);
	  show_help(true);
	}
      }
  
  
  
    game_scene = (scene_t *) malloc(sizeof(*game_scene) * N_GAME_SCENES);
    if(!game_scene){
      endwin();
      sysfatal(!game_scene);
    }

    /* Handle SIGNINT (loop control flag). */

    sigaction(SIGINT, NULL, &act);
    act.sa_handler = quit;
    sigaction(SIGINT, &act, NULL);

    /* Ncurses initialization. */

    initscr();
    noecho();
    curs_set(FALSE);
    cbreak();

    /* Get terminal size */
    int maxWidth, maxHeight;
    getmaxyx(stdscr, maxHeight, maxWidth);

    /* Set game board size */
    NROWS = (int) fmin(maxHeight - LOWER_PANEL_ROWS, 40);
    NCOLS = (int) fmin(maxWidth, 90);

    if(NROWS < 20 || NCOLS < 80){
      endwin();
      fprintf(stderr, "You need a terminal with at least 20 rows and 80 columns to play.\n");
      return EXIT_FAILURE;
    }

    main_window = newwin(NROWS + LOWER_PANEL_ROWS, NCOLS,
			 (maxHeight - NROWS - LOWER_PANEL_ROWS) / 2, (maxWidth - NCOLS) / 2);
    wrefresh(main_window);

    /* Default values. */

    movie_delay = 2.5E4;	  /* Movie frame duration in usec (40usec) */
    game_delay  = 9E4;	          /* Game frame duration in usec (4usec) */
    max_energy_blocks = 3;


    /* Handle game controls in a different thread. */

    rs = pthread_create (&pthread, NULL, userinput, NULL);
    sysfatal (rs);

    /* Play intro. */

    nscenes = readscenes (SCENE_DIR_INTRO, curr_data_dir, &intro_scene, N_INTRO_SCENES);

    go_on=1;			/* User may skip intro (q). */

    playmovie (intro_scene, nscenes);

    /* Play game. */

    readscenes (SCENE_DIR_GAME, curr_data_dir, &game_scene, N_GAME_SCENES);

    go_on=1;

    gettimeofday (&beginning, NULL);

    init_game ();
    playgame (game_scene);
    
    endwin();
    free(intro_scene);
    free(game_scene);
    free(curr_data_dir);
    
    return EXIT_SUCCESS;
  }
