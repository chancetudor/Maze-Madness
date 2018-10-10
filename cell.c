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
  CELL * neighbors[4];
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
  for (int i = 0; i < 4; i++) {
    cell->neighbors[i] = 0;
  }

  return cell;
}

extern void setCELLNeighbors(CELL * ptr, CELL * top, CELL * left, CELL * right, CELL * bottom) {
  printf("In setCELLNeighbors\n");
  int count = 0; // keep track of number of elements in neighbors array
  if (top != 0) {
    ptr->neighbors[0] = top;
    ++count;
  }
  if (left != 0) {
    ptr->neighbors[1] = left;
    ++count;
  }
  if (right != 0) {
    ptr->neighbors[2] = right;
    ++count;
  }
  if (bottom != 0) {
    ptr->neighbors[3] = bottom;
    ++count;
  }
  ptr->nCount = count;
  printf("Curr CELL row = %d, column = %d\n", getRow(ptr), getColumn(ptr));
  printf("neighbor count of curr CELL = %d\n", ptr->nCount);
  /*va_list l; // list of function arguments
  int count = 0; // keep track of number of elements in neighbors array
  int i = 0; // argument number 0-3
  int argCount = 4;

  va_start(l, top); // list starts at argument "top"
  while (i < argCount) {
    if (top != 0) { // if arg exists
      ptr->neighbors[i] = top;
      ++count;
      //printf("cell row = %d, cell column = %d\n", top->row, top->column);
    }
    ++i;
    top = va_arg(l, CELL *); // update next argument
  }
  va_end(l);
  ptr->nCount = count;*/
  printf("End of setCELLNeighbors\n");
}

extern CELL * getCELLNeighbors(CELL * ptr, unsigned int i) {
  printf("in getCELLNeighbors(); index = %d\n", i);
  CELL * val = ptr->neighbors[i];
  if (getRight(val) == false || getBottom(val) == false) {
    return 0;
  }
  return val;
}

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
