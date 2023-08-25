/*
Author: Christopher Muthi
Student Number: 21365126
Acknowlegment of the DCU Academic Integrity Policy:
        I understand that the University regards breaches of academic integrity and plagerism as grave and serious.
        
        I have read and understood the DCU Academic Integrity and Plagerism Policy. I accept the penalties that may be imposed should I engage in practice or practices
        that breach this policy.

        I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references.

        I delcare that this material, whihc I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent
        that such work has been cited and acknowledged within teh text of my work.

        I have used the DCU library referencing guidelines and/or the appropriate referencing system recommened in the assignment guidelines and/ore programme
        documentation.
*/

// All includes used
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/wait.h>

// Defined commonly used values
#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

// Define some variables
char *new_shell;
char *shell_path;
extern char **environ;

// Call all functions
void quit();
void clr();
void dir(char **args);
void echo(int argc, char **args);
void env();
void cd(char **args);
void pause_command();
int external_command(char **args);
void IO_Redirection(char **args);
void help();
void background_execution(char ** args);

// External variables
extern char *path;
extern char *new_shell;
extern char *shell_path;
extern char cwd[MAX_BUFFER];
extern char **environ;
