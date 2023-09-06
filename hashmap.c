#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

    long hashKey = hash(key,map->capacity);

    while(map->buckets[hashKey] != NULL){
        hashKey = (hashKey + 1) % map->capacity;
    }

    Pair* newPair = (Pair*)malloc(sizeof(Pair));
    if(newPair == NULL) return;

    newPair->key = key;
    newPair->value = value;

    map->buckets[hashKey] = newPair;

    map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; // No borrar (para propósitos de prueba)

    Pair **tempBucket = map->buckets;
    long tempCapacity = map->capacity;

    map->capacity *= 2;
    map->size = 0;

    map->buckets = (Pair **)realloc(map->buckets, map->capacity * sizeof(Pair *));

    if (map->buckets == NULL) {
        return;
    }

  
    for (long i = 0; i < tempCapacity; i++) {
        if(tempBucket[i] != NULL){
            insertMap(map, tempBucket[i]->key, tempBucket[i]->value);
        }
    }

    free(tempBucket);
}



HashMap * createMap(long capacity) {

    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
  
    map->capacity = capacity;
  
    map->size = 0;

    map->current = -1;

    map->buckets = (Pair**)malloc(capacity*sizeof(Pair*));

    return map;
}

void eraseMap(HashMap * map,  char * key) {    

    long newKey = hash(key,map->capacity);

    while(!is_equal(key, map->buckets[newKey]->key)){
        newKey = (newKey + 1) % map->capacity;
        if(map->buckets[newKey] == NULL) return;
    }

     map->buckets[newKey]->key = NULL;

    map->size--;
}

Pair * searchMap(HashMap * map,  char * key) {   

    long keyBuscar = hash(key,map->capacity);

    while(!is_equal(key, map->buckets[keyBuscar]->key)){
        keyBuscar = (keyBuscar + 1) % map->capacity;
        if(map->buckets[keyBuscar] == NULL) return NULL;
    }

    map->current = keyBuscar;
  
    return map->buckets[keyBuscar];
}

Pair * firstMap(HashMap * map) {
    long i = 0;

    while (i < map->capacity) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
        i++;
    }

    return NULL;
}


Pair * nextMap(HashMap * map) {

    long keyActual = (map->current + 1) % map->capacity;

    map->current = keyActual;

    if(map->buckets[keyActual] != NULL){
        return map->buckets[keyActual];
    }
    else{

        while(map->buckets[keyActual] == NULL){
          keyActual = (keyActual + 1) % map->capacity;

          if(map->buckets[keyActual] == firstMap(map)) return NULL;
        }
    }

    map->current = keyActual;
  
    return map->buckets[keyActual];
}
