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
  char * inFile;
  char * outFile;
  int numDashes;
  int numBars;
  int build;
  int solve;
  int draw;
};

static void Fatal(char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"An error occured: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  exit(-1);
}

static void ProcessOptions(MAZE * m, int argc, char **argv) {
  int start = 0;
  int argIndex = 1;
  int argsUsed = 0;
  char *arg = 0;
  char *iFile = 0; // create char array to hold name of input file
  char *oFile = 0; // create char array to hold name of output file
  int rows = 0;
  int columns = 0;
  unsigned int seed = 0;

  while (argIndex < argc && *argv[argIndex] == '-') {
  /* check if stdin, represented by "-" is an argument */
  /* if so, the end of options has been reached */
    argsUsed = 0;
    start = argIndex;

    /* advance argIndex to point to the first argument to the option */
    if (argv[start][2] == '\0') {
      arg = argv[start + 1];
      ++argIndex;
    }
    else {
      /* first arg is connected to option, so don't advance */
      arg = argv[start] + 2;
    }

    switch (argv[start][1]) {
      /*
      * when option has an argument, do this
      *
      *     examples are -m4096 or -m 4096
      *
      *     case 'm':
      *         MemorySize = atol(arg);
      *         argsUsed = 1;
      *         break;
      * ///////////////////////////////////////////////
      * when option has multiple arguments, do this
      *
      *     examples are -r4096 1280 or -r 4096 1280
      *
      *     case 'r':
      *         Rows = atoi(arg);
      *         Cols = atoi(argv[argIndex+1]);
      *         argsUsed = 2;
      *         break;
      * ///////////////////////////////////////////////
      * when option does not have an argument, do this
      *
      *     example is -a
      *
      *     case 'a':
      *         PrintActions = 1;
      *         break;
      */
      case 'v': // prints name, then exits
        printName();
        exit(0);
      case 'r': // seeds a pseudo-random number generator
        seed = atoi(arg);
        setMAZESeed(m, seed);
        argsUsed = 1;
        break;
      case 'c': // creates new maze
        rows = atoi(arg);
        columns = atoi(argv[argIndex + 1]);
        oFile = argv[argIndex + 2];
        setBuild(m);
        setMAZESize(m, rows, columns);
        setOutFile(m, oFile);
        createMatrix(m);
        //createNeighbors(m);
        argsUsed = 3;
        break;
      /*case 's': // solve the maze in file III placing solution in file OOO
        setSolve(m);
      */
      case 'd': // draw the created/solved maze in file III
        // number of '----' = (columns * 2) + 1
        setMAZEDashes(m, (getMAZEColumns(m) * 4 + 1));
        // number of '|' = # of columns given
        setMAZEBars(m, getMAZERows(m));
        setDraw(m);
        iFile = arg;
        setInFile(m, iFile);
        argsUsed = 1;
        break;
      default:
        fprintf(stderr, "option %s not understood\n", argv[start]);
        exit(-1);
    }
    argIndex += argsUsed;
  }
}

int main(int argc, char **argv) {
  MAZE * m = newMAZE();
  // process command line arguments
  if (argc == 1) { Fatal("%d arguments!\n", argc - 1); } // not enough arguments
  ProcessOptions(m, argc, argv);
  // build maze
  if (m->build == 1) {
    buildMAZE(m);
  }
  /*if (m->solve == 1) {
    solveMAZE(m);
  }*/
  if (m->draw == 1) {
    drawMAZE(m);
  }



  freeMAZE(m);
  return 0;
}
