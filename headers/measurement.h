#pragma once
#include "macros.h"

int Measurement(int** Values) //Forras: Varga Imre oldala (idő kezelés)
{
  time_t now;
  struct tm* local_now;
  time(&now);
  local_now = localtime(&now);
  int seconds_now = (*local_now).tm_sec; //jelenlegi masodperc
  int minutes_now = (*local_now).tm_min; //jelenlegi perc
  int measurement_count = minutes_now % 15 * 60 + seconds_now; //utolo negyed ora ota eltelt masodpercek szama
  measurement_count = 10;// max(measurement_count, 100);

  *Values = malloc(sizeof(**Values) * measurement_count); // dinamikus tomb letrehozasa
  int* array = *Values; //dinamikus tombre mutato pointer
  array[0] = 0; //az elso ertek 0

  for (int i = 1; i < measurement_count; i++) {
    float random_number = rand() / (float)RAND_MAX;
    if (random_number < 0.428571f) { //42.8571% esely ekkor + 1
      array[i] = array[i - 1] + 1;
    }
    else if (random_number < 0.428571f + 11.0f / 31.0f) { // 11/31 esely ekkor - 1 
      array[i] = array[i - 1] - 1;
    }
    else {
      array[i] = array[i - 1];
    }
  }

  return measurement_count;
}