/*
main.cpp - Main file for MultiMake
*/

#include <iostream>

#include "read.h"
#include "error.h"

int argc;
char **argv;

/**
 * Base entry point - not much happens.
 */
int main(int argct, char **argval) {
    argc = argct;
    argv = argval;
    check_debug();
    return initRead();
}
