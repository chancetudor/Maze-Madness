//
//  cell.h
//  project1
//
//  Created by Chance Tudor on 10/2/18.
//

#ifndef __CELL_INCLUDED__
#define __CELL_INCLUDED__

#include <stdio.h>
#include <stdbool.h>

typedef struct cell CELL;
typedef struct neighbor NEIGHBOR;

extern CELL * newCELL(void);
//extern int getWallInfo(CELL * elem);
extern void setValue(CELL * elem, int val);
extern int getValue(CELL * elem);
extern void setRight(CELL * elem, int rightWall);
extern void setBottom(CELL * elem, int bottomWall);
extern int getRight(CELL * elem);
extern int getBottom(CELL * elem);
extern void setCELLLocation(CELL * elem, int row, int column);
extern int getRow(CELL * elem);
extern int getColumn(CELL * elem);
extern void setCELLNeighbors(CELL * ptr, CELL * top, CELL * left, CELL * right, CELL * bottom);
extern CELL * getCELLNeighbors(CELL * ptr, unsigned int i);
extern void freeCELL(CELL * elem);



#endif /* cell_h */
