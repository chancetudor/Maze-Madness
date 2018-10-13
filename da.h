//
//  da.h
//  project0
//
//  Created by Chance Tudor on 8/30/18.
//

#ifndef __DA_INCLUDED__
#define __DA_INCLUDED__

#include <stdio.h>

typedef struct da DA;

extern DA *newDA(void);
extern void setDAdisplay(DA * items, void (*)(void *,FILE *));
extern void setDAfree(DA * items, void (*)(void *));
extern void insertDA(DA *items, int index, void *value);
extern void *removeDA(DA *items, int index);
extern void unionDA(DA *recipient, DA *donor);
extern void *getDA(DA *items, int index);
extern void *setDA(DA *items, int index, void *value);
extern int sizeDA(DA *items);
extern void displayDA(DA *items, FILE *fp);
extern int debugDA(DA *items, int level);
extern void freeDA(DA *items);

#define insertDAfront(items,value) insertDA(items,0,value)
#define insertDAback(items,value)  insertDA(items,sizeDA(items),value)

#endif /* da_h */
