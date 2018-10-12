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
  printf("\tCurrent CELL row = %d || column = %d\n", getRow(input), getColumn(input));
  unsigned int index = random() % getNeighborCount(input);
  CELL * val = getCELLNeighbors(input, index);

  return val;
}

extern void buildMAZE(MAZE * m) {
  printf("in buildMAZE()\n");
  STACK * s = newSTACK(); // FIXME: free!
  srandom(m->seed);
  CELL * curr = m->matrix[0][0];
  setVisited(curr, 1);
  push(s, curr);

  while (sizeSTACK(s) != 0) {
    curr = (CELL *)peekSTACK(s);
    CELL * neighbor = findNeighbor(curr);
    if (neighbor == 0) {
      printf("\tPopping\n");
      pop(s);
    }
    else {
      // if path went up
      if (getColumn(neighbor) < getColumn(curr)) {
        printf("\tPath went up\n");
        printf("\tRemoving bottom of neighbor\n");
        setBottom(neighbor, 0);
      }
      // if path went down
      if (getColumn(neighbor) > getColumn(curr)) {
        printf("\tPath went down\n");
        printf("\tRemoving bottom of current\n");
        setBottom(curr, 0);
      }
      // if path went left
      if (getRow(neighbor) < getRow(curr)) {
        printf("\tPath went left\n");
        printf("\tRemoving right of neighbor\n");
        setRight(neighbor, 0);
      }
      // if path went right
      if (getRow(neighbor) > getRow(curr)) {
        printf("\tPath went right\n");
        printf("\tRemoving right of current\n");
        setRight(curr, 0);
      }
      printf("\tPushing neighboring cell\n");
      push(s, neighbor);
    }
  }
  freeSTACK(s);
}

extern void solveMAZE(MAZE * m) {
  return;
}

extern void drawMAZE(MAZE * m) {
  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "w");
  // prints initial dashes
  for (int i = 0; i < getMAZEDashes(m); i++) {
    fprintf(outFile, "-");
  }
  fprintf(outFile, "\n");

  // first cell should not have left wall
  fprintf(outFile, " ");
  // printing maze
  for (int i = 0; i < getMAZERows(m); i++) { // rows
    for (int j = 0; j < getMAZEColumns(m); j++) { // columns
      // first cell of every row except first
      if (i != 0 && j == 0) {
        fprintf(outFile, "|");
      }
      // last cell should not have right wall
      if (i == getMAZERows(m) - 1 && j == getMAZEColumns(m) - 1) {
        //fprintf(outFile, " ");
        //fprintf(outFile, "%d ", getValue(m->matrix[i][j]));
        if (getVisited(m->matrix[i][j]) == 0 && getRight(m->matrix[i][j]) == 1) {
          //fprintf(outFile, "0|");
          //fprintf(outFile, "|");
          fprintf(outFile, "NV|");
        }
        else if (getVisited(m->matrix[i][j]) == 1 && getRight(m->matrix[i][j]) == 1) {
          //fprintf(outFile, " |");
          fprintf(outFile, "%d|", getValue(m->matrix[i][j]));
        }
        else if (getVisited(m->matrix[i][j]) == 1 && getRight(m->matrix[i][j]) == 0) {
          fprintf(outFile, "%d ", getValue(m->matrix[i][j]));
        }
      }
      // normal cell
      else if (i != getMAZERows(m) && j != getMAZEColumns(m)) {
        // if right wall exists
        if (getVisited(m->matrix[i][j]) == 0 && getRight(m->matrix[i][j]) == 1) {
          //fprintf(outFile, "0|");
          //fprintf(outFile, "|");
          fprintf(outFile, "NV|");
        }
        else if (getVisited(m->matrix[i][j]) == 1 && getRight(m->matrix[i][j]) == 1) {
          //fprintf(outFile, " |");
          fprintf(outFile, "%d|", getValue(m->matrix[i][j]));
        }
        else if (getVisited(m->matrix[i][j]) == 1 && getRight(m->matrix[i][j]) == 0) {
          fprintf(outFile, "%d ", getValue(m->matrix[i][j]));
        }
        //fprintf(outFile, "%d|", getValue(m->matrix[i][j]));
      }
    }
    // newline separates each row
    fprintf(outFile, "\n");
    // prints dashes between rows
    for (int k = 0; k < getMAZEDashes(m); k++) {
      fprintf(outFile, "-");
    }
    fprintf(outFile, "\n");
  }
  //fprintf(outFile, "\n");
  fclose(outFile);
}

extern void pushMAZE(MAZE * m) {
  FILE * outFile;
  char * oFile = m->outFile;
  outFile = fopen(oFile, "w");


  fclose(outFile);
}
