/*
 Author: Gustavo Ficher (gficher.com)
         Rafael Altarugio
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <signal.h>

#include "libinputparse.h"
#include "libcdhandler.h"
#include "libstrlist.h"
#include "libstrutils.h"

#define BUFFER_SIZE 1000


bool exit_prompt = false;
pid_t child_pid = -1;


void sigHandler(/*int sig*/);

void prompt();
char **readPrompt();

int spawnPipe(char** args, int pipe_r, int pipe_w);
int executeCommands(char **cmd_list, int cmd_index, int pipe_r);

void loop();
int main();



// Handle signal calls
void sigHandler(/*int sig*/) {
    return;
    // // printf("SIGINT Task: %d\n", child_pid);

    // if (child_pid != -1) {
    //     // A child process is running, so send it a signal
    //     kill(child_pid, SIGINT);
    // } else {
    //     // Nothing is running, it's time to go!
    //     printf("\n");
    //     exit(0);
    // }
}


// Print shell prompt
void prompt() {
    // Get all the stuff names
    char hostname[HOST_NAME_MAX];
    char *home_dir = getenv("HOME");
    char current_dir[PATH_MAX];
    char display_dir[PATH_MAX];
    int homeStart = 0;

    getcwd(current_dir, NAME_MAX);
    gethostname(hostname, HOST_NAME_MAX);


    // Check if the home path is a prefix of the current path and find the offset
    if (!strncmp(current_dir, home_dir, strlen(home_dir))) {
        homeStart = strlen(home_dir);
    }

    // print the home thing '~'
    if (homeStart > 0) {
        display_dir[0] = '~';
        strcpy(display_dir+1, current_dir+homeStart);
    } else {
        strcpy(display_dir, current_dir);
    }

    // Finally print the prompt
    printf("\033[0;31m[MySh] \033[0;32m%s@%s:\033[0;34m%s\033[0m$ ", getenv("LOGNAME"), hostname, display_dir);
}


// Read user input and return command array
char **readPrompt() {
    char buffer[BUFFER_SIZE];

    // Get input char by char
    int count = 0;
    while (count < BUFFER_SIZE) {
        char c = getchar();
        // printf("%d ", c);
        buffer[count++] = c;

        if (c == EOF || c == '\0' || c == '\n') {
            // Check Crtl+D
            if (c == EOF) {
                printf("\n");
                exit_prompt = true;
                count = 0;
            }

            break;
        }
    }
    buffer[count] = '\0';
    // printf("\n");
    
    // Check empty input
    if (strlen(trimWhitespace(buffer)) == 0 || (strlen(buffer) == 1 && buffer[0] == '\n')) {
        return NULL;
    }

    return parsePipe(buffer);
}

// Spawn new child process using pipes
int spawnPipe(char** args, int pipe_r, int pipe_w) {
    // Look for internal commands
    if (!strcmp(args[0], "cd")) {
        return cdHandler(args);;
    } else if (!strcmp(args[0], "exit")) {
        exit_prompt = true;
        return 0;
    }

    // Fork the process
    child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "Error while forking process.");
    }

    if (child_pid != 0) {
        // If this is the parent process, then return the child pid
        return child_pid;
    } else {
        // Handle pipes
        if (pipe_r != 0)
            dup2(pipe_r, STDIN_FILENO);
        if (pipe_w != 0)
            dup2(pipe_w, STDOUT_FILENO);

        // If this is the new child process, then run another program
        execvp(args[0], args);

        // If the process get to this point, then it haven't changed the program
        fprintf(stderr, "Error while running %s: %s\n", args[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    return -1;
}


// Recursively run all commands
int executeCommands(char **cmd_list, int cmd_index, int pipe_r) {
    // Return:
    //  0 - End of iteration
    // -1 - Process error
    // -2 - Pipe creation error

    int fds[2] = {0,0};
    pid_t pid;

    // Return from recursion
    if (cmd_list[cmd_index] == NULL) {
        return 0;
    }

    // Last command should not be piped
    if (cmd_list[cmd_index+1] != NULL) {
        if (pipe(fds) < 0) { 
            printf("Pipe could not be initialized\n"); 
            return -2; 
        }
    }

    // Split command into args
    char **args = parseArgExec(cmd_list[cmd_index]);

    // Spawn new process
    // printf("Running: %s\n", args[0]);
    pid = spawnPipe(args, pipe_r, fds[1]);
    // printf("PID: %d\n", pid);

    if (fds[1] != 0)
        close(fds[1]);

    // Only run next command if current command succeeds
    if (pid > 0) {
        executeCommands(cmd_list, cmd_index+1, fds[0]);
        wait(NULL);
    }

    if (fds[0] != 0)
        close(fds[0]);
    
    freeStrList(args);

    return 0;
}



// Prompt-input-run cycle
void loop() {
    char **commands;

    // Display prompt
    prompt();

    // Read command
    commands = readPrompt();

    // Check empty input
    if (commands == NULL) {
        return;
    }

    executeCommands(commands, 0, 0);

    freeStrList(commands);
}


// Setup and start the shell
int main() {
    // Handle CTRL+C
    signal(SIGINT, sigHandler);
    // Handle CTRL+Z
    signal(SIGTSTP, sigHandler);

    // Do stuff until 'exit' command
    while (!exit_prompt) {
        loop();
    }

    return 0;
}