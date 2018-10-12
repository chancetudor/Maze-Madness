//
//  maze.c
//  project1
//  This file contains necessary code for the maze
//
//  Created by Chance Tudor on 10/2/18.
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "maze.h"
#include "cell.h"
#include "stack.h"
#include "queue.h"

extern void srandom(unsigned int);
extern long int random(void);

struct cell {
  int row;
  int column;
  bool rightWall;
  bool bottomWall;
  int value;
  int nCount;
  CELL * neighbors[4];
};

struct maze {
  CELL * (**matrix);
  int rows;
  int columns;
  unsigned int seed;
  char * inFile;
  char * outFile;
  int numDashes;
  int numBars;
  int build;
  int solve;
  int draw;
};

extern MAZE * newMAZE(void) {
  MAZE * m = malloc(sizeof(MAZE)); // FIXME: free!
  assert(m != 0);
  m->rows = 0;
  m->columns = 0;
  m->seed = 1;
  m->inFile = 0;
  m->outFile = 0;
  m->matrix = 0;
  m->numDashes = 0;
  m->numBars = 0;
  m->build = 0;
  m->solve = 0;
  m->draw = 0;

  return m;
}

extern void freeMAZE(MAZE * m) {
  for (int i = 0; i < getMAZERows(m); ++i) {
   for (int j = 0; j < getMAZEColumns(m); ++j) {
     freeCELL(m->matrix[i][j]);
   }
 }

 free(m);
}

extern void printName(void) { printf("Written by Chance Tudor\n"); }

extern void setMAZESize(MAZE * m, int r, int c) {
  m->rows = r;
  m->columns = c;
}

extern void setMAZESeed(MAZE * m, int s) { m->seed = s; }

extern void setOutFile(MAZE * m, char * file) { m->outFile = file; }

extern void setInFile(MAZE * m, char * file) { m->inFile = file; }

extern void setBuild(MAZE * m) { m->build = 1; }

extern void setSolve(MAZE * m) { m->solve = 1; }

extern void setDraw(MAZE * m) { m->draw = 1; }

extern void setMAZEDashes(MAZE * m, int d) { m->numDashes = d; }

extern void setMAZEBars(MAZE * m, int b) { m->numBars = b; }

extern int getMAZEDashes(MAZE * m) { return m->numDashes; }

extern int getMAZEBars(MAZE * m) { return m->numBars; }

extern int getMAZERows(MAZE * m) { return m->rows; }

extern int getMAZEColumns(MAZE * m) { return m->columns; }

extern void createMatrix(MAZE * m) {
  // create a one-dimensional array of row pointers
  m->matrix = malloc(sizeof(CELL **) * getMAZERows(m)); // FIXME: free!
  // make all the rows
  for (int i = 0; i < getMAZERows(m); ++i) {
   // create a single row
   m->matrix[i] = malloc(sizeof(CELL *) * getMAZEColumns(m)); // FIXME: free!
   // initialize the slots in the row
   for (int j = 0; j < getMAZEColumns(m); ++j) {
     m->matrix[i][j] = newCELL(); // FIXME: free!
     // set row and column vals for each new cell
     CELL * ptr = m->matrix[i][j];
     setCELLLocation(ptr, i, j);
   }
 }
}

extern void createNeighbors(MAZE * m) {
  int num = 0;
  for (int row = 0; row < getMAZERows(m); row++) { // rows
    for (int col = 0; col < getMAZEColumns(m); col++) { // columns
      CELL * ptr = m->matrix[row][col];
      // curr cell, topN, leftN, rightN, bottomN
      // start of maze
      if (row == 0 && col == 0) {
        num = 2; // two neighbors
        // right and bottom neighbor
        setCELLNeighbors(num, ptr, m->matrix[row][col+1], m->matrix[row+1][col]);
      }
      // end of maze
      else if (row == getMAZERows(m) - 1 && col == getMAZEColumns(m) - 1) {
        num = 2; // two neighbors
        // top and left neighbor
        setCELLNeighbors(num, ptr, m->matrix[row-1][col], m->matrix[row][col-1]);
      }
      // first row
      else if (row == 0) {
        // last column
        if (col == getMAZEColumns(m) - 1) {
          num = 2; // two neighbors
          // left and bottom neighbor
          setCELLNeighbors(num, ptr, m->matrix[row][col-1], m->matrix[row+1][col]);
        }
        else {
          num = 3; // three neighbors
          // left, right, and bottom neighbor
          setCELLNeighbors(num, ptr, m->matrix[row][col-1], m->matrix[row][col+1], m->matrix[row+1][col]);
        }
      }
      // bottom row
      else if (row == getMAZERows(m) - 1) {
        // first column
        if (col == 0) {
          num = 2; // two neighbors
          // top and right neighbor
          setCELLNeighbors(num, ptr, m->matrix[row-1][col], m->matrix[row][col+1]);
        }
        else {
          num = 3; // three neighbors
          // top, left, and right neighbor
          setCELLNeighbors(num, ptr, m->matrix[row-1][col], m->matrix[row][col-1], m->matrix[row][col+1]);
        }
      }
      // first column
      else if (col == 0) {
        num = 3; // three neighbors
        // top, right, and bottom neighbor
        setCELLNeighbors(num, ptr, m->matrix[row-1][col], m->matrix[row][col+1], m->matrix[row+1][col]);
      }
      // last column
      else if (col == getMAZEColumns(m) - 1) {
        num = 3; // three neighbors
        // top, left, and bottom neighbor
        setCELLNeighbors(num, ptr, m->matrix[row-1][col], m->matrix[row][col-1], m->matrix[row+1][col]);
      }
      // cell in middle of array
      else {
        num = 4;
        // top, left, right, and bottom neighbor
        setCELLNeighbors(num, ptr, m->matrix[row-1][col], m->matrix[row][col-1], m->matrix[row][col+1], m->matrix[row+1][col]);
      }
    }
  }
}

