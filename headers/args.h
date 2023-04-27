#pragma once

#include <stdbool.h>



/*>>> --version PAR.ARG <<<*/
#define VERSION "1.0"
#define DEVELOPER "Hajnal Szabolcs"
#define DATE "2023.04.23"

/*### UZEMMOD ###*/
#define ARG_SEND "-send"
#define ARG_RECEIVE "-receive"

/*### KOMMUNIKACIO MODJA ###*/
#define ARG_FILE "-file"
#define ARG_SOCKET "-socket"

/*### HELP ###*/
#define ARG_HELP "--help"
#define ARG_VERSION "--version"

void print_help()
{
	printf("------------------------------------\n");
	printf("Hasznalat:\n");
	printf("%s => verzioszam, fejleszto neve, fejlesztes datuma\n", ARG_VERSION);
	printf("%s => sugo\n\n", ARG_HELP);

	printf("Uzemmod:\n");
	printf("%s => kuldo mod (alapertelmezett)\n", ARG_SEND); // ALAPERTELMEZETT MOD, HA NEM AD MEG PARANCSSORI ARGUMOT AMI ERRE UTAL!
	printf("%s => fogado mod\n\n", ARG_RECEIVE);

	printf("Kommunikacios mod:\n");
	printf("%s => fajl mod (alapertelmezett)\n", ARG_FILE); // ALAPERTELMEZETT MOD, HA NEM AD MEG PARANCSSORI ARGUMOT AMI ERRE UTAL!
	printf("%s => socket mod \n", ARG_VERSION);
	printf("------------------------------------\n");
}

void print_version() {
	printf("------------------------------------\n");
	printf("Verzio: %s\n", VERSION);
	printf("Fejleszto: %s\n", DEVELOPER);
	printf("Keszult: %s\n", DATE);
	printf("------------------------------------\n");
}

typedef struct Arguments {
	bool receiver_mode;
	bool file_mode;
} Arguments;

Arguments process_arguments(int argc, char** argv) {
	if (!strstr(argv[0], "chart.exe")) {
		printf("Hibas fajlnev! A program leall!\n");
		exit(1);
	}

	Arguments args = { true, true };
	if (argc == 1) return args;

	if (strstr(argv[1], ARG_HELP)) {
		print_help();
		exit(0);

	}
	else if (strstr(argv[1], ARG_VERSION)) {
		print_version();
		exit(0);
	}
	else {
		for (int i = 1; i < argc; i++) {
			if (strstr(argv[i], ARG_SEND)) {
				args.receiver_mode = false;
			}
			else if (strstr(argv[i], ARG_SOCKET)) {
				args.file_mode = false;
			}
			else if (!strstr(argv[i], ARG_RECEIVE) && !strstr(argv[i], ARG_FILE)) {
				printf("Ismeretlen argumentum: \'%s\'\n", argv[i]);
				exit(1);
			}
		}
	}
	return args;
}
