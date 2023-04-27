#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <dirent.h>
#include <ctype.h>

/*>>> --version PAR.ARG <<<*/
#define VERSION "1.0"
#define DEVELOPER "Hajnal Szabolcs"
#define DATE "2023.04.23"

/*### UZEMMOD ###*/
#define ARG_SEND "-send"
#define ARG_RECEIVE "-receive"

/*### KOMMUNIKACIO MODJA ###*/
#define ARG_FILE "-file"
#define ARG_SOCKET "-socket"

/*### HELP ###*/
#define ARG_HELP "--help"
#define ARG_VERSION "--version"

#define max(a, b) (((a) > (b)) ? (a) : (b))

/*4.FELADAT*/
#define MAX_BUF_SIZE 1024

void help()
{
  printf("------------------------------------\n");
  printf("HELP PARANCS => HASZNALAT:\n");
  printf("--version => verzioszam, fejleszto neve, fejlesztes datuma\n");
  printf("--help => parancssugo felulet\n");
  printf("-send => kuldo mod (ALAPERTELMEZETT)\n"); // ALAPERTELMEZETT MOD, HA NEM AD MEG PARANCSSORI ARGUMOT AMI ERRE UTAL!
  printf("-receive => fogado mod\n\n");
  printf("KOMMUNIKACIO MODJA:\n");
  printf("-file => file mod (ALAPERTELMEZETT)\n"); // ALAPERTELMEZETT MOD, HA NEM AD MEG PARANCSSORI ARGUMOT AMI ERRE UTAL!
  printf("-socket => socket mod \n\n");
  printf("------------------------------------\n");
}

// 2.FELADAT
int Measurement(int** Values) //Forras: Varga Imre oldala
{
  time_t now;
  struct tm* local_now;
  time(&now);
  local_now = localtime(&now);
  int seconds_now = (*local_now).tm_sec; //jelenlegi masodperc
  int minutes_now = (*local_now).tm_min; //jelenlegi perc
  int measurement_count = minutes_now % 15 * 60 + seconds_now; //utolo negyed ora ota eltelt masodpercek szama
  measurement_count = max(measurement_count, 100);

  *Values = malloc(sizeof(**Values) * measurement_count); // dinamikus tomb letrehozasa
  int* array = *Values;
  array[0] = 0;

  for (int i = 1; i < measurement_count; i++)
  {
    float random_number = rand() / (float)RAND_MAX;
    if (random_number < 0.428571f) //42.8571% esely ekkor + 1
    {
      array[i] = array[i - 1] + 1;
    }
    else if (random_number < 0.428571f + 11.0f / 31.0f) // 11/31 esely ekkor - 1
    {
      array[i] = array[i - 1] - 1;
    }
    else
    {
      array[i] = array[i - 1];
    }
  }

  return measurement_count;
}

typedef struct
{
  uint32_t size;             // File size in bytes
  uint16_t reserved1;        // Not used
  uint16_t reserved2;        // Not used
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
  uint32_t important_colors;             // Number of colors
  uint32_t color1;           // color 0
  uint32_t color2;           // color 1
} BMPHeader;

//3.feladat

void set_pixel(int x, int y, int value, uint32_t* pixels, int line_width)
{
  int index = y * line_width + x / 32;
  int bits_to_shift = x % 32;
  if (value)
    pixels[index] |= (1 << bits_to_shift);
  else
    pixels[index] &= ~(1 << bits_to_shift);
}

void BMPcreator(int* Values, int NumValues) { //NumValues = measurement_count = imagewidth

  int line_width = NumValues / 32 + 1;
  int pixels_length = line_width * 21;
  uint32_t* pixels = calloc(pixels_length * sizeof(*pixels), 1);
   
  //set_pixel(0, 0, 1, pixels, line_width);
  pixels[0] = (uint32_t)0b10000000000000000000000000000001;

  BMPHeader header = {
      .size = pixels_length * 4 + sizeof(header) + 2,
      .reserved1 = 0,
      .reserved2 = 0,
      .offset = sizeof(BMPHeader)+2,
      .dib_header_size = 40,
      .width_px = NumValues,
      .height_px = 21,
      .num_planes = 1,
      .bits_per_pixel = 1,
      .compression = 0,
      .image_size_bytes = 0,
      .x_resolution_ppm = 0,
      .y_resolution_ppm = 0,
      .num_colors = 0,
      .important_colors = 0,
      .color1 = 0xffffff,
      .color2 = 0 };

  printf("size: %d\n", header.size);

  FILE* file = fopen("chart.bmp", "wb");
  uint16_t magic = 0x4d42;

  fwrite(&magic, sizeof(magic), 1, file);
  fwrite(&header, sizeof(header), 1, file);
  fwrite(pixels, pixels_length * sizeof(*pixels), 1, file);
  free(pixels);
  fclose(file);
}

//3.feladat END

//4.FELADAT

int FindPID() {
  DIR *dirp;
    struct dirent *dp;
    FILE *fp;
    char path[MAX_BUF_SIZE];
    char line[MAX_BUF_SIZE];
    int pid = -1;

    dirp = opendir("/proc");
    if (dirp == NULL) {
        perror("Failed to open directory /proc");
        return -1;
    }

    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_type == DT_DIR && isdigit(dp->d_name[0])) {
            snprintf(path, MAX_BUF_SIZE, "/proc/%s/status", dp->d_name);
            fp = fopen(path, "r");
            if (fp == NULL) {
                continue;
            }
            while (fgets(line, MAX_BUF_SIZE, fp) != NULL) {
                if (strncmp(line, "Name:\t", 6) == 0 && strstr(line, "chart") != NULL) {
                    while (fgets(line, MAX_BUF_SIZE, fp) != NULL) {
                        if (strncmp(line, "Pid:\t", 5) == 0) {
                            pid = atoi(line + 5);
                            printf("PID: %d\n", pid);
                            break;
                        }
                    }
                    break;
                }
            }
            fclose(fp);
        }
        if (pid != -1) {
            break;
        }
    }

    closedir(dirp);
    return pid;
}

void kuldo_mod(bool file_mod)
{
  printf("A program kuldo modban uzemel! A kommunikacio modja: %s\n", file_mod ? "file" : "socket");
  int* tomb;
  int n = Measurement(&tomb);
  printf("n: %d\n", n);
  BMPcreator(tomb, 128);
  printf("PID: %d\n", FindPID());
}

void fogado_mod(bool file_mod) {
  printf("A program fogado modban uzemel! A kommunikacio modja: %s\n", file_mod ? "file" : "socket");
}

int main(int argc, char* argv[]) {
  srand(time(NULL));

  if (strstr(argv[0], "chart.exe") == 0) {
    printf("Hibas fajlnev! A program leall!\n");
    exit(1);
  }

  bool fogado_mod_bool = true;
  bool file_mod_bool = true;

  if (argc == 1) {
    help();
  }
  else {
    for (int i = 1; i < argc; i++) {
      if (strstr(argv[i], ARG_SEND)) {
        fogado_mod_bool = false;
      }
      else if (strstr(argv[i], ARG_SOCKET)) {
        file_mod_bool = false;
      }
      else if (!strstr(argv[i], ARG_RECEIVE) && !strstr(argv[1], ARG_FILE)) {
        printf("Ismeretlen kapcsolo: \'%s\'\n", argv[i]);
      }
    }
  }

  if (fogado_mod_bool) {
    fogado_mod(file_mod_bool);
  }
  else {
    kuldo_mod(file_mod_bool);
  }
}