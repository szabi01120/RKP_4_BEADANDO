#pragma once
#include <stdlib.h>
#include <string.h>

typedef struct Array {
  int* items;
  int size;
  int _allocated;
} Array;

Array new_array() {
  Array array = {
    0, 0, 16
  };
  array.items = malloc(array._allocated * sizeof(*array.items));
  return array;
}

void add_to_array(Array* array, int value) {
  if (array->size + 1 == array->_allocated) {
    int* items_bigger = malloc(array->_allocated * 2 * sizeof(*array->items));
    memcpy(items_bigger, array->items, array->_allocated * sizeof(*array->items));
    free(array->items);
    array->items = items_bigger;
    array->_allocated *= 2;
  }
  array->items[array->size] = value;
  array->size++;
}

void free_array(Array* array) {
  free(array->items);
}

