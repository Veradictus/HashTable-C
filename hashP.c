#include "hashP.h"

Dictionary create(int initial_capacity, int delta_capacity, float load_threshold) {
    Dictionary table = malloc(sizeof(hashTable)); //Initialize the Dictionary

    if (table == NULL) {
        //fprintf("HASHTABLE IS NULL");
        return NULL;
    }

    /* Initialize the hashTable with the given parameters */
    table->table_maxsize = initial_capacity;
    table->table_cursize = 0;
    table->table_delta_capacity = 0;
    table->table_load_threshold = load_threshold;
    table->entries = calloc((size_t) table->table_maxsize, sizeof(entry));
    table->resize_count = 0;
    // Create memory allocation for the entries.

    srand(time(NULL));

    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    // Prevent any unforseen issues by assigning nulls to each index.
    for (int i = 0; i < table->table_maxsize; i++) {
        table->entries[i].word = NULL;
        table->entries[i].defn = NULL;
    }

    return table;
}

void destroy(Dictionary table) {
    if (!table) return;

    /**
     * The hashTable is freed sequentially.
     * We first free the pointers of the entries: `word` and `defn`
     * After which, we free the `entries` and the table itself.
     */

    for (int i = 0; i < table->table_maxsize; i++) {
        free(table->entries[i].word);
        free(table->entries[i].defn);
    }

    free(table->entries);
    free(table);
}

/* Function to check whether or not a value exists in the hashTable */
int exists(Dictionary table, char *index) {
    unsigned int keyHash = hash(index) % table->table_maxsize;

    for (int i = keyHash; !isIndexEmpty(table, i); i = (i + 1) % table->table_maxsize)
        if (strcmp(table->entries[i].word, index) == 0)
            return 1;

    return 0;
}

/* Function checks if the given index is empty/NULL */
int isIndexEmpty(Dictionary table, int index) {
    return table->entries[index].word == NULL ? 1 : 0;
}

/**
 * The purpose of this function is to clean up the code a bit.
 * It sets the value of the `word` and `defn` in the given entry index
 * to the values of `index` and `value`.
 */
void setEntry(Dictionary table, int i, char *index, char *value) {
    table->entries[i].word = index;
    table->entries[i].defn = value;
}

/**
 * Code is a derivative of my final lab in CMPT200.
 */
void insertEntry(Dictionary table, char *index, char *value) {
    unsigned int keyHash = hash(index) % table->table_maxsize;

    /**
     * Check if the array is half empty in accordance to the
     * load_threshold value in hashTable.
     */

    float diff = (float) table->table_cursize / (float) table->table_maxsize;

    if (diff >= table->table_load_threshold)
        resize(table);

    int i;

    /**
     * The following is a linear probing open-addressing implementation.
     * It is done by iterating through the entries in the table,
     * and inserting the index and value if a spot is empty.
     */

    for (i = keyHash; !isIndexEmpty(table, i); i = (i + 1) % table->table_maxsize) {
        if (strcmp(table->entries[i].word, index) == 0) {
            strcpy(table->entries[i].defn, value);
            return;
        }
    }

    char *insertWord = calloc(strlen(index) + 1, sizeof(char));
    char *insertDefinition = calloc(strlen(value) + 1, sizeof(char));

    strcpy(insertWord, index);
    strcpy(insertDefinition, value);

    setEntry(table, i, insertWord, insertDefinition);

    table->table_cursize++;
}

void resize(Dictionary table) {
    /**
     * This function recreates the hashTable by temporarily storing the old
     * entries in a pointer and re-allocating the table->entries.
     * We then iterate through the oldEntries and re-insert them into
     * the new hashTable->entries.
     */

    entry *oldEntries = table->entries;
    int oldSize = table->table_maxsize;

    table->table_maxsize = table->table_maxsize * 2; // update size
    table->entries = calloc((size_t) table->table_maxsize, sizeof(entry));
    table->table_cursize = 0;
    table->resize_count++;

    // Set all values to NULL.
    for (int i = 0; i < table->table_maxsize; i++) {
        table->entries[i].word = NULL;
        table->entries[i].defn = NULL;
    }

    // Re-insert the old entries.
    for (int i = 0; i < oldSize; i++) {
        if (oldEntries[i].word != NULL) {
            insertEntry(table, oldEntries[i].word, oldEntries[i].defn);

            free(oldEntries[i].word);
            free(oldEntries[i].defn);
        }
    }

    if (oldEntries)
        free(oldEntries); // Remove the temporary pointer.
}

// Generates a random number using C rand()
int randomNumber(int min, int max) {
    return (rand() % abs(max - min)) + min;
}

// Function used to generate keyHash for an entry.
unsigned int hash(char *index) {
    unsigned int keyHash = 0;

    for (int i = 0; i < strlen(index); i++) // 5-bit left cyclic shift
        if (index[i] != '\0')
            keyHash = ((keyHash << 5 | keyHash >> 27) + index[i]) & 0xFFFFFFFF;

    return keyHash;
}

// Returns the value of the `index` entry.
char *getEntry(Dictionary table, char *index) {
    unsigned int keyHash = hash(index) % table->table_maxsize;

    for (int i = keyHash; !isIndexEmpty(table, i); i = (i + 1) % table->table_maxsize)
        if (strcmp(table->entries[i].word, index) == 0)
            return table->entries[i].defn;

    return NULL;
}

void deleteEntry(Dictionary table, char *index) {
    unsigned int keyHash = hash(index) % table->table_maxsize;

    /**
     * This uses the same algorithm as the insertEntry
     * and does the opposite of it.
     */

    for (int i = keyHash; !isIndexEmpty(table, i); i = (i + 1) % table->table_maxsize) {
        if (table->entries[i].word == index) {
            free(table->entries[i].word);
            free(table->entries[i].defn);

            setEntry(table, i, NULL, NULL);
        }
    }

    table->table_cursize--;
}
