//shell.h
//author: Niko Tepe, Leon Rösener, Nikolas Schawe

#ifndef SHELL_H
#define SHELL_H

#define MAX_ARGS 64 // Maximale Anzahl der Argumente^
#define INPUT_BUFFER_MAX 512

/*---LIBRARIES---*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>


void shell();
#endif