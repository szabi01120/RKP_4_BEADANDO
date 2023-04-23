#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*>>> --version PAR.ARG <<<*/
#define version "1.0"
#define developer "Hajnal Szabolcs"
#define date "2023.04.23"

/*### UZEMMOD ###*/
#define send "-send"
#define receive "-receive"

/*### KOMMUNIKACIO MODJA ###*/
#define file "-file"
#define socket "-socket"

void help() {
    printf("------------------------------------\n");
    printf("HELP PARANCS => HASZNALAT:\n");
    printf("--version => verzioszam, fejleszto neve, fejlesztes datuma\n");
    printf("--help => parancssugo felulet\n");
    printf("-send => kuldo mod (ALAPERTELMEZETT)\n"); //ALAPERTELMEZETT MOD, HA NEM AD MEG PARANCSSORI ARGUMOT AMI ERRE UTAL!
    printf("-receive => fogado mod\n\n");
    printf("KOMMUNIKACIO MODJA:\n");
    printf("-file => file mod (ALAPERTELMEZETT)\n"); //ALAPERTELMEZETT MOD, HA NEM AD MEG PARANCSSORI ARGUMOT AMI ERRE UTAL!
    printf("-socket => socket mod \n\n");

    printf("A PROGRAM JELENLEG KULDO MODBAN UZEMEL!\n");   
    printf("A KOMMUNIKACIO JELENLEG FILE MODBAN TORTENIK!\n"); 
    printf("------------------------------------\n");
}

void kuldo_mod() {
    printf("A program kuldo modban uzemel!\n");
}

void fogado_mod() {
    printf("A program fogado modban uzemel!\n");
}

void file_mod() {
    printf("A kommunikacio file modban tortenik!\n");
}

void socket_mod() {
    printf("A kommunikacio socket modban tortenik!\n");
}

//2.FELADAT
int Measurement(int **Values) {
    int x = 0;
    
    return 1;
}

int main(int argc, char *argv[]) {
    if(strstr(argv[0], "chart.exe") == 0) {
        printf("Hibas fajlnev! A program leall!\n");
        exit(1);
    }
    if(argc == 1 || strstr(argv[1], "--help")) {
        help();
    } else if(strstr(argv[1], "--version")) {
        printf("Verzioszam: %s\n", version);
        printf("Fejleszto: %s\n", developer);
        printf("Fejlesztes datuma: %s\n", date);
    }
    else if(strstr(argv[1], receive)) { //FOGADO MOD, FILE MOD (ALAPERTELMEZETT)
        fogado_mod();
        file_mod();
    } else if(strstr(argv[1], send)) {  //KULDO MOD, FILE MOD (ALAPERTELMEZETT)
        kuldo_mod();
        file_mod();
    } else if(strstr(argv[1], send) && strstr(argv[2], file)) { //KULDO MOD, FILE MOD
        kuldo_mod();
        file_mod();
    } else if(strstr(argv[1], receive) && strstr(argv[2], file)) {  //FOGADO MOD, FILE MOD
        fogado_mod();
        file_mod();
    } else if(strstr(argv[1], send) && strstr(argv[2], socket)) {   //KULDO MOD, SOCKET MOD
        kuldo_mod();
        socket_mod();
    } else if(strstr(argv[1], receive) && strstr(argv[2], socket)) {    //FOGADO MOD, SOCKET MOD
        fogado_mod();
        socket_mod();
    }    
    else {
        printf("Ismeretlen parancs!\n");
    }
}