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
extern void allocateMAZE(MAZE * m);
extern void freeMAZE(MAZE * m);
extern void setMAZESize(MAZE * m, int r, int c);
extern void setMAZESeed(MAZE * m, int s);
extern void setMAZEDashes(MAZE * m, int d);
extern void setMAZEBars(MAZE * m, int b);
extern int getMAZERows(MAZE * m);
extern int getMAZEColumns(MAZE * m);
extern void createMatrix(MAZE * m);
extern void buildMAZE(MAZE * m);
extern CELL * findNeighbor(CELL * c);
extern void freeMAZE(MAZE * m);
extern void buildMAZE(MAZE * m);
// solve maze function
// draw maze function


#endif /* maze_h */
