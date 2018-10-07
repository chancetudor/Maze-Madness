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

typedef struct maze MAZE;

extern MAZE * newMAZE(void);
extern void allocateMaze(MAZE * m);
extern void freeMAZE(MAZE * m);
extern void setMazeSize(MAZE * m, int r, int c);
extern void setMazeSeed(MAZE * m, int s);
extern void setMazeDashes(MAZE * m, int d);
extern void setMazeBars(MAZE * m, int b);
extern int getMazeRows(MAZE * m);
extern int getMazeColumns(MAZE * m);
extern void createMatrix(MAZE * m);
extern void buildMAZE(MAZE * m);
// solve maze function
// build maze function
// draw maze function


#endif /* maze_h */
