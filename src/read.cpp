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
#include <regex>
#include <numeric>
#include <sstream>

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
    std::vector<std::tuple<std::string, std::string>> variables;

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

        std::string reg = "var\\s+([a-zA-Z0-9]+)\\s=\\s*(.*)";
        std::regex var(reg);
        std::smatch match;

        if (std::regex_search(lines[i], match, var)) {
            std::string name = match[1];
            std::string value = match[2];
            variables.push_back(std::make_tuple(name, value));
        }
    }

    // Join all content into single string where each line is separated by a newline
    std::string content;
    for (int i = 0; i < (int)lines.size(); i++) {
        content += lines[i] + "\n";
    }

    // Targets are defined as:
    // target <target>(<dependencies>) { \n<commands>\n }
    // So we need to check for those.

    // A cool regex to get everything (newlines, spaces, just absolutely everything) is: "\\s*([^\\s]+)\\s*"

    std::string reg = "target\\s+([a-zA-Z0-9]+)\\((.*)\\)\\s*\\{\\s*([^\\}]*)\\s*\\}";
    std::regex target(reg);
    std::smatch match;

    // Replace all variables with their values
    for (int i = 0; i < (int)variables.size(); i++) {
        std::string name = std::get<0>(variables[i]);
        std::string value = std::get<1>(variables[i]);
        std::string reg = "@\\(" + name + "\\)";
        std::regex var(reg);
        content = std::regex_replace(content, var, value);
    }

    debug("Content: " + content);
    
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

    bool didFindArgumentTarget = false;
    
    // Parse cl args
    for (int i = 1; i < argc; i++) {
        // If its the name of a target, run the commands
        if (std::find(targets.begin(), targets.end(), argv[i]) != targets.end()) {
            didFindArgumentTarget = true;
            debug("Found target " + std::string(argv[i]) + " in arguments");
            
            // Run each line of the command
            std::string command = commands[std::distance(targets.begin(), std::find(targets.begin(), targets.end(), argv[i]))];
            std::stringstream ss(command);
            std::string line;
            while (std::getline(ss, line, '\n')) {
                debug("Running command: " + line);
                system(line.c_str());
            }
        } else {
            error("Could not find target " + std::string(argv[i]) + " in MultiMake file");
        }
    }

    if (!didFindArgumentTarget) {
        // Run first target
        debug("Running first target");

        if (commands.size() > 0) {
            // Run each line of the command
            std::stringstream ss(commands[0]);
            std::string to;

            while (std::getline(ss, to, '\n')) {
                debug("Running command: " + to);
                system(to.c_str());
            }
        } else {
            error("No targets found in MultiMake file");
        }
    }

    return 0;
}