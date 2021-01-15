//=================================================================
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

/*
 Student Name:
 Date: 

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {

   //allocate memory on the heap for the new entry along with its components
   HashTableEntry* newSpot = (HashTableEntry*)malloc(sizeof(HashTableEntry));
   newSpot->key = key;
   newSpot->value = value;
   newSpot->next = NULL;
        
   return newSpot; 
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {

  int index = hashTable->hash(key);
  HashTableEntry* spot = hashTable->buckets[index];
   
  for(spot = spot; spot != NULL; spot = spot->next)
  {
      if(spot->key == key)
   break;
  }
  return spot;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
  
  //initialize the number of buckets
  int numBuck = 0; 
  //get buckets from hashTable
  unsigned int totalBuckets = hashTable->num_buckets; 

 //loop through all the buckets to find the non null entries, delete their contents and set a temporary entries to those entries.
  for(numBuck = 0; numBuck < totalBuckets; numBuck++){ 
       HashTableEntry* spot = hashTable->buckets[numBuck];      
       while(spot != NULL){
         free(spot->value); 
         HashTableEntry* temp = spot->next;
         free(spot);
         spot = temp;
       }
   }
   //free the buckets and the hash table 
   free(hashTable->buckets);
   free(hashTable);           
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {

   //retrieves the index of the item to the key mentioned
   int index = hashTable->hash(key); 
   //creates an entry which will go to a bucket
   HashTableEntry* spot = hashTable->buckets[index]; 
   //creates a trailing entry
   HashTableEntry* spot_trail = NULL; 

  
  //loop to find a key parameter that matches the entry key
   for(spot = spot; spot != NULL; spot = spot->next)
   {
      if(key == spot->key)
      {
         break; //break to stop the search when the key is found
      }
      spot_trail = spot; //if key not found, trail takes spot postion.    
   }
   
   //creates a second entry  
   HashTableEntry* spot2; 
   if(spot == NULL)
   {
      spot2 = createHashTableEntry(key, value); //initialization of the second entry
      
      //set the contents to spot2 if the trail is null, otherwise the next trail will become spot2
      if(spot_trail == NULL)
      {
        hashTable->buckets[index] = spot2;
      }
      else{
        spot_trail->next = spot2;
      }
      return NULL;
   }
   
   //returns the value of spot with a temporary and free it.
   void* temp = spot->value;
   spot->value = value;
   return temp;
}

void* getItem(HashTable* hashTable, unsigned int key) {

  
   HashTableEntry* x = findItem(hashTable, key); //gets an entry
    
   //checks if entry is null
   if(x == NULL) 
   {
        return NULL;
   }
    
   //returns the value of the entry
   return x->value; 

}

void* removeItem(HashTable* hashTable, unsigned int key) {

   int index = hashTable->hash(key);                 //creates an index
   HashTableEntry* spot = hashTable->buckets[index]; //creates an entry
   HashTableEntry* spot_trail = NULL;                //creates a trailing entry

   //loop to find a key parameter that matches the entry key
   for(spot = spot; spot != NULL; spot = spot->next)
   {
      if(key == spot->key)
      {
         break; //break to stop the search when the key is found
      }
      spot_trail = spot; //if key not found, trail takes spot postion. 
   }

   //checks if spot is null
   if(spot == NULL)
   {
      return NULL;
   }

   //checks the trailing entry if spot in non null
   else
   { 
     //the contents of the index equals the next entry if the trailing entry is found to be null
     if(spot_trail == NULL)
     {
        hashTable->buckets[index] = spot->next;
     }
     //if not, we can set the next entry of trail equals to the next entry of spot
     else
     {
        spot_trail->next = spot->next;
     }
     
   }
   
   //returns the value of spot with a temporary and free it.
   void* temp = spot->value;
   free(spot);
   return temp;
}

void deleteItem(HashTable* hashTable, unsigned int key) {
 
   //calls removeItem which is the same as deleteItem
   void* x = removeItem(hashTable, key);
   
   //deallocation of the variable when the value is null
   if(x != NULL)
   {
      free(x);
   }
}
 

/*
void deleteItem(HashTable* hashTable, unsigned int key) {
    unsigned int ndx = hashTable->hash(key);  // gets bucket index from hash function
    HashTableEntry* item = findItem(hashTable, key);    // gets item's pointer from find function
    if (item == NULL) {                                         // if item's pointer == NULL / item not in hash table, return null
        return;
    } else {
        if (item->next == NULL) {                               // if hash table bucket at pointer is a linked list of length 1, set it to NULL and return
            hashTable->buckets[ndx] = NULL;
            return;
        }
        if (hashTable->buckets[ndx]->key == key) {      // if the next item is the one we want, free it and return 
            hashTable->buckets[ndx] = item->next;
            free(item->value);
            free(item);
            return;
        }
        while (item->next != NULL) {                            // while the next item in the linked list is not NULL
            if ((item->next)->key == key) {                 // if the next item is the one we want
                if ((item->next)->next == NULL) {           // if the item we want's next == NULL, free it and return 
                    item->next = NULL;
                    free(item->value);
                    free(item);
                    return;
                } else {                                                // if it's not == NULL, cycle to its next, free and return
                    item->next = item->next->next;
                    free(item->value);
                    free(item);
                    return;
                }
            } else {                                                    // setting item = it's next
                item = item->next;
            }
        }
    }
    return;
}
*/