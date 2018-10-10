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

struct neighbor {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
};

struct cell {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
  int value;
  int nCount;
  NEIGHBOR * neighbors[4];
};

static NEIGHBOR * newNEIGHBOR(void) {
  NEIGHBOR * n = malloc(sizeof(NEIGHBOR));
  n->row = 0;
  n->column = 0;
  n->rightWall = true;
  n->bottomWall = true;

  return n;
}

extern CELL * newCELL(void) {
  CELL * cell = malloc(sizeof(CELL));
  assert(cell != 0);
  cell->row = 0;
  cell->column = 0;
  cell->rightWall = true;
  cell->bottomWall = true;
  cell->value = 0;
  cell->nCount = 0;
  for (int i = 0; i < 4; i++) {
    cell->neighbors[i] = newNEIGHBOR();
  }

  return cell;
}

extern void setCELLNeighbors(CELL * ptr, CELL * top, ...) {
  printf("In setCELLNeighbors\n");
  va_list l; // list of function arguments
  int count = 0; // keep track of number of elements in neighbors array
  int i = 0; // argument number 0-3
  int argCount = 4;
  printf("top row = %d, top column = %d\n", getRow(top), getColumn(top));
  va_start(l, top); // list starts at argument "top"
  while (i < argCount) {
    if (top != 0) { // if arg exists
      ptr->neighbors[i]->row = top->row;
      ptr->neighbors[i]->column = top->column;
      ptr->neighbors[i]->rightWall = top->rightWall;
      ptr->neighbors[i]->bottomWall = top->bottomWall;
      ++count;
      //printf("cell row = %d, cell column = %d\n", top->row, top->column);
    }
    top = va_arg(l, CELL *); // update next argument
    ++i;
  }
  va_end(l);
  ptr->nCount = count;
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

extern int getValue(CELL * elem) { return elem->value; }

extern void freeCELL(CELL * c) {
  free(c);
}
