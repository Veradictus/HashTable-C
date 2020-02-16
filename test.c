#include "hashP.h"

#define NUMBER_OF_TESTS 12500
#define RANDOM_ENTRIES_PRINT 10

void testHashTable();

int main() {

    testHashTable();

    return 0;
}

/**
 * The following is an extremely large test program that inserts thousands
 * of random words and definitions into a hashTable and observes its behaviour.
 * The testing then removes a random amount of entries through a random number generator.
 * This is used primarily to test the resizing and modularity of the has table.
 */
void testHashTable() {

    Dictionary hashTable = create(5, 5, 0.5); // Create an empty has table.
    char word[32] = { '\0' }, definition[256] = { '\0' }; // Initialize the strings to avoid warnings.

    // Iterate through the NUMBER_OF_TESTS and create words and definitions randomly.
    for (int i = 0; i < NUMBER_OF_TESTS; i++) {
        for (int j = 0; j < sizeof(word) / 4; j++)
            word[j] = randomNumber(0, 2) ? randomNumber(65, 90) : randomNumber(97, 122);

        for (int j = 0; j < sizeof(definition) / 4; j++)
            definition[j] = randomNumber(0, 2) ? randomNumber(65, 90) : randomNumber(97, 122);

        insertEntry(hashTable, word, definition); // Insert an entry

        // Reset the words.
        memset(word, 0, sizeof(word));
        memset(definition, 0, sizeof(definition));
    }

    /**
     * Considering we are dealing with thousands of entries, it would be unwise
     * to print to all of them. Instead, we will be plucking 10 entries randomly
     * from inside the table to see if they are holding properly.
     */

    int i = 0;

    while (i < RANDOM_ENTRIES_PRINT) {
        int randomInt = randomNumber(0, hashTable->table_maxsize - 1);

        if (hashTable->entries[randomInt].word != NULL) {
            printf("%d - %s - %s\n", i, hashTable->entries[randomInt].word, hashTable->entries[randomInt].defn);
            i++;
        }
    }

    // Print out the current status of the hash table.
    printf("Hash table status: %d/%d\n", hashTable->table_cursize, hashTable->table_maxsize);

    printf("---------------\n");
    printf("NOW DELETING SOME RANDOM ELEMENTS\n");
    printf("---------------\n");

    /**
     * Through the use of a random number generator, we throw the dice on a
     * 50/50 chance on whether or not to remove an entry in the hash table.
     * This more-or-less results in ~50% of the entries getting deleted.
     * In order to know if this test was successful, we expect the
     * next print of the hash table's status to be missing around 50% of the entries.
     */
    for (int i = 0; i < hashTable->table_maxsize; i++) {
        int randomInt = randomNumber(0, 100); // Throw the dice.

        if (randomInt > 50 && hashTable->entries[i].word != NULL)
            deleteEntry(hashTable, hashTable->entries[i].word); // Delete an entry.
    }

    i = 0;
    // Print 10 random terms again.
    while (i < RANDOM_ENTRIES_PRINT) {
        int randomInt = randomNumber(0, hashTable->table_maxsize - 1);

        if (hashTable->entries[randomInt].word != NULL) {
            printf("%d - %s - %s\n", i, hashTable->entries[randomInt].word, hashTable->entries[randomInt].defn);
            i++;
        }
    }

    printf("Hash table status: %d/%d\n", hashTable->table_cursize, hashTable->table_maxsize);

    printf("Total number of resizes: %d\n", hashTable->resize_count);

    // Free the hash table.
    destroy(hashTable);

}
