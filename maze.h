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
extern void freeMAZE(MAZE * array);
extern void setMazeSize(MAZE * array, int r, int c);



#endif /* maze_h */
