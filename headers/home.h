#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

char* getHomeDir() {
  char* homeDir = getenv("HOME");
  if (!homeDir) {
    homeDir = getpwuid(getuid())->pw_dir;
  }
  return homeDir;
}


