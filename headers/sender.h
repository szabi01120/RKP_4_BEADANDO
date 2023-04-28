#pragma once
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "pid.h"
#include "home.h"
#include "measurement.h"
#include "receiver.h"

void SendViaFile(int* Values, int NumValues) {
	printf("A program kuldo modban fut\n");
	char path[100] = "\0";
	strcat(path, getHomeDir());
	strcat(path, "/Measurement.txt");
	FILE* file = fopen(path, "w");
	for (int i = 0; i < NumValues; i++) {
		fprintf(file, "%d\n", Values[i]);
	}
	fclose(file);
	int pid = FindPID();
	if (pid == -1) {
		printf("A program nem talalt fogado uzemmodban mukodo folyamatot");
		exit(-1);
	}
	kill(pid, SIGUSR1);
}

void send_data_to_server(int s, void* buffer, int data_size, int flag, struct sockaddr* server, unsigned int server_size)
{
	int bytes = sendto(s, buffer, data_size, flag, server, server_size);
	if (bytes <= 0)
	{
		fprintf(stderr, "Sending error.\n");
		exit(3);
	}
	printf(" %i bytes have been sent to server.\n", bytes - 1);
}

void receive_data_from_server(int s, void* buffer, int data_size, int flag, struct sockaddr* server, unsigned int server_size)
{
	int bytes = recvfrom(s, buffer, data_size, flag, server, &server_size);
	if (bytes < 0)
	{
		fprintf(stderr, "Receiving error.\n");
		exit(4);
	}
}

void SendViaSocket(int *Values, int NumValues)
{ // Server's IPv4 address is needed as command line argument

	/************************ Declarations **********************/
	int s;					   // socket ID
	int flag;				   // transmission flag
	char on;				   // sockopt option
	unsigned int server_size;  // length of the sockaddr_in server
	struct sockaddr_in server; // address of server

	/************************ Initialization ********************/
	on = 1;
	flag = 0;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(3333);
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
	send_data_to_server(s, (void *)&NumValues, sizeof(NumValues), flag, (struct sockaddr *)&server, server_size);

	int receivedNumValues = 0;
	receive_data_from_server(s, (void *)&receivedNumValues, sizeof(receivedNumValues), flag, (struct sockaddr *)&server, server_size);
	if (NumValues != receivedNumValues)
	{
		fprintf(stderr, "Epick hiba van a rendszerben xdddd.\n");
		exit(5);
	}

	int values_size = sizeof(*Values) * NumValues;
	send_data_to_server(s, (void *)&Values, values_size, flag, (struct sockaddr *)&server, server_size);

	int received_values_size = 0;
	receive_data_from_server(s, (void *)&received_values_size, sizeof(received_values_size), flag, (struct sockaddr *)&server, server_size);
	if (values_size != received_values_size)
	{
		fprintf(stderr, "Még epickebb hiba van a rendszerben xdddd.\n");
		exit(6);
	}

	/************************ Closing ***************************/
	close(s);
}

void sender_mode(bool file_mode) {
	int* values;
	int num_values = Measurement(&values);
	if (file_mode) {
		SendViaFile(values, num_values);
	} else  {
		for(int i =0; i < num_values; i++) {
			printf("%d: %d\n", i, values[i]);
		}
		SendViaSocket(values, num_values);
	}
	free(values);	
}
