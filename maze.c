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
	int numDashes;
};

extern MAZE * newMAZE(void) {
	MAZE * m = malloc(sizeof(MAZE)); // FIXME: free!
	assert(m != 0);
	m->rows = 0;
	m->columns = 0;
	m->seed = 1;
	m->matrix = 0;
	m->numDashes = 0;

	return m;
}

extern void freeMAZE(MAZE * m) {
	for (int i = 0; i < getMAZERows(m); ++i) {
		for (int j = 0; j < getMAZEColumns(m); ++j) {
      CELL * ptr = m->matrix[i][j];
			freeCELL(ptr);
		}
	}

	free(m);
}

extern void setMAZESize(MAZE * m, int r, int c) {
	m->rows = r;
	m->columns = c;
}

extern void setMAZESeed(MAZE * m, int s) { m->seed = s; }

extern int getMAZESeed(MAZE * m) { return m->seed; }

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
	if (getMAZERows(m) == 1 && getMAZEColumns(m) == 1) {
		return;
	}
	else {
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
			if ((neighbor != 0 && getVisited(neighbor) == 1) || neighbor == 0) {
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
			freeDA(neighbors);
	  }
  }
  freeSTACK(s);
}

// performs BFS
extern void solveMAZE(MAZE * m) {
	// for each cell in maze, mark as unvisited
	for (int i = 0; i < getMAZERows(m); ++i) {
		for (int j = 0; j < getMAZEColumns(m); ++j) {
			CELL * ptr = m->matrix[i][j];
			setVisited(ptr, 0);
		}
	}
	// creates queue for BFS
	QUEUE * q = newQUEUE();
	CELL * start = m->matrix[0][0];
	setVisited(start, 1);
	int step = 1;
	int val = 0;
	setValue(start, val);
	enqueue(q, start);
	while (sizeQUEUE(q) != 0) {
		CELL * curr = (CELL *)dequeue(q);
		int currRow = getRow(curr);
		int currCol = getColumn(curr);
		// if cell is the exit cell, break exploration process
		if (currRow == getMAZERows(m) - 1 && currCol == getMAZEColumns(m) - 1) {
      // mark as visited
      setVisited(curr, 2);
      //val = step % 10;
      val = getValue(curr);
      if (val > 9) {
        val = 0;
      }
      setValue(curr, val);
      goto end;
    }
		else {
			DA * neighbors = newDA();
			createNeighbors(m, curr, neighbors, currRow, currCol);
			for (int i = 0; i < getNeighborCount(curr); ++i) {
				CELL * neighbor = (CELL *)getDA(neighbors, i);
        CELL * pred = curr;
        // neighbor went up
        if (getRow(neighbor) < getRow(curr)) {
          if (getBottom(neighbor) == 0 && getVisited(neighbor) == 0) {
            // mark as visited
            setVisited(neighbor, 1);
            //val = step % 10;
            val = getValue(pred) + 1;
            if (val > 9) {
              val = 0;
            }
            setValue(neighbor, val);
            // enqueue onto queue to keep exploring
            enqueue(q, neighbor);
            ++step;
          }
        }
        // neighbor went down
        if (getRow(neighbor) > getRow(curr)) {
          if (getBottom(curr) == 0 && getVisited(neighbor) == 0) {
            // mark as visited
            setVisited(neighbor, 1);
            //val = step % 10;
            val = getValue(pred) + 1;
            if (val > 9) {
              val = 0;
            }
            setValue(neighbor, val);
            // enqueue onto queue to keep exploring
            enqueue(q, neighbor);
            ++step;
          }
        }
        // neighbor went left
        if (getColumn(neighbor) < getColumn(curr)) {
          if (getRight(neighbor) == 0 && getVisited(neighbor) == 0) {
            // mark as visited
            setVisited(neighbor, 1);
            //val = step % 10;
            val = getValue(pred) + 1;
            if (val > 9) {
              val = 0;
            }
            setValue(neighbor, val);
            // enqueue onto queue to keep exploring
            enqueue(q, neighbor);
            ++step;
          }
        }

        // neighbor went right
        if (getColumn(neighbor) > getColumn(curr)) {
          if (getRight(curr) == 0 && getVisited(neighbor) == 0) {
            // mark as visited
            setVisited(neighbor, 1);
            //val = step % 10;
            val = getValue(pred) + 1;
            if (val > 9) {
              val = 0;
            }
            setValue(neighbor, val);
            // enqueue onto queue to keep exploring
            enqueue(q, neighbor);
            ++step;
          }
        }
			}
			setVisited(curr, 2);
			freeDA(neighbors);
		}
	}
  end:
  freeQUEUE(q);
}

