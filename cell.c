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

  return cell;
}

extern void setCELLNeighbors(CELL * ptr, int num, DA * array, CELL * top, ...) {
  va_list l; // list of function arguments
  int count = 0; // keep track of number of elements in neighbors array
  int i = 0; // argument number 0-3

  va_start(l, top); // list starts at argument "top"
  while (i < num) {
    if (getVisited(top) == 1) {
      top = va_arg(l, CELL *); // update next argument
      ++i;
    }
    else {
      insertDAback(array, top);
      ++count;
      ++i;
      top = va_arg(l, CELL *); // update next argument
    }
  }
  va_end(l);

  /*for (int i = 0; i < sizeDA(array); i++) {
    if (getVisited((CELL*)getDA(array, i)) == 1) {
      removeDA(array, i);
      --count;
    }
  }*/
  printf("Neighbors of [%d, %d]:\n", getRow(ptr), getColumn(ptr));
  //for (int i = 0; i < sizeDA(array); i++) {
    //printf("\t[%d, %d]\n", getRow((CELL*)getDA(array, i)), getColumn((CELL*)getDA(array, i)));
  //}
  printf("Neighbor count = %d\n", count);
  setNeighborCount(ptr, count);
  printf("Neighbor count after set = %d\n", getNeighborCount(ptr));
}

/*extern void setCELLNeighbors(CELL * ptr, DA * array, CELL * val) {
  insertDAback(array, val);
  setNeighborCount(ptr, sizeDA(array));
  printf("Neighbors of [%d, %d]:\n", getRow(ptr), getColumn(ptr));
  for (int i = 0; i < sizeDA(array); i++) {
    printf("\t[%d, %d]\n", getRow((CELL*)getDA(array, i)), getColumn((CELL*)getDA(array, i)));
  }
  printf("nCount of [%d, %d] = %d\n", getRow(ptr), getColumn(ptr), getNeighborCount(ptr));
  printf("\n");
}*/

extern void setNeighborCount(CELL * ptr, int count) { ptr->nCount = count; }

extern int getNeighborCount(CELL * ptr) { return ptr->nCount; }

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

extern void freeCELL(CELL * c) { free(c); }
