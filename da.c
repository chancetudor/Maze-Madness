//
//  da.c
//  A dynamic, generic array class with corresponding methods to alter the array
//  project0
//
//  Created by Chance Tudor on 8/30/18.
//  Copyright © 2018 Chance Tudor. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "da.h"

static void doubleArray(DA * items);
static void halveArray(DA * items);
static int getCapacityDA(DA * items);
typedef void (*FM)(void * ptr); // typedef declaration to store a freeMethod function pointer in DA struct
typedef void (*DM)(void * ptr, FILE *fp); // typedef declaration to store a displayMethod function pointer in DA struct

struct da {
  void * (*storage);
  int capacity;
  int size;
  int debugVal;
  FM freeMethod;
  DM displayMethod;
};

// constructor; returns new, initialized dynamic array object
extern DA * newDA(void) {
  DA * array = malloc(sizeof(DA));
  assert(array != 0);
  array->capacity = 1;
  array->storage = malloc(sizeof(void *) * array->capacity);
  assert(array->storage != 0);
  array->size = 0;
  array->debugVal = 0;
  array->freeMethod = 0;
  array->displayMethod = 0;

  return array;
}

// method is passed a function able to display element in generic array slot
extern void setDAdisplay(DA * items, void (*displayMeth)(void * ptr, FILE *fp)) { items->displayMethod = displayMeth; }

// method is passed a function able to free element in generic array slot
extern void setDAfree(DA * items, void (*freeMeth)(void * ptr)) { items->freeMethod = freeMeth; }

// method places given item at the slot named by given index
// array doubles if there is no room for insertion
extern void insertDA(DA * items, int index, void * value) {
  assert(index >= 0 && index <= sizeDA(items));
  if (getCapacityDA(items) == sizeDA(items)) { doubleArray(items); } // FIXME: possible memory leak
  if (index == sizeDA(items)) {
    items->storage[index] = value;
    items->size += 1;
  }
  else {
    for (int i = sizeDA(items); i >= index + 1; i--) { items->storage[i] = items->storage[i - 1]; }
    items->storage[index] = value;
    items->size += 1;
  }
}

// method removes item at the given index
// method shifts each higher element one index down
// if ratio of array size to array capacity < .25, array shrinks by half
extern void * removeDA(DA * items, int index) {
  assert(sizeDA(items) > 0);
  void * val = getDA(items, index);
  memmove(&items->storage[index], &items->storage[index + 1], ((sizeDA(items) - 1) - index) * sizeof(void *)); // shifting left
  items->size -= 1;
  if (sizeDA(items)/((double)getCapacityDA(items)) < .25) {
    halveArray(items);
    if (sizeDA(items)/((double)getCapacityDA(items)) < .25) {
      halveArray(items);
    }
  }

  return val;
}

// method doubles array capacity and reallocates memory for new capacity
static void doubleArray(DA * items) {
  items->capacity = (items->capacity) * 2;
  items->storage = realloc(items->storage, sizeof(void *) * items->capacity); // FIXME: possibly memory leak
  assert(items->storage != 0);
}

// method halves array capacity and reallocates memory for new capacity
static void halveArray(DA * items) {
  items->capacity = (items->capacity) / 2;
  assert(items->capacity >= 1);
  items->storage = realloc(items->storage, sizeof(void *) * items->capacity);
  assert(items->storage != 0);
}

// method moves all items in donor array to recipient array
extern void unionDA(DA * recipient, DA * donor) {
  for (int i = 0; i < sizeDA(donor); i++) { insertDA(recipient, sizeDA(recipient), donor->storage[i]); }
  donor->size = 0;
  donor->capacity = 1;
  free(donor->storage);
  donor->storage = malloc(sizeof(void *) * 1);
}

// method returns the value at the given index
extern void * getDA(DA * items, int index) {
  assert(index >= 0 && index < sizeDA(items));
  return items->storage[index];
}

// method replaces element value at the given index
// method returns null ptr if no value is replaced
extern void * setDA(DA * items, int index, void * value) {
  assert(index >= 0 && index <= sizeDA(items));
  if (index == sizeDA(items)) {
    insertDA(items, index, value);
    return 0;
  }
  else {
    void * oldVal = getDA(items, index);
    items->storage[index] = value;
    return oldVal;
  }
}