extern void drawMAZE(MAZE * m, FILE * file) {
	// initializes dash array for printing
	char midDash[getMAZEDashes(m)];
	for (int i = 0; i < getMAZEDashes(m); i++) {
		midDash[i] = '-';
	}

	int lineIndex;

	// prints top barrier
	for (int i = 0; i < getMAZEDashes(m); i++) {
		fprintf(stdout, "-");
	}

	fprintf(stdout, "\n");

	// iterates through rows
	for (int i = 0; i < getMAZERows(m); i++) {
		// reinitializes dash array for printing
		for (int k = 0; k < getMAZEDashes(m); k++) {
			midDash[k] = '-';
		}

		// if not start, print left wall
		if (i != 0) { fprintf(stdout, "|"); }
		// if start, do not print left wall
		else if (i == 0) { fprintf(stdout, " "); }
		// reset line index
		lineIndex = 1;
			// iterates through columns
		for (int j = 0; j < getMAZEColumns(m); j++) {
      CELL * ptr = m->matrix[i][j];
			// if last cell, do not print right wall
			if (i == getMAZERows(m) - 1 && j == getMAZEColumns(m) - 1) {
        if (getValue(ptr) >= 0 && getVisited(ptr) == 2) {
          fprintf(file, " %d  ", getValue(ptr));
        }
				else { fprintf(stdout, "    "); }
			}
			// if right wall exists print vert. bar
			else if (getRight(ptr) == 1) {
        if (getValue(ptr) >= 0 && getVisited(ptr) == 2) {
          fprintf(file, " %d |", getValue(ptr));
        }
        else { fprintf(stdout, "   |"); }
			}
			// if right wall does not exists, don't print vert. bar
			else if (getRight(ptr) == 0) {
        if (getValue(ptr) >= 0 && getVisited(ptr) == 2) {
          fprintf(file, " %d  ", getValue(ptr));
        }
        else { fprintf(stdout, "    "); }
			}
			// if bottom wall does not exist, print gap
			if (getBottom(ptr) == 0) {
				midDash[lineIndex] = ' ';
				midDash[lineIndex + 1] = ' ';
				midDash[lineIndex + 2] = ' ';
				lineIndex = lineIndex + 4;
			}
			// if bottom wall exists, skip to next col.
			else if (getBottom(ptr) == 1) {
				lineIndex = lineIndex + 4;
				}
		}
		fprintf(stdout, "\n");

		// prints barrier between rows
		for (int x = 0; x < getMAZEDashes(m); x++) {
			fprintf(stdout, "%c", midDash[x]);
		}
		fprintf(stdout, "\n");
	}
}

extern void pushMAZE(MAZE * m, FILE * oFile) {
	if (oFile == NULL) {
		printf("Error: file unable to be opened\n");
		exit(1);
	}
	// first line: row, col, numDashes of MAZE
	fprintf(oFile, "%d %d %d\n", getMAZERows(m), getMAZEColumns(m), getMAZEDashes(m));
	// iterate through maze, printing each cell's info
	for (int i = 0; i < getMAZERows(m); ++i) {
		for (int j = 0; j < getMAZEColumns(m); ++j) {
			CELL * ptr = m->matrix[i][j];
			// rightWall, bottomWall, value, neighbor count, visited
			fprintf(oFile, "%d %d %d %d %d\n", getRight(ptr), getBottom(ptr), getValue(ptr), getNeighborCount(ptr), getVisited(ptr));
		}
	}
}

extern MAZE * pullMAZE(FILE * iFile) {
	MAZE * new = newMAZE(); // FIXME: free?
  //MAZE * copy = new;
	if (iFile == NULL) {
		printf("Error in pullMAZE(): file unable to be opened\n");
		exit(1);
	}
	else {
		int r = 0; // rows
		int c = 0; // cols
		int d = 0; // dashes
		fscanf(iFile, "%d %d %d", &r, &c, &d);
		setMAZESize(new, r, c);
		setMAZEDashes(new, d);
	  createMatrix(new);
		for (int i = 0; i < getMAZERows(new); ++i) {
			for (int j = 0; j < getMAZEColumns(new); ++j) {
				CELL * ptr = new->matrix[i][j];
				int rWall = 0;
				int bWall = 0;
				int val = 0;
				int nCount = 0;
				int visited = 0;
				// reading numbers line by line
				// rightWall, bottomWall, value, neighbor count, visited
				fscanf(iFile, "%d %d %d %d %d", &rWall, &bWall, &val, &nCount, &visited);
				setRight(ptr, rWall);
				setBottom(ptr, bWall);
				setValue(ptr, val);
				setNeighborCount(ptr, nCount);
				setVisited(ptr, visited);
			}
		}
	}
  //free(new);
	return new;
}
