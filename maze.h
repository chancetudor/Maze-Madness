//
//  maze.h
//  project1
//
//  Created by Chance Tudor on 10/2/18.
//

#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

#include <stdio.h>

typedef struct maze MAZE;

extern MAZE * newMAZE(void);
extern void allocateMaze(MAZE * array);
extern void freeMAZE(MAZE * array);
extern void setMazeSize(MAZE * array, int r, int c);
extern void setMazeSeed(MAZE * array, int s);
extern int getMazeRows(MAZE * array);
extern int getMazeColumns(MAZE * array);
// solve maze function
// build maze function
// draw maze function


#endif /* maze_h */
