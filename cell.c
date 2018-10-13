//
//  cell.c
//  project1
//  This file contains necessary code for
//  cell objects of the maze
//
//  Created by Chance Tudor on 10/2/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include "cell.h"

struct cell {
  int row;
  int column;
  int rightWall;
  int bottomWall;
  int value;
  int nCount;
  int visited;
  //CELL * neighbors[4];
};

extern CELL * newCELL(void) {
  CELL * cell = malloc(sizeof(CELL));
  assert(cell != 0);
  cell->row = 0;
  cell->column = 0;
  cell->rightWall = 1;
  cell->bottomWall = 1;
  cell->value = 0;
  cell->nCount = 0;
  cell->visited = 0;
  //for (int i = 0; i < 4; i++) {
    //cell->neighbors[i] = 0;
  //}

  return cell;
}

extern void setCELLNeighbors(CELL * ptr, int num, CELL **array, CELL * top, ...) {
  printf("in setCELLNeighbors()\n");
  va_list l; // list of function arguments
  int count = 0; // keep track of number of elements in neighbors array
  int i = 0; // argument number 0-3

  va_start(l, top); // list starts at argument "top"
  while (i < num) {
    if (getVisited(top) != 1) {
      //ptr->neighbors[i] = top;
      array[i] = top;
      ++count;
    }
    //else { array[i] = 0; }
    ++i;
    top = va_arg(l, CELL *); // update next argument
  }
  va_end(l);
  setNeighborCount(ptr, count);
  printf("\tneighbor count = %d\n", getNeighborCount(ptr));
}

/*extern void removeNeighbor(CELL * currCell, int index) {
  for (int i = index; i < getNeighborCount(currCell) - 1; i++) {
    currCell->neighbors[i] = currCell->neighbors[i + 1];
  }
  int newCount = getNeighborCount(currCell) - 1;
  setNeighborCount(currCell, newCount);
}*/

extern void setNeighborCount(CELL * ptr, int count) { ptr->nCount = count; }

extern int getNeighborCount(CELL * ptr) { return ptr->nCount; }

/*extern CELL * getCELLNeighbors(CELL * ptr, unsigned int i) {
  CELL * val = ptr->neighbors[i];
  if (getVisited(val) == 1) {
    return 0;
  }
  //if (getBottom(val) == 0 || getRight(val) == 0) {
    //return 0;
  //}

  else {
    return val;
  }
}*/

extern void setVisited(CELL * elem, int v) { elem->visited = v; }

extern int getVisited(CELL * elem) { return elem->visited; }

extern void setRight(CELL * elem, int right) { elem->rightWall = right; }

extern void setBottom(CELL * elem, int bottom) { elem->bottomWall = bottom; }

extern int getRight(CELL * elem) { return elem->rightWall; }

extern int getBottom(CELL * elem) { return elem->bottomWall; }

extern void setCELLLocation(CELL * elem, int r, int c) {
  elem->row = r;
  elem->column = c;
}

extern int getRow(CELL * elem) { return elem->row; }

extern int getColumn(CELL * elem) { return elem->column; }

extern void setValue(CELL * elem, int val) { elem->value = val; }

extern int getValue(CELL * elem) { return elem->value; }

extern void freeCELL(CELL * c) {
  //freeDA(c->neighbors);
  free(c);
}
