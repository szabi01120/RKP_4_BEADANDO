#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024 // Max length of buffer
#define PORT_NO 2222 // The port on which the server is listening

void send_data_to_server(int s, void* buffer, int data_size, int flag, struct sockaddr_in* server, unsigned int server_size)) {
    int bytes = sendto(s, buffer, data_size, flag, (struct sockaddr *)&server, server_size);
    if (bytes <= 0)
    {
        fprintf(stderr, "Sending error.\n");
        exit(3);
    }
    printf(" %i bytes have been sent to server.\n", bytes - 1);
}

void receive_data_from_server(int s, void* buffer, int data_size, int flag, struct sockaddr_in* server, unsigned int server_size)) {
    int bytes = recvfrom(s, buffer, data_size, flag, (struct sockaddr *)&server, &server_size);
    if (bytes < 0)
    {
        fprintf(stderr, "Receiving error.\n");
        exit(4);
    }
    printf(" Server's (%s:%d) acknowledgement:\n  %s\n",
           inet_ntoa(server.sin_addr), ntohs(server.sin_port), buffer);

    
}

void SendViaSocket(int *Values, int NumValues)
{ // Server's IPv4 address is needed as command line argument

    /************************ Declarations **********************/
    int s;                     // socket ID
    int flag;                  // transmission flag
    char on;                   // sockopt option
    unsigned int server_size;  // length of the sockaddr_in server
    struct sockaddr_in server; // address of server

    /************************ Initialization ********************/
    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;

    /************************ Creating socket *******************/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "Socket creation error.\n");
        exit(2);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    /************************ Sending data **********************/
    printf(" Message to send: ");
    memcpy(buffer, Values, NumValues * sizeof(*Values));
    send_data_to_server(s, (void *)&NumValues, sizeof(NumValues), flag, server, server_size);

    int receivedNumValues = 0;
    receive_data_from_server(s, (void *)&receivedNumValues, sizeof(receivedNumValues), flag, server, server_size);
    if (NumValues != receivedNumValues) {
        fprintf(stderr, "Epick hiba van a rendszerben xdddd.\n");
        exit(5);
    }

    int values_size = sizeof(*Values) * NumValues;
    send_data_to_server(s, (void *)&Values, values_size, flag, server, server_size);

    int received_values_size = 0;
    receive_data_from_server(s, (void *)&received_values_size, sizeof(received_values_size), flag, server, server_size);
    if (values_size != received_values_size)
    {
        fprintf(stderr, "Még epickebb hiba van a rendszerben xdddd.\n");
        exit(6);
    }

    /************************ Closing ***************************/
    close(s);
    return EXIT_SUCCESS;
}
