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
#include "score.h"

/* Game defaults. */

/* assert if a system call was properly made and */
#define ASSERT_SYSTEM_CALL(x) if (!(x)) \
	fprintf(stderr, "System does not recognize call \""#x"\"\n");

#define N_GAME_SCENES   4	      /* Number of frames of the gamepay scnene. */
#define N_INTRO_SCENES  485	    /* Number of frames of game intro.         */

#define LOWER_PANEL_ROWS 6      /* Number of rows occupied by the lower panel.*/

#define BLANK ' '		                   /* Blank-screen character. */

#define SCENE_DIR_INTRO "intro"        /* Path to the intro animation scenes.*/
#define SCENE_DIR_GAME  "game"	       /* Path to the game animation scene.  */


#define SNAKE_TAIL	     '.'           /* Character to draw the snake tail.  */
#define SNAKE_BODY       'x'           /* Character to draw the snake body.  */
#define SNAKE_HEAD	     '0'	         /* Character to draw the snake head.  */
#define ENERGY_BLOCK     '+'	         /* Character to draw the energy block.*/
#define FRUIT_BLOCK      '$'	         /* Character to draw the fruit block. */

#define HORIZONTAL_MOVE  2   /* Number of positions on matrix to move horizontally. */
#define VERTICAL_MOVE    1   /* Number of positions on matrix to move vertically.   */

#define MAX_ENERGY_BLOCKS_LIMIT 50	   /* How many energy blocks we can have.*/
#define MAX_SNAKE_ENERGY (NCOLS+NROWS) /* How much energy the snake can hold.*/

#define MIN_GAME_DELAY 10200	         /* Empirically set. */
#define MAX_GAME_DELAY 2.5E5	         /* Empirically set. */
#define DEFAULT_GAME_DELAY 9E4
#define DELTA_DELAY 5000
#define ENERGY_MINIMAL 100


/* Global variables. */

struct timeval beginning,       /* Time when game started. */
	now,		                /* Time now. */
	before,			            /* Time in the last frame. */
	paused_time,                /* Time slice of the game when is paused */
	elapsed_last,			    /* Elapsed time since last frame. */
	elapsed_total;		        /* Elapsed time since game baginning. */

int NROWS;                /* Number of rows of the game board. */
int NCOLS;                /* Number of cols of the game board. */

int movie_delay;	      /* How long between move scenes. */
int game_delay;		      /* How long between game scenes. */
int go_on; 			      /* Whether to continue or to exit main loop.*/
int go_on_cutscene = 0;   /* Whether to continue to display cutscene or skip it.*/
int max_energy_blocks;    /* Max number of energy blocks to display at once. */

const int num_option_in_menu = 2; /*Number of options in setting menu*/
int term_has_colored_mode;
int colored_mode = 0;	  /*Whether or not to display the game in colored mode*/
int selected_option = 0;


int block_count; 		      /* Number of energy blocks collected. */

int paused = 1; 		      /* Play/Pause indicator. */
int game_end = 0;		      /* End of the game indicator.  */
int restarted = 1; 		    /* Restart indicator. */
int entered_score = 0;    /* Indicates if user registered his score. */

char nickname[MAX_NICKNAME + 1]; /*Nickname used in the score system*/
int actual_pos_nickname = 0;

WINDOW *main_window;

int main_process_pid; /*Keeps track of main process PID*/

/* SIGINT handler. The variable go_on controls the main loop. */
void quit () {
	go_on = 0;
}

/* SIGUSR1 handler. The variable go_on_cutscene controls the cutscene loop. */
void stop_cutscene () {
	go_on_cutscene = 0;
}

void enter_colored_mode(){
	if(term_has_colored_mode){
		colored_mode = 1;
	}

}

void exit_colored_mode(){
	colored_mode = 0;
	
}

/* Scene types enumerated from 0 to 3. */
typedef enum {
	RUNNING = 0, GAME_OVER, RESTARTED, PAUSED
} scene_type_t;


/* The snake data structrue. */
typedef enum {up, right, left, down} direction_t;

typedef enum {
	collision_none,
	collision_self,
	collision_wall,
	collision_energy,
	collision_fruit,
} collision_type_t;

typedef struct snake_st snake_t;

typedef struct pair_st {
	int x, y;
} pair_t;

struct snake_st {
	pair_t head;		       /* The snake's head. */
	int length;			       /* The snake length (including head). */
	pair_t* positions;	       /* Position of each body part of the snake. */
	direction_t direction;     /* Movement direction. */
	int energy;                /* Snake energy. */
};

snake_t snake;			         /* The snake istance. */


/* Energy blocks. */
struct {
	int x;			/* Coordinate x of the energy block. */
	int y;			/* Coordinate y of the energy block. */
} energy_block[MAX_ENERGY_BLOCKS_LIMIT];   /* Array of energy blocks. */

/* Fruit block. */
struct {
  int x;			/* Coordinate x of the fruit block. */
  int y;			/* Coordinate y of the fruit block. */
} fruit_block;

/* All chars of one single scene. */
typedef char scene_t[40][90]; /* Maximum values. TODO: allocate dyamically. */


/* Read all the scenes in the 'dir' directory, save it in 'scene_array' and
   return the number of read scenes. */
int read_scenes (char *dir, char *data_dir, scene_t **scene_array_ptr, int nscenes) {
	int i, j, k;
	FILE *file;
	char scenefile[1024], c;
	scene_t *scene_array;

	/* Same scene is used sometimes. In this case, *scene != NULL and there's no need to malloc again */
	if (*scene_array_ptr == NULL) {
		*scene_array_ptr = malloc(sizeof(**scene_array_ptr) * nscenes);
	}
	
	scene_array = *scene_array_ptr;

	/* Read nscenes. */
	for (k = 0; k < nscenes; k++) {
		sprintf(scenefile, "%s/%s/scene-%07d.txt",data_dir, dir, k+1);

		file = fopen (scenefile, "r");
		if (!file) {
			free(*scene_array_ptr);
			*scene_array_ptr = NULL;
			endwin();
			sysfatal(!file);
		}

		/* Write top and bottom borders. */
		for (j = 0; j < NCOLS; j++) {
			scene_array[k][0][j] = '-';
			scene_array[k][NROWS-1][j] = '-';
		}

		fseek(file, sizeof(char) * NCOLS, SEEK_CUR);
		while (((c = fgetc(file)) != '\n') && (c != EOF)) {}

		/* Iterate through NROWS. */
		for (i = 1; i < NROWS-1; i++)
		{
			/* Write left border.  */
			scene_array[k][i][0] = '|';
			fseek(file, sizeof(char), SEEK_CUR);

			/* Read NCOLS columns from row i. */
			for (j = 1; j < NCOLS-1; j++) {

				/* Actual ascii text file may be smaller than NROWS x NCOLS.
				If we read something out of the 32-127 ascii range,
				consider a blank instead. */
				c = (char) fgetc(file);
				scene_array[k][i][j] = ((c >= ' ') && (c <= '~')) ? c : BLANK;
			}

			/* Write right border and correct stream position. */
			scene_array[k][i][NCOLS-1] = '|';
			fseek(file, sizeof(char), SEEK_CUR);

			/* Discard the rest of the line (if longer than NCOLS). */
			while (((c = fgetc(file)) != '\n') && (c != EOF));
		}

		fclose(file);
	}

	return k;
}


/* Draw a the given scene on the screen. Currently, this iterates through the
   scene matrix outputig each caracter by means of individual puchar calls. One
   may want to try a different approach which favour performance. For instance,
   issuing a single 'write' call for each line. Would this yield any significant
   performance improvement? */
void draw (scene_t* scene_array, int number) {
	int i, j;
	

	wmove(main_window, 0, 0);
	for (i = 0; i < NROWS; i++) {
		for (j = 0; j < NCOLS; j++) {
			if(number == RUNNING && colored_mode == 1){
				if(scene_array[number][i][j] == SNAKE_TAIL || scene_array[number][i][j] == SNAKE_BODY || scene_array[number][i][j] == SNAKE_HEAD ){
					wattron(main_window,COLOR_PAIR(1));
					waddch(main_window, scene_array[number][i][j]);
					wattroff(main_window,COLOR_PAIR(1));
				}else if(scene_array[number][i][j] == ENERGY_BLOCK || scene_array[number][i][j] == FRUIT_BLOCK){
					wattron(main_window,COLOR_PAIR(2));
					waddch(main_window, scene_array[number][i][j]);
					wattroff(main_window,COLOR_PAIR(2));
				}else{
					waddch(main_window, scene_array[number][i][j]);
				}
			
			}else{
				waddch(main_window, scene_array[number][i][j]);
			}
		  
		}
	}

	if (number == GAME_OVER) {
	/* If displayed scene is the game over scene.
	   We must show the final score. */
	mvwprintw(main_window, NROWS*3/4, NCOLS/2-7, "Score: %d", block_count);

	/*Instructions to enter nickname*/
		if (!entered_score) {
			mvwprintw(main_window, NROWS*3/4 + 1,NCOLS/5 - 2,
					  "                                             ");
			mvwprintw(main_window, NROWS*3/4 + 3, NCOLS/5 - 2,
					  "Type a nick of at most 4 letters to save your score.");

			mvwprintw(main_window, NROWS*3/4  +  4, NCOLS/5 + 2,
					  "In case you do not want to save it, type '#'.");

			mvwprintw(main_window, NROWS*3/4 + 5, NCOLS/5 + 3,
					  "If you want to erase a character, type '&'.");
			mvwprintw(main_window, NROWS*3/4 + 7, NCOLS/2 - 7, "%s", nickname);
		}
	}else if(number == RESTARTED){
		
	}

	wrefresh(main_window);
}


/* Draw scene indexed by number, get some statics and repeat.
   If menu is true, draw the game controls. */
void showscene (scene_t* scene, int scene_type, int menu) {
	double fps, t;
	int i;

	/* Draw the scene. */
	draw (scene, scene_type);

	/* Updating current time and passing the last time
	 (from last frame) to the 'before' variable. */
	memcpy (&before, &now, sizeof (struct timeval));
	gettimeofday (&now, NULL);

	/* Calculate the FPS. */
	t = ((double)elapsed_total.tv_sec*1e6 + (double)elapsed_total.tv_usec)
			 - ((double)elapsed_last.tv_sec*1e6 + (double)elapsed_last.tv_usec);
	fps = 1e6/t;

	switch (scene_type) {
		case RUNNING:
			/* Calculating elapsed time to display while the game is running. */
			timeval_subtract(&elapsed_last, &before, &beginning);
			timeval_subtract(&elapsed_total, &now, &beginning);
			break;
			
		case PAUSED:
			/* Calculating a short time while the game is paused. */
			timeval_subtract(&paused_time, &now, &before);

			/* Update the beginning adding the paused_time. */
			timeval_add(&beginning, &paused_time, &beginning);
			break;

		case RESTARTED:
			gettimeofday (&beginning, NULL);
			elapsed_last = (struct timeval){(__time_t) 0, (__suseconds_t) 0};
			elapsed_total = (struct timeval){(__time_t) 0, (__suseconds_t) 0};
			break;

		case GAME_OVER:
			elapsed_last = (struct timeval){(__time_t) 0, (__suseconds_t) 0};
			elapsed_total = (struct timeval){(__time_t) 0, (__suseconds_t) 0};
			break;

		default:
			break;
	}
  
	if (menu) {
		/* CR-LF because of ncurses. */
		wprintw(main_window, "Elapsed: %5ds, fps=%5.2f\n", (int) elapsed_total.tv_sec, fps);

		/* Add to the menu score and blocks collected. */
		wprintw(main_window, "Score: %d\n", block_count);
		wprintw(main_window, "Energy: %d	Game Speed: %.2f\n", snake.energy,(DEFAULT_GAME_DELAY/game_delay));
		
		for (i = 0; i < snake.energy; i++) {
			if ((MAX_SNAKE_ENERGY / 100) != 0 && i % ((MAX_SNAKE_ENERGY / 100) * 5) == 0) {
				/* Prints one bar for every 5% energy left. */
				wprintw(main_window, "|");
			}
		}

		wprintw(main_window, "\n");
		wprintw(main_window, "\
		Controls: q: quit | r: restart | WASD/HJKL/ARROWS: move the snake\
		| +/-: change game speed\n");
		wprintw(main_window, "          h: help & settings | p: pause game\n");

	}
	else if (game_end) {
		print_scores(main_window, NROWS, NCOLS);
	}
}


/* Generates fruit_block coordinates randomly. */
void generate_fruit_block () {
	fruit_block.x = (rand() % (NCOLS - 4)) + HORIZONTAL_MOVE;
	fruit_block.y = (rand() % (NROWS - 2)) + VERTICAL_MOVE;

	/* Verifies if the fruit is in an even position, because the snake moves 2 positions horizontally */
	if ((fruit_block.x)%2 == 0) {
		fruit_block.x -= 1;
	}
}


/* Verifies if the block positions conflicts with the snake coordinates. */
int fruit_block_conflict () {
  int i;

  if (fruit_block.x == snake.head.x && fruit_block.y == snake.head.y) {
		return 1;
  }

  for (i = 0; i < snake.length - 1; i++) {
	if (fruit_block.x == snake.positions[i].x && 
		fruit_block.y == snake.positions[i].y) {
		return 1; 
	}
  }

  return 0;
}


/* Spawns a fruit_block on the map. */
void spawn_fruit_block () {
	generate_fruit_block();
	
	while (fruit_block_conflict()) {
		generate_fruit_block(); 
	}
}


/* Generates energy_block[0] coordinates randomly. */
void generate_energy_block () {
	energy_block[0].x = (rand() % (NCOLS - 4)) + HORIZONTAL_MOVE;
	energy_block[0].y = (rand() % (NROWS - 2)) + VERTICAL_MOVE;

	/* Verifies if the energy is in a pair position, 'cause the snake moves 2 pos horizontally */
	if ((energy_block[0].x)%2 == 0) {
		energy_block[0].x -= 1;
	}
}


/* Verifies if the block positions conflicts with the snake coordinates. */
int energy_block_conflict () {
	int i;

	if (energy_block[0].x == snake.head.x && energy_block[0].y == snake.head.y) {
		return 1;
	}

	for (i = 0; i < snake.length - 1; i++) {
		if (energy_block[0].x == snake.positions[i].x &&
			energy_block[0].y == snake.positions[i].y) {
			return 1;
		}
	}

	return 0;
}


/* Spawns an energy_block on the map. */
void spawn_energy_block () {
	generate_energy_block();

	while (energy_block_conflict()) {
		generate_energy_block();
	}
}


/* Initialize resources and counters. */
void init_game () {
	ASSERT_SYSTEM_CALL(system("curl https://raw.githubusercontent.com/courselab/snaskii21/develop/sound/maintheme.mp3 | mpg123 --no-visual --no-control --quiet - &"));

    /* fflush to avoid influence of past key pressed after game is restarted */
    fflush(stdin);

	int i;
	block_count = 0;
	snake.energy = ENERGY_MINIMAL;
	snake.direction = right;
	snake.length = 5;
	snake.head.x = 11;
	snake.head.y = 11;

	if (snake.positions != NULL){
		free(snake.positions);
	}

	snake.positions = (pair_t*) malloc(sizeof(pair_t) * snake.length);

	for (i = 0; i < snake.length; i++) {
		snake.positions[i].x = snake.head.x - i - HORIZONTAL_MOVE;
		snake.positions[i].y = snake.head.y - i - VERTICAL_MOVE;
	}

	/* Spawns of the first energy block. */
	spawn_energy_block();

	/* Spawns of the first fruit block. */
	spawn_fruit_block();
}


/* Grows the snake - increases the snake length by one. */
void grow_snake () {
	snake.length++;
	snake.positions = (pair_t *) realloc(snake.positions,
									   sizeof(pair_t) * snake.length);
	snake.positions[snake.length - 1].x = snake.positions[snake.length - 2].x;
	snake.positions[snake.length - 1].y = snake.positions[snake.length - 2].y;
}


/* Checks if the snake has hit itself, a wall, energy block or fruit block. */
collision_type_t check_collision() {
	int i;

	if (snake.head.x <= 0 || snake.head.x >= NCOLS - 1 || snake.head.y <= 0 || snake.head.y >= NROWS - 1) {
		return collision_wall;
	} else if (snake.head.x == energy_block[0].x && snake.head.y == energy_block[0].y) {
		return collision_energy;
		
	} else if (snake.head.x == fruit_block.x && snake.head.y == fruit_block.y) {
		return collision_fruit;
	}

	for (i = 0; i < snake.length - 1; i++) {
		if (snake.head.x == snake.positions[i].x &&
		snake.head.y == snake.positions[i].y) {
			return collision_self;
		}
	}

	return collision_none;
}


/* This function moves the snake. */
void move_snake() {
	int i = snake.length;
	while (i--) {
		if (i) {
			snake.positions[i].x = snake.positions[i - 1].x;
			snake.positions[i].y = snake.positions[i - 1].y;

		} else {
			snake.positions[i].x = snake.head.x;
			snake.positions[i].y = snake.head.y;
		}
	}

	switch (snake.direction) {
		case up:
			snake.head.y -= VERTICAL_MOVE;
			break;
		case left:
			snake.head.x -= HORIZONTAL_MOVE;
			break;
		case down:
			snake.head.y += VERTICAL_MOVE;
			break;
		case right:
			snake.head.x += HORIZONTAL_MOVE;
			break;
		default:
			break;
	}
}


/* This function plays the game introduction animation. */
void playmovie (scene_t* scene_array, int nscenes) {
	int k;
	struct timespec how_long;
	how_long.tv_sec = 0;

	/*Skip String*/
	char* skipString = "Press the 'space' key to skip the animation";
	double halfLen = strlen(skipString)/2.0;

	for (k = 0; (k < nscenes) && go_on_cutscene; k++) {
		wclear(main_window);			               /* Clear screen.    */
		wrefresh(main_window);			               /* Refresh screen.  */
		showscene(scene_array, k, 0);                  /* Show k-th scene .*/
		mvwprintw(main_window, NROWS, (int)(NCOLS/2.0-halfLen), skipString); /*Print skip String*/
		wrefresh(main_window);                          /* Refresh screen.  */
		how_long.tv_nsec = movie_delay*1e3;            /* Compute delay.   */
		nanosleep(&how_long, NULL);	       /* Apply delay.     */
	}
}


void draw_settings(scene_t *scene) {
	char buffer[NCOLS];
	int i;

	/* Clear buffer. */
	for (i = 0; i < NCOLS; i++) {
		buffer[i] = ' ';
	}
	char selected[5] = "-->  ";
	for(i=0;i<num_option_in_menu;i++){
		if(i==selected_option){
			memcpy(&scene[2][22+i][7], selected, strlen(selected));
		}else{
			memcpy(&scene[2][22+i][7], "     ",5);
		}
		
	}
	

	sprintf(buffer,"        < %3d >     Maximum number of blocks to display at the same time.", max_energy_blocks);
	memcpy(&scene[2][22][12], buffer, strlen(buffer));
	if(term_has_colored_mode){
		sprintf(buffer, "        < %3d >     Colored Mode.", colored_mode);
	}else{
		sprintf(buffer, "        < %3d >     Colored Mode (not available on this terminal).", colored_mode);
	}
	
	memcpy(&scene[2][23][12], buffer, strlen(buffer));
}


void update_snake_in_scene(scene_t scene, pair_t old_tail_pos) {
	int i;
	int tail = snake.length - 1;

	/* Erase old tail. */
	scene[old_tail_pos.y][old_tail_pos.x] = ' ';

	/* Draw head. */
	scene[snake.head.y][snake.head.x] = SNAKE_HEAD;

	/* Draw body. */
	for (i = 0; i < tail; i++) {
		scene[snake.positions[i].y][snake.positions[i].x] = SNAKE_BODY;
	}

	scene[snake.positions[tail].y][snake.positions[tail].x] = SNAKE_TAIL;
}


void update_blocks_in_scene(scene_t scene) {
	scene[energy_block[0].y][energy_block[0].x] = ENERGY_BLOCK;
	scene[fruit_block.y][fruit_block.x] = FRUIT_BLOCK;
}


void drain_energy() {
	/* Consume energy. */
	if (snake.energy > 0) {
		snake.energy -= 1;
	} else { /* Snake is dead. */
		game_end = 1;
		paused = 1;
	}
}


/* This function implements the gameplay. */
void run(scene_t game_scene) {
	collision_type_t collision;
	pair_t old_tail_pos = snake.positions[snake.length - 1];

	/* Move snake and check for collisions. */
	move_snake();
	drain_energy();
	collision = check_collision();

	/* Apply collision effects. */
	switch (collision) {
		case collision_none:
			break;
		case collision_self:
			game_end = 1;
			paused = 1;
			break;
		case collision_wall:
			game_end = 1;
			paused = 1;
			break;
		case collision_energy:
			block_count++;
			grow_snake();
			spawn_energy_block();
			snake.energy = ENERGY_MINIMAL;
			break;
		case collision_fruit:
			block_count++;
			spawn_fruit_block();
			break;
	}

	update_snake_in_scene(game_scene, old_tail_pos);
	update_blocks_in_scene(game_scene);
	
	/* If the game is over, set delay to default. */
	if (game_end) {
		game_delay = DEFAULT_GAME_DELAY;
	}
}


/* This function implements the gameplay loop. */
void playgame (scene_t* scene_array, char* curr_data_dir) {
	struct timespec how_long;
	how_long.tv_sec = 0;

	/* User may change delay (game speedy) asynchronously. */
	while (go_on) {
		clear();             /* Clear screen.   */
		refresh();	         /* Refresh screen. */

		if (restarted) {
			draw_settings(scene_array);
			showscene(scene_array, RESTARTED, 1);

		} else if (game_end) {
			showscene(scene_array, GAME_OVER, 0);
			read_scenes(SCENE_DIR_GAME, curr_data_dir, &scene_array, N_GAME_SCENES);

		} else if (paused) {
			showscene(scene_array, PAUSED, 1);

		} else {
			run(scene_array[RUNNING]);
			showscene(scene_array, RUNNING, 1);
		}

		how_long.tv_nsec = game_delay * 1e3;  /* Compute delay. */
		nanosleep(&how_long, NULL);
	}
}


/* Process user input. This function runs in a separate thread. */
void *userinput () {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	int c, last_c = 0;
	struct timespec how_long;

	how_long.tv_sec = 0;
	how_long.tv_nsec = game_delay * 1e3;  /*Delay to stop fast inputs bug*/

	while (1) {
		c = getch();

		/* If the game is over and you havent entered
		   your nickname, the key is used here. */
		if (game_end && !entered_score) {

			/* Dont insert score. */
			if (c == '#') {
				entered_score = 1;
				actual_pos_nickname = 0;

			/* Insert char to nickname. */
			} else if (c != '\n' && c!= '\t' && c!=' ' && c!= '\r') {

				/*Delete last char*/
				if (c == '&' && actual_pos_nickname > 0) {
					actual_pos_nickname--;
					nickname[actual_pos_nickname] = '\0';
				}

				/* Insert char. */
				else if (actual_pos_nickname < MAX_NICKNAME) {
					nickname[actual_pos_nickname] = c;
					actual_pos_nickname++;
					nickname[actual_pos_nickname] = '\0';
				}

			/* Enter score to score system. */
			} else {
				if (actual_pos_nickname > 0) {
					add_score(nickname, block_count);
				}

				actual_pos_nickname = 0;
				entered_score = 1;
			}

		} else if (go_on_cutscene) { /* If its playing the cutscene */
			if (c == ' '){
				kill(main_process_pid, SIGUSR1);
			}
		} else {
			if (c == last_c && c != 'p') {
				continue;
			}

			switch (c) {
				case 'p':
					if (!game_end) {
						paused = paused ^ 1;
						restarted = 0;
					}
					break;

				case KEY_UP:
				case 'w':
				case 'k':
				case 'e':
					if (paused) { /* Avoid moving the snake after unpause unintendedly. */
						if(restarted){
							selected_option = (selected_option - 1 + num_option_in_menu) % num_option_in_menu;
						}
						break;
					}

					if (snake.direction != down) {
						snake.direction = up;
						last_c = c;
					}
					break;

				case KEY_LEFT:
				case 'h':
				case 'a':
					if (paused) { /* Avoid moving the snake after unpause unintendedly. */
						if(restarted && selected_option == 1 && colored_mode == 1){
							exit_colored_mode();
						}
						break;
					}

					if (snake.direction != right) {
						snake.direction = left;
						last_c = c;
					}
					break;

				case KEY_DOWN:
				case 'j':
				case 's':
					if (paused) { /* Avoid moving the snake after unpause unintendedly. */
						if(restarted){
							selected_option = (selected_option + 1) % num_option_in_menu;
						}
						break;
					}

					if (snake.direction != up) {
						snake.direction = down;
						last_c = c;
					}
					break;

				case KEY_RIGHT:
				case 'l':
				case 'd':
					if (paused) { /* Avoid moving the snake after unpause unintendedly. */
						if(restarted && selected_option == 1 && colored_mode == 0){
							enter_colored_mode();
						}
						break;
					}

					if (snake.direction != left) {
						snake.direction = right;
						last_c = c;
					}
					break;

				case 'q':
					if (game_end || restarted) {
						/* Both flags equals 1 means game is over for real and we should (try to) quit gracefully */
						quit();
						return NULL; /* Ending thread */
					}
					break;

				case 'r':
					if (game_end) {
						ASSERT_SYSTEM_CALL(system("killall mpg123"));
						init_game();
						restarted = 1;
						game_end = 0;
					}
					break;

				case '+':
					if (!game_end && !restarted) {
						if (game_delay - DELTA_DELAY >= MIN_GAME_DELAY) {
						  game_delay -= DELTA_DELAY;

						} else {
						  game_delay = MIN_GAME_DELAY;
						}
					}
					break;

				case '-':
					if (!game_end && !restarted) {
						if (game_delay + DELTA_DELAY <= MAX_GAME_DELAY) {
							game_delay += DELTA_DELAY;

						} else {
						  game_delay = MAX_GAME_DELAY;
						}
					}
					 break;

				default:
					break;
			}
		}
		nanosleep (&how_long, NULL);
	}
}


/* The main function. */
int main (int argc, char **argv) {
	int rs, nscenes;
	pthread_t pthread;
	scene_t *intro_scene = NULL, *game_scene = NULL;
	struct sigaction act;
	const struct option stoptions[] = {
					 {"data", required_argument, 0, 'd'},
					 {"help", no_argument, 0, 'h'},
					 {"version", no_argument, 0, 'v'}};

	/* Defaults curr_data_screen to {datarootdir}/ttsnake */
	char *curr_data_dir = (char *)malloc((strlen(DATADIR "/" ALT_SHORT_NAME) + 1)
									   * sizeof(char));
	strcpy(curr_data_dir, DATADIR "/" ALT_SHORT_NAME);

	/* Initializes program options struct. */
	char curr_opt;

	/* Handle options passed as arguments. */
	while ((curr_opt = (getopt_long(argc, argv, "d:h:v", stoptions, NULL))) != -1) {

		switch (curr_opt) {
			/* Changes data_dir to one passed via argument. */
			case 'd':
				curr_data_dir = (char *) realloc(curr_data_dir, (strlen(optarg) + 1) * sizeof(char));
				strcpy(curr_data_dir, optarg);
				break;

			case 'h':
				show_help(false, curr_data_dir);
				break;

			case 'v':
				free(curr_data_dir);
				printf (PACKAGE_STRING "\n");
				exit (EXIT_SUCCESS);

			default:
				show_help(true, curr_data_dir);
		}
	}

	/* Handle SIGNINT (loop control flag). */
	sigaction(SIGINT, NULL, &act);
	act.sa_handler = &quit;
	sigaction(SIGINT, &act, NULL);

	/* Handle SIGUSR1(animation control flag). */
	sigaction(SIGUSR1, NULL, &act);
	act.sa_handler = &stop_cutscene;
	sigaction(SIGUSR1, &act, NULL);

	/* Ncurses initialization. */
	initscr();
	noecho();
	curs_set(FALSE);
	cbreak();

	/*Prepare colored mode*/
	if(has_colors() == FALSE){
		term_has_colored_mode = 0;
	}else{
		term_has_colored_mode = 1;
		start_color();
		use_default_colors();
		init_color(COLOR_BLACK, 0, 0, 0);
		init_color(COLOR_WHITE, 1000,1000, 1000);
		init_color(COLOR_GREEN, 0, 1000, 0);
		init_color(COLOR_RED, 1000,0, 0);
		init_pair(1,COLOR_GREEN,-1);
		init_pair(2,COLOR_RED,-1);
	}

	/* Get terminal size. */
	int max_width, max_height;
	getmaxyx(stdscr, max_height, max_width);

	/* Set game board size. */
	NROWS = (int) fmin(max_height - LOWER_PANEL_ROWS, 40);
	NCOLS = (int) fmin(max_width, 90);

	if (NROWS < 20 || NCOLS < 80) {
		endwin();
		fprintf(stderr, "\
		You need a terminal with at least 20 rows and 80 columns to play.\n");
		return EXIT_FAILURE;
	}

	main_window = newwin(NROWS + LOWER_PANEL_ROWS, NCOLS,
							(max_height - NROWS - LOWER_PANEL_ROWS)/2,
							(max_width - NCOLS)/2);
	wrefresh(main_window);
	



	

	/* Default values. */
	movie_delay = 2.5E4;	            /* Movie frame duration in usec (40usec) */
	game_delay  = DEFAULT_GAME_DELAY;	/* Game frame duration in usec (4usec) */
	max_energy_blocks = 3;


	main_process_pid = getpid();
	/* Handle game controls in a different thread. */
	rs = pthread_create(&pthread, NULL, &userinput, NULL);
	sysfatal(rs);

	/* Play intro. */
	if (curr_data_dir) {
		nscenes = read_scenes(SCENE_DIR_INTRO, curr_data_dir, &intro_scene,
							N_INTRO_SCENES);
		go_on_cutscene = 1;            /* User may skip intro (space). */
		playmovie(intro_scene, nscenes);
		go_on_cutscene = 0;
		/* Play game. */
		read_scenes(SCENE_DIR_GAME, curr_data_dir, &game_scene, N_GAME_SCENES);
		go_on = 1;

		gettimeofday(&beginning, NULL);
		init_game();
		playgame(game_scene, curr_data_dir);
	}

	endwin();
	free(intro_scene);
	intro_scene = NULL;
	free(game_scene);
	game_scene = NULL;
	free(curr_data_dir);
	free(snake.positions);

	/* Waits game control thread to end */
	int controls_thread_err = pthread_join(pthread, NULL);
	
	if (controls_thread_err) {
		return EXIT_FAILURE;
	}

	ASSERT_SYSTEM_CALL(system("killall mpg123"));
	return EXIT_SUCCESS;
}
