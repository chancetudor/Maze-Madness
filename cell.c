//
//  cell.c
//  project1
//  This file contains necessary code for
//  cell objects of the maze
//
//  Created by Chance Tudor on 10/2/18.
//

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "cell.h"

struct cell {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
  int value;
};

extern CELL * newCELL(void) {
  CELL * cell = malloc(sizeof(CELL));
  assert(cell != 0);
  cell->row = 0;
  cell->column = 0;
  cell->rightWall = true;
  cell->bottomWall = true;
  cell->value = 0;
}

extern int getValue(CELL * elem) {
  return elem->value;
}

extern void setWallInfo(CELL * elem, bool right, bool bottom) {
  elem->rightWall = right;
  elem->bottomWall = bottom;
}

extern void setLocation(CELL * elem, int r, int c) {
  elem->row = r;
  elem->column = c;
}

extern void setValue(CELL * elem, int val) {
  elem->value = val;
}

extern void freeCELL(CELL * elem) {
  free(elem);
}
