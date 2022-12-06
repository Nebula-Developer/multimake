/*
exec.cpp - Execute commands fetched from read.cpp
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <sstream>

#include "exec.h"

/**
 * Execute command and return the status code
 */
int exec(std::string command) {
    // Use pipe
    int pipefd[2];
    pipe(pipefd);

    // Fork
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Split command into arguments
        std::istringstream iss(command);
        std::vector<std::string> args((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

        // Convert to char**
        char** argv = new char*[args.size() + 1];
        for (int i = 0; i < (int)args.size(); i++) {
            argv[i] = new char[args[i].size() + 1];
            std::copy(args[i].begin(), args[i].end(), argv[i]);
            argv[i][args[i].size()] = '\0';
        }
        argv[args.size()] = NULL;

        // Execute
        execvp(argv[0], argv);
    } else {
        // Parent process
        close(pipefd[1]);
        char buffer[1024];
        read(pipefd[0], buffer, 1024);
        std::cout << buffer;
        close(pipefd[0]);
    }

    return 0;
}