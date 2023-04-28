#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

// Home directory-t adja vissza ide írja a measurement.txt-t
char* getHomeDir() {
  char* homeDir = getenv("HOME");
  if (!homeDir) {
    homeDir = getpwuid(getuid())->pw_dir;
  }
  return homeDir;
}


