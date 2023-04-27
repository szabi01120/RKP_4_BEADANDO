#pragma once

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include "macros.h"

#define BMP_MAGIC 0x4d42 //magic number bmp header

//BMP header struktúra
typedef struct
{
  uint32_t size;             // File size in bytes
  uint32_t reserved;         // Not used
  uint32_t offset;           // Offset to image data in bytes from beginning of file
  uint32_t dib_header_size;  // DIB Header size in bytes
  int32_t width_px;          // Width of the image
  int32_t height_px;         // Height of image
  uint16_t num_planes;       // Number of color planes
  uint16_t bits_per_pixel;   // Bits per pixel
  uint32_t compression;      // Compression type
  uint32_t image_size_bytes; // Image size in bytes
  int32_t x_resolution_ppm;  // Pixels per meter
  int32_t y_resolution_ppm;  // Pixels per meter
  uint32_t num_colors;       // Number of colors
  uint32_t important_colors; // Number of colors
  uint32_t color1;           // color 0
  uint32_t color2;           // color 1
} BMPHeader;

void BMPcreator(int* Values, int NumValues) {
  uint32_t cols = (((NumValues + 31) & ~31) >> 3); //oszlopok száma
  uint32_t rows = NumValues;                       //sorok száma
  uint32_t data_size = rows * cols * sizeof(uint8_t); //méret

  uint8_t* data = calloc(data_size, sizeof(*data));  //memória lefoglalása

  BMPHeader header = { //header inicializálása
          .size = 2 + sizeof(header) + data_size,
          .reserved = 0,
          .offset = 2 + sizeof(header),
          .dib_header_size = 40,
          .width_px = NumValues,
          .height_px = rows,
          .num_planes = 1,
          .bits_per_pixel = 1,
          .compression = 0,
          .image_size_bytes = 0,
          .x_resolution_ppm = 0,
          .y_resolution_ppm = 0,
          .num_colors = 0,
          .important_colors = 0,
          .color1 = 0xffffff00,
          .color2 = 0x0 
  };

  for (int i = 0; i < NumValues; i++) { 
    uint32_t target_row = min(max(Values[i] + NumValues / 2, 0), NumValues - 1); 
    bitset(data[cols * target_row + i / 8], 7 - (i % 8) ); 
  }

  char path[100] = "\0";
  getcwd(path, 100);
  strcat(path, "/chart.bmp"); //fájl elérési útja

  FILE* image = fopen(path, "w"); //fájl megnyitása

  uint16_t magic = BMP_MAGIC;
  fwrite(&magic, sizeof(magic), 1, image); //magic number fwrite
  fwrite(&header, sizeof(header), 1, image); //header fwrite
  fwrite(data, data_size, 1, image); //data fwrite
  free(data); //felszabadítás
  fclose(image); //fájl bezárása
  
  chmod(path, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH); //minden felh olvashassa, de csak a tulajdonos tud írni bele
}

