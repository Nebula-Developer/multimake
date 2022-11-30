/*
error.h - Primary multimake error handling
*/

#ifndef ERROR_H
#define ERROR_H

#include "color.h"
#include <iostream>

void error(std::string err, bool exits = true) {
    std::cout << BOLDRED << "multimake: error: " << RESET << err << std::endl;
    if (exits) exit(1);
    return;
}

void warning(std::string warn) {
    std::cout << BOLDYELLOW << "multimake: warning: " << RESET << warn << std::endl;
    return;
}

void info(std::string info) {
    std::cout << GREY << "multimake: info: " << RESET << info << std::endl;
    return;
}

#endif