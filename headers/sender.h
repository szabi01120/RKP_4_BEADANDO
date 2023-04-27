#pragma once
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
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

void sender_mode(bool file_mode) {
	int* values;
	int num_values = Measurement(&values);
	if (file_mode) {
		SendViaFile(values, num_values);
	}
	free(values);	
}
