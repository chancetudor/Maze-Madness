#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "maze.h"
#include "cell.h"

extern void srandom(unsigned int);

struct maze {
  CELL * (**matrix);
  int rows;
  int columns;
  unsigned int seed;
  int numDashes;
};

static void Fatal(char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"An error occured: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  exit(-1);
}

int main(int argc, char **argv) {
  int author = 0, solve = 0, build = 0, draw = 0;
  unsigned int seedVal = 1;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-v") == 0) { author = 1; }
    if (strcmp(argv[i], "-r") == 0) { seedVal = atoi(argv[i + 1]); }
    if (strcmp(argv[i], "-s") == 0) { solve = i; }
    if (strcmp(argv[i], "-c") == 0) { build = i; }
    if (strcmp(argv[i], "-d") == 0) { draw = i; }
  }

  if (argc == 1) { Fatal("%d arguments!\n", argc - 1); } // not enough arguments

  if (author) {
    printf("Written by Chance Tudor\n");
    return 0;
  }
  if (build) {
    MAZE * m = newMAZE();
    MAZE * copy = m;
    int rows = atoi(argv[build + 1]);
    int columns = atoi(argv[build + 2]);
    char * oFile = argv[build + 3];
    setMAZESize(copy, rows, columns);
    setMAZEDashes(copy, (getMAZEColumns(copy) * 4 + 1));
    setMAZESeed(copy, seedVal);
    createMatrix(copy);
    buildMAZE(copy);
    FILE * outFile = fopen(oFile, "w");
    pushMAZE(copy, outFile);
    fclose(outFile);
    freeMAZE(m);
  }
  if (solve) {
    char * iFile = argv[solve + 1];
    char * oFile = argv[solve + 2];
    FILE * inFile = fopen(iFile, "r");
    MAZE * newMaze = pullMAZE(inFile);
    fclose(inFile);
    solveMAZE(newMaze);
    FILE * outFile = fopen(oFile, "w");
    pushMAZE(newMaze, outFile);
    fclose(outFile);
  }
  if (draw) {
    char * iFile = argv[draw + 1];
    FILE * inFile = fopen(iFile, "r");
    MAZE * newMaze = pullMAZE(inFile);
    fclose(inFile);
    drawMAZE(newMaze, stdout);
  }

  return 0;
}
