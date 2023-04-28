#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <dirent.h>
#include <ctype.h>

#include "headers/args.h"
#include "headers/receiver.h"
#include "headers/sender.h"


int main(int argc, char** argv) {
  srand(time(NULL));

  Arguments args = process_arguments(argc, argv);


  if (args.receiver_mode) {
    receiver_mode(args.file_mode);
  }
  else {
    sender_mode(args.file_mode);
  }

  return 0;
}