#pragma once

#include <stdbool.h>
#include <omp.h>

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

void print_version() { //PÁRHUZAMOS PROGRAMOZÁS OPENMP-VEL || FORRÁS: VARGA IMRE OLDALA
	printf("------------------------------------\n");
	#pragma omp parallel sections //OMP SECTION DEKLARÁLÁS || /////*****8.FELADAT*****////
	{
		#pragma omp section 
		{
			printf("Verzio: %s\n", VERSION);
		}
		#pragma omp section 
		{
			printf("Fejleszto: %s\n", DEVELOPER);
		}
		#pragma omp section 
		{
			printf("Keszult: %s\n", DATE);
		}
	}	
	printf("------------------------------------\n"); //PÁRHUZAMOSAN FUTNAK A SECTION-ÖK, AMELYIK HAMARABB HAJTÓDIK VÉGRE, AZ LESZ HAMARABB KIÍRVA
}

typedef struct Arguments {
	bool receiver_mode;
	bool file_mode;
} Arguments;

Arguments process_arguments(int argc, char** argv) {
	if (!strstr(argv[0], "chart.exe")) { //ha nem chart.exe a név akk leáll
		printf("Hibas fajlnev! A program leall!\n");
		exit(1);
	}

	Arguments args = { true, true };
	if (argc == 1) return args; //ha nincs megadva argv, csak a program neve akk visszatér

	if (strstr(argv[1], ARG_HELP)) { //--help argumentum
		print_help();
		exit(0);
	}
	else if (strstr(argv[1], ARG_VERSION)) { //--version argumentum
		print_version();
		exit(0);
	}
	else {
		for (int i = 1; i < argc; i++) {
			if (strstr(argv[i], ARG_SEND)) { //ha valamelyik arg send-del egyenlő akk a receiver_mode false lesz
				args.receiver_mode = false;
			}
			else if (strstr(argv[i], ARG_SOCKET)) { //ha valamelyik arg socket-tel egyenlő akk a file_mode false lesz
				args.file_mode = false;
			}
			else if (!strstr(argv[i], ARG_RECEIVE) && !strstr(argv[i], ARG_FILE)) { //ha nem egyezik meg egyik arg sem akk hibát dob
				printf("Ismeretlen argumentum: \'%s\'\n", argv[i]);
				exit(1);
			}
		}
	}
	return args;
}
