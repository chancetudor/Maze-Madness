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

extern void srandom(unsigned int);
extern long int random(void);

static void Fatal(char *fmt, ...) {
  va_list ap;

  fprintf(stderr,"An error occured: ");
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  exit(-1);
}

static int ProcessOptions(int argc, char **argv) {
  int start = 0;
  int argIndex = 1;
  int argsUsed = 0;
  char *arg = 0;
  FILE *inFile = 0;
  FILE *outFile = 0;

  while (argIndex < argc && *argv[argIndex] == '-') {
  /* check if stdin, represented by "-" is an argument */
  /* if so, the end of options has been reached */
    if (argv[argIndex][1] == '\0') { return argIndex; }

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
      *
      * when option has multiple arguments, do this
      *
      *     examples are -r4096 1280 or -r 4096 1280
      *
      *     case 'r':
      *         Rows = atoi(arg);
      *         Cols = atoi(argv[argIndex+1]);
      *         argsUsed = 2;
      *         break;
      *
      *
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
        break;
      case 'c': // creates new maze
        MAZE * m = newMAZE();
        int rows = atoi(arg);
        int columns = atoi(argv[argIndex + 1]);
        outFile = argv[argIndex + 2];
        setMazeSize(m, r, c);
        argsUsed = 2;
        break;
      case 'r': // seeds a pseudo-random number generator
        unsigned int seed = atoi(arg);
        
        argsUsed = 1;
        break;
      case 's': // solve the maze in file III placing solution in file OOO

      case 'd':

      default:
        fprintf(stderr, "option %s not understood\n", argv[start]);
        exit(-1);
    }

    argIndex += argsUsed;
  }

  return argIndex;
}

static void printName(void) {
  printf("Written by Chance Tudor");
  exit(0);
}

extern void setMazeSize(MAZE * array, int r, int c) {
  array->rows = r;
  array->columns = c;
}

int main(int argc, char const *argv[]) {
  int argIndex = 0;
  if (argc == 1) { Fatal("%d arguments!\n", argc - 1); } // not enough arguments
  argIndex = ProcessOptions(argc, argv);
  if (argIndex == argc) { printf("No arguments\n"); }

  struct cell {
    int row;
    int column;
    bool rightWall;
    bool bottomWall;
    int value;
  };

  struct maze {
    int rows;
    int columns;
    CELL * (**array);
  };

  extern MAZE * newMaze(void) {
    MAZE * maze = malloc(sizeof(MAZE));
    assert(maze != 0);
    maze->rows = 0;
    maze->columns = 0;

  }




  return 0;
}
