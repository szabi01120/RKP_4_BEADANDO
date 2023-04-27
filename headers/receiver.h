#pragma once
#include <stdbool.h>
#include <signal.h>

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
}
