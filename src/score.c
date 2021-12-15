/* score.c - Implementation of TexTronSnake score system.

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
#include <string.h>
#include <curses.h>
#include "score.h"

/*Struct used to represent a single score*/
typedef struct {
    char nickname[MAX_NICKNAME + 1];
    int points;
} score_t;


/*Struct used to represent all scores from score system*/
typedef struct {
    int nTopScores;
    score_t scores[MAX_SCORES];
} top_scores_t;

/*Function used to sort scores after new score is entered*/
int compare_scores(const void* a, const void* b){
    score_t* score1 = (score_t*) a;
    score_t* score2 = (score_t*) b;

    if(score1->points < score2->points){
        return 1;
    }
    else if(score1->points > score2->points){
        return -1;
    }
    else{
        return 0;
    }
}

/*Read all scores from score file and returns a top_scores_t struct*/
top_scores_t* read_scores(){
    top_scores_t* topScores = (top_scores_t*) malloc(sizeof(top_scores_t));

    FILE* fp = fopen(SCORES_FILE, "rb");
    
    if(fp == NULL){
        topScores->nTopScores = 0;
        return topScores;  
    }

    int nScores;
    if(!fread(&nScores, sizeof(int), 1, fp)){
        fclose(fp);
        topScores->nTopScores = 0;
        return topScores;  
    }

    topScores->nTopScores = nScores;

    int i;
    for(i = 0; i < nScores; i++) {
        fread(topScores->scores[i].nickname, sizeof(char), MAX_NICKNAME + 1, fp);
        fread(&(topScores->scores[i].points), sizeof(int), 1, fp);
    }

    fclose(fp);

    return topScores;
}


/*Write top scores to score file*/
void write_scores(top_scores_t* topScores){

    FILE* fp = fopen(SCORES_FILE, "wb");
    
    if(fp == NULL){
        return;
    }

    fwrite(&(topScores->nTopScores), sizeof(int), 1, fp);

    int i;
    for(i = 0; i < topScores->nTopScores; i++) {
        fwrite(topScores->scores[i].nickname, sizeof(char), MAX_NICKNAME + 1, fp);
        fwrite(&(topScores->scores[i].points), sizeof(int), 1, fp);
    }

    fclose(fp);

    return;
}

/*Add new score to score file if has space or score is greater than last score*/
void add_score(const char nickname[MAX_NICKNAME+1], int points){
    score_t score;

    strcpy(score.nickname, nickname);
    score.points = points;

    top_scores_t* topScores = read_scores();

        
    if(topScores->nTopScores == MAX_SCORES && score.points <= topScores->scores[MAX_SCORES-1].points){
        free(topScores);
        return;
    }

    if(topScores->nTopScores == MAX_SCORES) {
        topScores->scores[topScores->nTopScores-1].points = score.points;

        strcpy(topScores->scores[topScores->nTopScores-1].nickname, score.nickname);
    }
    
    else {
        topScores->scores[topScores->nTopScores].points = score.points;

        strcpy(topScores->scores[topScores->nTopScores].nickname, score.nickname);
        
        topScores->nTopScores++;
    }


    qsort(topScores->scores,topScores->nTopScores,sizeof(score_t),compare_scores);

    write_scores(topScores);

    free(topScores);
    return;
}


/*Print all score at the bottom of the window*/
void print_scores(WINDOW* mainWindow, int NROWS, int NCOLS) {
    top_scores_t* topScores = read_scores();

    int i;

    mvwprintw(mainWindow, NROWS*3/4+8,0,"Top Scores:\n");
    int actualY = NROWS*3/4+10;
    int actualX = 0;
    for(i = 0; i < topScores->nTopScores; i++) {
        mvwprintw(mainWindow,actualY,actualX,"%d: Nickname: %4s; Score: %d\t", i+1, topScores->scores[i].nickname,  topScores->scores[i].points);

        actualX+=NCOLS/3;
        if(actualX >= NCOLS){
            actualX = 0;
            actualY++;
        }
    }

    free(topScores);
}



