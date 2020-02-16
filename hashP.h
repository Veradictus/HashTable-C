#ifndef hashP_H
#define hashP_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

typedef struct {
  char *word;
  char *defn;
} entry;

/* Replace the following with your implementation declarations, but your structure must be typedef'd to hashTable */
typedef struct {
    int table_maxsize;
    int table_cursize;
    int table_delta_capacity;
    float table_load_threshold;
    entry *entries;
    int resize_count;
} hashTable;
/* end of your implementation declarations */

typedef hashTable *Dictionary;
typedef entry* Entry;

Dictionary create(int initial_capacity, int delta_capacity, float load_threshold);

void destroy (Dictionary table);

int exists(Dictionary table, char *index);

int isIndexEmpty(Dictionary table, int index);

void setValueForIndex(Dictionary table, int i, char *index, char *value);

void insertEntry(Dictionary table, char *index, char *value);

void resize(Dictionary table);

int randomNumber(int min, int max);

/* Convert string into hash-index by adding characters. */
unsigned int hash(char *index);

char *getEntry(Dictionary table, char *index);

void deleteEntry(Dictionary table, char *index);


#endif
