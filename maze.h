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
extern void printName(void);
extern void setInFile(MAZE * m, char * iFile);
extern void setOutFile(MAZE * m, char * oFile);
extern void setBuild(MAZE * m);
extern void setSolve(MAZE * m);
extern void setDraw(MAZE * m);
extern int getMAZEDashes(MAZE * m);
extern int getMAZEBars(MAZE * m);
extern int getMAZERows(MAZE * m);
extern int getMAZEColumns(MAZE * m);
extern void createMatrix(MAZE * m);
extern void createNeighbors(MAZE * m);
extern void buildMAZE(MAZE * m);
extern CELL * findNeighbor(CELL * c);
extern void freeMAZE(MAZE * m);
extern void buildMAZE(MAZE * m);
// solve maze function
extern void drawMAZE(MAZE * m);
extern void pushMAZE(MAZE * m);


#endif /* maze_h */
