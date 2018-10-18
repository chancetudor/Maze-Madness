//
//  maze.h
//  project1
//
//  Created by Chance Tudor on 10/2/18.
//

#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

#include <stdio.h>
#include "cell.h"
#include "da.h"

typedef struct maze MAZE;

extern MAZE * newMAZE(void);
extern void freeMAZE(MAZE * m);
extern void setMAZESize(MAZE * m, int r, int c);
extern void setMAZESeed(MAZE * m, int s);
extern int getMAZESeed(MAZE * m);
extern void setMAZEDashes(MAZE * m, int d);
extern int getMAZEDashes(MAZE * m);
extern int getMAZERows(MAZE * m);
extern int getMAZEColumns(MAZE * m);
extern void createMatrix(MAZE * m);
extern void createNeighbors(MAZE * m, CELL * curr, DA *array, int row, int col);
extern CELL * findNeighbor(CELL * c, DA * array);
extern void buildMAZE(MAZE * m);
extern void solveMAZE(MAZE * m);
extern void drawMAZE(MAZE * m, FILE * file);
extern void pushMAZE(MAZE * m, FILE * f);
extern MAZE * pullMAZE(FILE * f);


#endif /* maze_h */