// method returns the number of items stored in the array
extern int sizeDA(DA * items) { return items->size; }

// method returns the capacity of array
static int getCapacityDA(DA * items) { return items->capacity; }

// method displays filled region of array and unfilled region of array
// if debugVal > zero, the display method prints number of empty slots (in brackets) immediately after the last element
// if no display method is set, the address of each item is printed
// an empty array with capacity 1 displays as [[1]]
extern void displayDA(DA * items, FILE *fp) {
  /*if ((sizeDA(items) == 0) && (items->debugVal > 0)) {
    fprintf(fp, "[[%d]]", items->capacity);
  }
  else if ((sizeDA(items) == 0) && (items->debugVal == 0)) {
    fprintf(fp, "[]");
  }*/

  if (sizeDA(items) == 0) {
    if (items->debugVal > 0) { // empty array and method should display num. empty indeces
      fprintf(fp, "[[%d]]", items->capacity);
    }
    else { // empty array and method should not display num. empty indeces
      fprintf(fp, "[]");
    }
  }

  else if (items->displayMethod == 0) {
    if (items->debugVal > 0) { // no display method set and method should display num. empty indeces
      fprintf(fp, "[");
      for (int i = 0; i < sizeDA(items); i++) {
        fprintf(fp, "@%p", &items->storage[i]); // no set display method forces addresses of each item to be printed
        if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
      }
      fprintf(fp, ",[%d]]", (getCapacityDA(items) - sizeDA(items)));
    }
    else { // no display method set and method should not display num. empty indeces
      fprintf(fp, "[");
      for (int i = 0; i < sizeDA(items); i++) {
        fprintf(fp, "@%p", &items->storage[i]);
        if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
      }
      fprintf(fp, "]");
    }
  }

  else {
    if (items->debugVal > 0) { // display method set and method should display num. empty indeces
      fprintf(fp, "[");
      for (int i = 0; i < sizeDA(items); i++) {
        items->displayMethod(getDA(items, i), fp);
        if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
      }
      fprintf(fp, ",[%d]]", (getCapacityDA(items) - sizeDA(items)));
    }
    else { // display method set and method should not display num. empty indeces
      fprintf(fp, "[");
      for (int i = 0; i < sizeDA(items); i++) {
        items->displayMethod(getDA(items, i), fp);
        if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
      }
      fprintf(fp, "]");
    }
  }

  /*else if ((items->displayMethod == 0) && (items->debugVal > 0)) {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(items); i++) {
      fprintf(fp, "@%p", &items->storage[i]); // no set display method forces addresses of each item to be printed
      if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
    }
    fprintf(fp, ",[%d]]", (getCapacityDA(items) - sizeDA(items)));
  }
  else if ((items->displayMethod == 0) && (items->debugVal == 0)) {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(items); i++) {
      fprintf(fp, "@%p", &items->storage[i]);
      if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
    }
    fprintf(fp, "]");
  }
  else if ((items->displayMethod != 0) && (items->debugVal > 0)) {
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(items); i++) {
      items->displayMethod(items->storage[i], fp);
      if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
    }
    fprintf(fp, ",[%d]]", (getCapacityDA(items) - sizeDA(items)));
  }
  else if ((items->displayMethod != 0) && (items->debugVal == 0)) { // display method set and method should not display num. empty indeces
    fprintf(fp, "[");
    for (int i = 0; i < sizeDA(items); i++) {
      items->displayMethod(items->storage[i], fp);
      if (i != (sizeDA(items) - 1)) { fprintf(fp, ","); }
    }
    fprintf(fp, "]");
  }*/
}

// method sets an internal flag in the object to the given value
extern int debugDA(DA * items, int level) {
  int prevVal = items->debugVal;
  items->debugVal = level;

  return prevVal;
}

// method frees dynamic array
extern void freeDA(DA * items) {
  if (items->freeMethod != 0) { // individual items are only freed if a freeMethod is set
    for (int i = 0; i < sizeDA(items); i++) { items->freeMethod(getDA(items, i)); }
  }
  free(items->storage);
  free(items);
}
