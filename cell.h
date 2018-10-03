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

extern CELL * newCELL(void);
//extern bool getWallInfo(CELL * elem);
extern int getValue(CELL * elem);
extern void setWallInfo(CELL * elem, bool rightWall, bool bottomWall);
extern void setLocation(CELL * elem, int row, int column);
extern void setValue(CELL * elem, int val);
extern void freeCELL(CELL * elem);



#endif /* cell_h */
