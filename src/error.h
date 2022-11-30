/*
error.h - Primary multimake error handling
*/

#ifndef ERROR_H
#define ERROR_H

#include "color.h"
#include "args.h"

#include <iostream>

inline void error(std::string err, bool exits = true) {
    std::cout << BOLDRED << "multimake: error: " << RESET << err << std::endl;
    if (exits) exit(1);
    return;
}

inline void warning(std::string warn) {
    std::cout << BOLDYELLOW << "multimake: warning: " << RESET << warn << std::endl;
    return;
}

inline void info(std::string info) {
    std::cout << GREY << "multimake: info: " << RESET << info << std::endl;
    return;
}

inline bool should_debug = false;

inline void check_debug() {
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == "-d" || std::string(argv[i]) == "--debug") {
            should_debug = true;
            // Remove from args
            for (int j = i; j < argc; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            return;
        }
    }
}

inline void debug(std::string debug) {
    if (should_debug)
        std::cout << BOLDGREY << "multimake: debug: " << RESET << debug << std::endl;
    return;
}

#endif