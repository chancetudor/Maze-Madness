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
#include "da.h"

extern void srandom(unsigned int);
extern long int random(void);

struct maze {
  CELL * (**matrix);
  int rows;
  int columns;
  unsigned int seed;
  char * inFile;
  char * outFile;
  int numDashes;
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

extern int getMAZESeed(MAZE * m) { return m->seed; }

extern void setOutFile(MAZE * m, char * file) { m->outFile = file; }

extern void setInFile(MAZE * m, char * file) { m->inFile = file; }

extern void setBuild(MAZE * m) { m->build = 1; }

extern void setSolve(MAZE * m) { m->solve = 1; }

extern void setDraw(MAZE * m) { m->draw = 1; }

extern void setMAZEDashes(MAZE * m, int d) { m->numDashes = d; }

extern int getMAZEDashes(MAZE * m) { return m->numDashes; }

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
     // end of maze should not have right wall
     if (i == getMAZERows(m) - 1 && j == getMAZEColumns(m) - 1) { setRight(ptr, 0); }
   }
 }
}

extern void createNeighbors(MAZE * m, CELL * curr, DA * array, int row, int col) {
  int n = 0;
  // first row
  if (row == 0) {
    // top left corner
    if (col == 0) {
      n = 2;
      CELL * right = m->matrix[row][col+1];
      CELL * bottom = m->matrix[row+1][col];
      // right and bottom neighbor
      setCELLNeighbors(curr, n, array, right, bottom);
    }
    // top right corner
    else if (col == getMAZEColumns(m) - 1) {
      n = 2;
      CELL * left = m->matrix[row][col-1];
      CELL * bottom = m->matrix[row+1][col];
      // left and bottom neighbor
      setCELLNeighbors(curr, n, array, left, bottom);
    }
    // middle of first row
    else {
      n = 3;
      CELL * left = m->matrix[row][col-1];
      CELL * right = m->matrix[row][col+1];
      CELL * bottom = m->matrix[row+1][col];
      // left, right, and bottom neighbor
      setCELLNeighbors(curr, n, array, left, right, bottom);
    }
  }

  // last row
  else if (row == getMAZERows(m) - 1) {
    // bottom left corner
    if (col == 0) {
      n = 2;
      CELL * top = m->matrix[row-1][col];
      CELL * right = m->matrix[row][col+1];
      // top and right neighbor
      setCELLNeighbors(curr, n, array, top, right);
    }
    // bottom right corner
    else if (col == getMAZEColumns(m) - 1) {
      n = 2;
      CELL * top = m->matrix[row-1][col];
      CELL * left = m->matrix[row][col-1];
      // top and left neighbor
      setCELLNeighbors(curr, n, array, top, left);
    }
    // middle of last row
    else {
      n = 3;
      CELL * top = m->matrix[row-1][col];
      CELL * left = m->matrix[row][col-1];
      CELL * right = m->matrix[row][col+1];
      // top, left, and right neighbor
      setCELLNeighbors(curr, n, array, top, left, right);
    }
  }

  // middle of matrix
  else if (row != getMAZERows(m) - 1) {
    // middle of first column
    if (col == 0) {
      n = 3;
      CELL * top = m->matrix[row-1][col];
      CELL * right = m->matrix[row][col+1];
      CELL * bottom = m->matrix[row+1][col];
      // top, right, and bottom neighbor
      setCELLNeighbors(curr, n, array, top, right, bottom);
    }
    // middle of last column
    else if (col == getMAZEColumns(m) - 1) {
      n = 3;
      CELL * top = m->matrix[row-1][col];
      CELL * left = m->matrix[row][col-1];
      CELL * bottom = m->matrix[row+1][col];
      // top, left, and bottom neighbor
      setCELLNeighbors(curr, n, array, top, left, bottom);
    }
    // dead middle of array
    else {
      n = 4;
      CELL * top = m->matrix[row-1][col];
      CELL * left = m->matrix[row][col-1];
      CELL * right = m->matrix[row][col+1];
      CELL * bottom = m->matrix[row+1][col];
      // top, left, right, and bottom neighbor
      setCELLNeighbors(curr, n, array, top, left, right, bottom);
    }
  }
}

