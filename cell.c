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
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include "cell.h"
#include "da.h"

struct cell {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
  int value;
  int nCount;
  DA * neighbors;
};

extern CELL * newCELL(void) {
  CELL * cell = malloc(sizeof(CELL));
  assert(cell != 0);
  cell->row = 0;
  cell->column = 0;
  cell->rightWall = true;
  cell->bottomWall = true;
  cell->value = 0;
  cell->nCount = 0;
  cell->neighbors = newDA();

  return cell;
}

extern void setCELLNeighbors(CELL * ptr, CELL * top, ...) {
  DA * adj = ptr->neighbors;
  va_list l; // list of function arguments
  int count = 0;
  int i = 0; // argument number 0-3
  int argCount = 4;

  va_start(l, top); // list starts at argument "arg"
  while (i < argCount) {
    if (top != 0) {
      insertDA(adj, i, top); // insert CELL into neighbors DA
      ++count;
    }
    top = va_arg(l, CELL *); // update next argument
    ++i;
  }
  va_end(l);
  ptr->nCount = count;
}

extern CELL * getCELLNeighbors(CELL * ptr, int i) {
  CELL * val = (CELL *)(getDA(ptr->neighbors, i));
  if (val->rightWall == false || val->bottomWall == false) {
    return 0;
  }
  return val;
}

extern int getValue(CELL * elem) {
  return elem->value;
}

extern void setRight(CELL * elem, bool right) {
  elem->rightWall = right;
}

extern void setBottom(CELL * elem, bool bottom) {
  elem->bottomWall = bottom;
}

extern bool getRight(CELL * elem) { return elem->rightWall; }

extern bool getBottom(CELL * elem) { return elem->bottomWall; }

extern void setCELLLocation(CELL * elem, int r, int c) {
  elem->row = r;
  elem->column = c;
}

extern int getRow(CELL * elem) { return elem->row; }

extern int getColumn(CELL * elem) { return elem->column; }

extern void setValue(CELL * elem, int val) { elem->value = val; }

extern void freeCELL(CELL * c) {
  freeDA(c->neighbors);
  free(c);
}
