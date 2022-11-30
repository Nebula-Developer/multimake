/*
read.cpp - Read MultiMake files and run (final use)
*/

#include "read.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <regex>

void initRead() {
    std::ifstream file("MultiMake");

    if (!file) {
        std::cout << "multimake: error: no MultiMake file found" << std::endl;
        return;
    }

    std::string line;

    std::vector<std::string> lines;
    std::vector<std::string> commands;
    std::vector<std::string> targets;
    std::vector<std::string> dependencies;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    for (int i = 0; i < (int)lines.size(); i++) {
        std::cout << "Line " << i << ": " << lines[i] << std::endl;
    }

    return;
}