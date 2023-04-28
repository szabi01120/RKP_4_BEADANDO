#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFSIZE 1024 // Max length of buffer
#define PORT_NO 3333 // The port on which the server is listening
int s;               // socket ID

void stop(int sig)
{ // signal handler
    /************************ Closing ***************************/
    close(s);
    printf("\n Server is terminated.\n");
    exit(0);
}


