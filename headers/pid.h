#pragma once
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>

#define BUFFER_SIZE 300

int FindPID() {
  struct dirent* directoryEntry;
  FILE* process;
  char path[BUFFER_SIZE];
  char line[BUFFER_SIZE];

  DIR* directory = opendir("/proc");  // /proc konyvtar megnyitasa
  if (directory == NULL) {            // Ha nem sikerult megnyitni akkor hibauzenet, majd -1 return
    perror("Failed to open directory /proc");
    return -1;
  }

  while ((directoryEntry = readdir(directory))) { // /proc konyvtar bejarasa
    if (directoryEntry->d_type == DT_DIR && isdigit(directoryEntry->d_name[0])) { // Ha a bejart elem egy konyvtar es szamjegy az elso char
      snprintf(path, BUFFER_SIZE, "/proc/%s/status", directoryEntry->d_name); // status fajl eleresi utvonala
      process = fopen(path, "r"); 
        if (process && 
          fgets(line, BUFFER_SIZE, process) && 
          !strncmp(line, "Name:\t", 6) &&
          strstr(line, "chart")) { // Ha a status fajlban a Name: es kozotte a chart szerepel
          while (fgets(line, BUFFER_SIZE, process) &&
            strncmp(line, "Pid:\t", 5)); 

          int pid = atoi(line + 5); 
          if (pid != getpid()) //ha a pid nem onmaga, akkor bezarunk mindent es visszaadjuk a pid-et
          {
            fclose(process);
            closedir(directory);
            return pid;
          }
        }
      fclose(process);
    }
  }
  closedir(directory);

  return -1;
}