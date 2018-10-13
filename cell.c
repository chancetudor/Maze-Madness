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
#include "da.h"

struct cell {
  int row;
  int column;
  int rightWall;
  int bottomWall;
  int value;
  int nCount;
  int visited;
  CELL * neighbors[4];
  //DA * neighbors;
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
  for (int i = 0; i < 4; i++) {
    cell->neighbors[i] = 0;
  }
  //cell->neighbors = newDA();

  return cell;
}

extern void setCELLNeighbors(CELL * ptr, int num, CELL * top, ...) {
  //printf("In setCELLNeighbors\n");
  va_list l; // list of function arguments
  int count = 0; // keep track of number of elements in neighbors array
  int i = 0; // argument number 0-3

  va_start(l, top); // list starts at argument "top"
  while (i < num) {
    ptr->neighbors[i] = top;
    //insertDA(ptr->neighbors, i, top);
    ++count;
    ++i;
    top = va_arg(l, CELL *); // update next argument
  }
  va_end(l);
  setNeighborCount(ptr, count);
  /*printf("Neighbor CELL row = %d || column = %d\n", getRow(ptr), getColumn(ptr));
  printf("\tneighbor count = %d\n", getNeighborCount(ptr));
  for (int i = 0; i < getNeighborCount(ptr); i++) {
    printf("\tneighbor %d row = %d | col = %d \n", i, getRow((CELL *)getDA(ptr->neighbors, i)), getColumn((CELL *)getDA(ptr->neighbors, i)));
  }
  printf("End of setCELLNeighbors\n");*/
}

/*extern void removeNeighbor(CELL * currCell, int index) {
  //removeDA(currCell->neighbors, i);
  for (int i = index; i < getNeighborCount(currCell) - 1; i++) {
    currCell->neighbors[i] = currCell->neighbors[i + 1];
  }
  int newCount = getNeighborCount(currCell) - 1;
  setNeighborCount(currCell, newCount);
}*/

extern void setNeighborCount(CELL * ptr, int count) { ptr->nCount = count; }

extern int getNeighborCount(CELL * ptr) { return ptr->nCount; }

extern CELL * getCELLNeighbors(CELL * ptr, unsigned int i) {
  //printf("in getCELLNeighbors()\n");
  CELL * val = ptr->neighbors[i];
  //CELL * val = (CELL *)getDA(ptr->neighbors, i);
  setVisited(val, 1);
  //printf("\tNeighbor CELL row = %d || column = %d\n", getRow(val), getColumn(val));
  //printf("\tNeighbor CELL row = %d || column = %d\n", getRow((CELL *)getDA(ptr->neighbors, i)), getColumn((CELL *)getDA(ptr->neighbors, i)));
  if (getBottom(val) == 0 || getRight(val) == 0) {
    //printf("\t\tNeighboring cell was visited already\n");
    return 0;
  }

  else {
    return val;
  }
}

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