extern CELL * findNeighbor(CELL * input) {
  printf("in findNeighbor()\n");
  //printf("\tCurrent CELL row = %d || column = %d\n", getRow(input), getColumn(input));
  unsigned int index = random() % getNeighborCount(input);
  printf("\tindex = %d\n", index);
  printf("\tneighbor count = %d\n", getNeighborCount(input));
  CELL * val = getCELLNeighbors(input, index);

  return val;
}

// performs DFS
extern void buildMAZE(MAZE * m) {
  printf("in buildMAZE()\n");
  STACK * s = newSTACK(); // FIXME: free!
  srandom(m->seed);
  CELL * curr = m->matrix[0][0];
  setVisited(curr, 1);
  push(s, curr);
  printf("\tinitial push\n");
  CELL * temp = (CELL *)peekSTACK(s);
  printf("\t\tCurrent CELL row = %d || column = %d\n", getRow(temp), getColumn(temp));

  while (sizeSTACK(s) != 0) {
    curr = (CELL *)peekSTACK(s);
    CELL * neighbor = findNeighbor(curr);
    if (neighbor == 0) {
      printf("\tPopping\n");
      pop(s);
    }
    else {
      // if path went up
      if (getRow(neighbor) < getRow(curr)) {
        //printf("\tPath went up\n");
        //printf("\tRemoving bottom of neighbor\n");
        setBottom(neighbor, 0);
      }
      // if path went down
      if (getRow(neighbor) > getRow(curr)) {
        //printf("\tPath went down\n");
        //printf("\tRemoving bottom of current\n");
        setBottom(curr, 0);
      }
      // if path went left
      if (getColumn(neighbor) < getColumn(curr)) {
        //printf("\tPath went left\n");
        //printf("\tRemoving right of neighbor\n");
        setRight(neighbor, 0);
      }
      // if path went right
      if (getColumn(neighbor) > getColumn(curr)) {
        //printf("\tPath went right\n");
        //printf("\tRemoving right of current\n");
        setRight(curr, 0);
      }
      printf("\tPushing neighboring cell\n");
      push(s, neighbor);
      temp = (CELL *)peekSTACK(s);
      printf("\t\tCurrent CELL row = %d || column = %d\n", getRow(temp), getColumn(temp));
    }
  }
  freeSTACK(s);
}

extern void solveMAZE(MAZE * m) {
  return;
}

extern void drawMAZE(MAZE * m) {
  /*int i = 0;
  int j = 0;
  printf("in drawMAZE()\n");
  for (i = 2; i < getMAZERows(m); i++) {
    for (j = 0; j < getMAZEColumns(m); j++) {
      printf("Current CELL row = %d || column = %d\n", i, j);
      printf("bottom val = %d\n", getBottom(m->matrix[i][j]));
      printf("right val = %d\n", getRight(m->matrix[i][j]));
    }
  }*/

  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "w");

  char midline[getMAZEDashes(m)];
  for (int z = 0; z < getMAZEDashes(m); z++) {
    midline[z] = '-';
  }

  int lineIndex; //index int to track midline

  // prints top barrier
  for (int z = 0; z < getMAZEDashes(m); z++) {
    fprintf(outFile, "-");
  }
  fprintf(outFile, "\n");
  // iterates through rows
  for (int i = 0; i < getMAZERows(m); i++) {
    char midline[getMAZEDashes(m)];
    for (int z = 0; z < getMAZEDashes(m); z++) {
      midline[z] = '-';
    }
    // if not start, print left wall
    if (i != 0) { fprintf(outFile, "|"); }
    // if start, do not print left wall
    else if (i == 0) { fprintf(outFile, " "); }
    // reset line index
    lineIndex = 1;
    // iterates through columns
    for (int j = 0; j < getMAZEColumns(m); j++) {
      //printf("Current CELL row = %d || column = %d\n", i, j);
      //printf("\tright val = %d\n", getRight(m->matrix[i][j]));
      //printf("\tbottom val = %d\n", getBottom(m->matrix[i][j]));
      // if right wall exists
      if (getRight(m->matrix[i][j]) == 1) {
        fprintf(outFile, " %d |", j);
      }
      else if (getRight(m->matrix[i][j]) == 0) {
        fprintf(outFile, " %d  ", j);
      }
      // if bottom wall does not exist
      if (getBottom(m->matrix[i][j]) == 0) {
        midline[lineIndex] = ' ';
        midline[lineIndex + 1] = ' ';
        midline[lineIndex + 2] = ' ';
        lineIndex = lineIndex + 4;
      }
    }
    fprintf(outFile, "\n");
    for (int x = 0; x < getMAZEDashes(m); x++) {
      fprintf(outFile, "%c", midline[x]);
    }
    fprintf(outFile, "\n");
  }

  fclose(outFile);
}

extern void pushMAZE(MAZE * m) {
  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "w");


  fclose(outFile);
}
