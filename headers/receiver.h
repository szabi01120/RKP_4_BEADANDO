#pragma once
#include <stdbool.h>
#include <signal.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "home.h"
#include "array.h"
#include "measurement.h"
#include "bmp.h"
#include "pid.h"

void ReceiveViaFile(int sig) {
	char path[100] = "\0";
	strcat(path, getHomeDir());
	strcat(path, "/Measurement.txt");

	Array measurements = new_array();

	FILE* file = fopen(path, "r");
	char line[30];
	while (fgets(line, 30, file)) {
		int value = atoi(line);
		add_to_array(&measurements, value);
	}
	fclose(file);

	BMPcreator(measurements.items, measurements.size);

	free_array(&measurements);
}

void receive_data_from_client(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t* addr_size) {
	printf("\n Waiting for a message...\n");
	ssize_t bytes = recvfrom(socket, buffer, size, flags, addr, addr_size);
	if (bytes < 0)
	{
		fprintf(stderr, "Receiving error.\n");
		exit(4);
	}
	printf("%ld bytes have been received from the client\n", bytes - 1);
}

void send_data_to_client(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t addr_size)
{
	ssize_t bytes = sendto(socket, buffer, size + 1, flags, addr, addr_size);
	if (bytes <= 0)
	{
		fprintf(stderr, "Sending error.\n");
		exit(5);
	}
	printf("%ld bytes have been sent to the client\n", bytes );
}

void ReceiveViaSocket()
{
	/************************ Declarations **********************/
	int bytes;				   // received/sent bytes
	int err;				   // error code
	int flag;				   // transmission flag
	char on;				   // sockopt option
	unsigned int server_size;  // length of the sockaddr_in server
	unsigned int client_size;  // length of the sockaddr_in client
	struct sockaddr_in server; // address of server
	struct sockaddr_in client; // address of client

	/************************ Initialization ********************/
	on = 1;
	flag = 0;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(3333);
	server_size = sizeof server;
	client_size = sizeof client;

	/************************ Creating socket *******************/
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		fprintf(stderr, "Socket creation error.\n");
		exit(2);
	}
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
	setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

	/************************ Binding socket ********************/
	err = bind(s, (struct sockaddr *)&server, server_size);
	if (err < 0)
	{
		fprintf(stderr, "Binding error.\n");
		exit(3);
	}

	while (1)
	{ // Continuous server operation
		/************************ Receive data **********************/
		int num_values;
		receive_data_from_client(s, &num_values, sizeof(num_values), flag, (struct sockaddr *)&client, &client_size);
		printf("num_values: %d\n", num_values);
		send_data_to_client(s, &num_values, sizeof(num_values), flag, (struct sockaddr *)&client, client_size);

		int values_size = num_values * sizeof(int);
		int *values = malloc(values_size);

		/************************ Sending data **********************/
		receive_data_from_client(s, values, values_size, flag, (struct sockaddr *)&client, &client_size);
		send_data_to_client(s, &values_size, sizeof(values_size), flag, (struct sockaddr *)&client, client_size);

		for (int i = 0; i < num_values; i++)
		{
			printf("%d: %d\n", i, values[i]);
		}

		BMPcreator(values, num_values);
		free(values);
	}
}

void receiver_mode(bool file_mode) {
	printf("A program fogado modban fut\n");

	sigset_t signal_set;

	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGUSR1);

	int sig;

	if (file_mode) {
		while(1) {
			sigwait(&signal_set, &sig);
			ReceiveViaFile(sig);
		} 
	}
	else
	{
		ReceiveViaSocket();
	}
}