// performs DFS
extern void buildMAZE(MAZE * m) {
  STACK * s = newSTACK();
  srandom(getMAZESeed(m));
  CELL * curr = m->matrix[0][0];
  setVisited(curr, 1);
  push(s, curr);

  while (sizeSTACK(s) != 0) {
    curr = (CELL *)peekSTACK(s);
    int row = getRow(curr);
    int col = getColumn(curr);

    DA * neighbors = newDA();
    createNeighbors(m, curr, neighbors, row, col);
    CELL * neighbor;
    if (getNeighborCount(curr) != 0) {
      unsigned int index = random() % getNeighborCount(curr);
      neighbor = (CELL *)getDA(neighbors, index);
    }
    else {
      neighbor = 0;
    }

    // no eligible neighbors, so pop
    if (neighbor != 0 && getVisited(neighbor) == 1) {
      pop(s);
    }
    else if (neighbor == 0) {
      pop(s);
    }
    else {
      // if path went up
      if (getRow(neighbor) < getRow(curr)) {
        setBottom(neighbor, 0);
      }
      // if path went down
      if (getRow(neighbor) > getRow(curr)) {
        setBottom(curr, 0);
      }
      // if path went left
      if (getColumn(neighbor) < getColumn(curr)) {
        setRight(neighbor, 0);
      }
      // if path went right
      if (getColumn(neighbor) > getColumn(curr)) {
        setRight(curr, 0);
      }
      setVisited(neighbor, 1);
      push(s, neighbor);
    }
  }
  freeSTACK(s);
}

/*extern void solveMAZE(MAZE * m) {
  return;
}*/

extern void drawMAZE(MAZE * m) {
  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "w");

  // initializes dash array for printing
  char midDash[getMAZEDashes(m)];
  for (int i = 0; i < getMAZEDashes(m); i++) {
    midDash[i] = '-';
  }

  int lineIndex;

  // prints top barrier
  for (int i = 0; i < getMAZEDashes(m); i++) {
    fprintf(outFile, "-");
  }

  fprintf(outFile, "\n");

  // iterates through rows
  for (int i = 0; i < getMAZERows(m); i++) {
    // reinitializes dash array for printing
    for (int k = 0; k < getMAZEDashes(m); k++) {
      midDash[k] = '-';
    }

    // if not start, print left wall
    if (i != 0) { fprintf(outFile, "|"); }
    // if start, do not print left wall
    else if (i == 0) { fprintf(outFile, " "); }
    // reset line index
    lineIndex = 1;
    // iterates through columns
    for (int j = 0; j < getMAZEColumns(m); j++) {
      // if last cell, do not print right wall
      if (i == getMAZERows(m) - 1 && j == getMAZEColumns(m) - 1) {
        fprintf(outFile, "   ");
      }
      // if right wall exists
      else if (getRight(m->matrix[i][j]) == 1) {
        fprintf(outFile, "   |");
      }
      else if (getRight(m->matrix[i][j]) == 0) {
        fprintf(outFile, "    ");
      }
      // if bottom wall does not exist
      if (getBottom(m->matrix[i][j]) == 0) {
        midDash[lineIndex] = ' ';
        midDash[lineIndex + 1] = ' ';
        midDash[lineIndex + 2] = ' ';
        lineIndex = lineIndex + 4;
      }
      else if (getBottom(m->matrix[i][j]) == 1) {
        lineIndex = lineIndex + 4;
      }
    }
    fprintf(outFile, "\n");

    // prints barrier between rows
    for (int x = 0; x < getMAZEDashes(m); x++) {
      fprintf(outFile, "%c", midDash[x]);
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
