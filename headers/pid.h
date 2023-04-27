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

  DIR* directory = opendir("/proc");
  if (directory == NULL) {
    perror("Failed to open directory /proc");
    return -1;
  }

  while ((directoryEntry = readdir(directory))) {
    if (directoryEntry->d_type == DT_DIR && isdigit(directoryEntry->d_name[0])) {
      snprintf(path, BUFFER_SIZE, "/proc/%s/status", directoryEntry->d_name);
      process = fopen(path, "r");
        if (process &&
          fgets(line, BUFFER_SIZE, process) && 
          !strncmp(line, "Name:\t", 6) &&
          strstr(line, "chart")) {
          while (fgets(line, BUFFER_SIZE, process) &&
            strncmp(line, "Pid:\t", 5));

          int pid = atoi(line + 5);
          if (pid != getpid())
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