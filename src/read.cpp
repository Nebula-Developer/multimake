/*
read.cpp - Read MultiMake files and run (final use)
*/

#include "read.h"
#include "error.h"
#include "args.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <regex>
#include <numeric>

int initRead() {
    std::ifstream file("MultiMake");

    if (!file) {
        error("Could not locate or open MultiMake file in current directory");
        return -1;
    }

    std::string line;

    std::vector<std::string> lines; // Primary vector for line reading/handling
    std::vector<std::string> commands;
    std::vector<std::string> targets;
    std::vector<std::string> dependencies;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    // Line by line parsing:
    for (int i = 0; i < (int)lines.size(); i++) {
        // Variables are declared as such:
        // var <name> = <value>
        // And referenced by:
        // @(<name>)
        // So we need to check for those.

        std::string reg = "var\\s+([a-zA-Z0-9]+)\\s*=\\s*(.*)";
        std::regex var(reg);
        std::smatch match;

        if (std::regex_search(lines[i], match, var)) {
            std::string name = match[1];
            std::string value = match[2];

            // TODO: Improve stability and efficiency

            // Now we need to replace all instances of @(<name>) with <value>
            for (int j = 0; j < (int)lines.size(); j++) {
                std::string reg = "@\\(" + name + "\\)";
                std::regex var(reg);
                std::string value = match[2];
                lines[j] = std::regex_replace(lines[j], var, value);
            }

            debug("Found variable " + name + " with value " + value);
        }
    }

    // Join all content into single string for regex
    std::string content = std::accumulate(lines.begin(), lines.end(), std::string()); // #include <numeric>

    // Targets are defined as:
    // <target>(<dependencies>) { <commands> }
    // So we need to check for those.

    std::string reg = "target\\s+(\\w+)\\((\\w+)\\)\\s*\\{\\s*(.*)\\s*\\}";
    std::regex target(reg);
    std::smatch match;

    while (std::regex_search(content, match, target)) {
        std::string target = match[1];
        std::string dependency = match[2];
        std::string command = match[3];

        debug("Found target " + target + " with dependency " + dependency + " and command " + command);

        targets.push_back(target);
        dependencies.push_back(dependency);
        commands.push_back(command);

        content = match.suffix().str();
    }

    
    // Parse cl args
    for (int i = 0; i < argc; i++) {
        // If its the name of a target, run the commands
        if (std::find(targets.begin(), targets.end(), argv[i]) != targets.end()) {
            debug("Found target " + std::string(argv[i]) + " in arguments");
            int index = std::distance(targets.begin(), std::find(targets.begin(), targets.end(), argv[i]));
            debug("Running command " + commands[index]);
            system(commands[index].c_str());
        }
    }

    return 0;
}