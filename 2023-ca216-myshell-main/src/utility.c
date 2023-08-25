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
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include"myshell.h"

// All defined
#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

// Defining all variables
char *new_shell;
char *shell_path;
char **arg;
char buf[MAX_ARGS];
char *args[MAX_ARGS];
char cwd[MAX_BUFFER];
char *prompt;
int pid;
extern char **environ;
char help_path[MAX_BUFFER];

// Quits the shell
void quit()
{
    exit(0);
}

// Displays a list of the contents in chosen directory
void dir(char **args)
{
    // Defining variables used to run command later
    char *argument[MAX_ARGS];
    int i;
    // Setting the first 2 argument array to be 'ls -al' which is hte display format
    argument[0] = "ls";
    argument[1] = "-al";

    // Iterate through arguments
    for(i = 1; args[i] != NULL; i++)
    {
        // Sets the file after the 'ls -al' command as the next argument
        argument[i+1] = args[i];
    }

    // If file is not empty
    if(args[1] != NULL)
    {
        // Remove the next argument in the array
        argument[i+1] = NULL;
        // Run command using execute to run 'ls' alongside the file
        execvp("ls", argument);
    }
    // If no directory is given
    else
    {
        // Print error message
        printf("Directory cannot be empty.\n");
    }
}

// Prints out all arguments after echo
void echo(int argc, char **args)
{ 
    // Setting arg to be all arguments in args
    arg = args;
    // Iterate through arg (pointer means that it is now iterable)
    arg += 1;
    // While there are still arguments
    while(*arg)
    {
        // Prints it the standard output the string at current argument
        fprintf(stdout, "%s", *arg++);
        // Iterating means that space length is irrelevant, fputs to simulate one white space
        fputs(" ", stdout);
    }
    printf("\n");

}

// Shows all environment variable strings
void env()
{
    // Iterate through environment strings as long as there still are argument 
    for(int i = 0; environ[i] != NULL; i++)
    {
        // Print environment strings
        printf("%s\n", environ[i]);
    }
}

// Clears the screen
void clr()
{
    // Set argument to have the array 'clear' and NULL
    char *argument[] = {"clear", NULL};
    /* Command 'execvp' needs 2 pointer inputs so we set the first one to be the 'clear' argument
       and the rest does not matter as clear has no other arguments so we set it as NULL */
    execvp(argument[0], argument);
}

// Changes directory
void cd(char **args)
{
    char old_cwd[MAX_BUFFER];
    // Checks if the first argument after 'cd' is empty
    if(args[1] == NULL)
    {
        // If there is no specified directory, just print out the current working directory
        getcwd(cwd, sizeof(cwd));
        printf("Current working directory: %s\n", cwd);
    }
    // If directory is provided
    else
    {
        // Gets the new current directory
        getcwd(cwd, sizeof(cwd));
        // Sets old directory
        strcpy(old_cwd, cwd);
        // Uses built in function to change directory
        chdir(args[1]);
        // Gets the new current directory
        getcwd(cwd, sizeof(cwd));
        // If old directory is the same as the new one, it means no directory was found
        if(!strcmp(old_cwd, cwd))
        {
            printf("Directory does not exist.\n");
        }
        else
        {
            // Sets the environment variable 'PWD' (current working directory) to be equal to current directory
            setenv("PWD", cwd, 1);
            // Concatenated current working directory with '$' to display as prompy
            strcat(cwd, " $ ");
            prompt = cwd;
        }
    }
}

// Pauses all function of shell until the 'Enter' key is pressed
void pause_command()
{
    // Print a message for the user to see how to unpause the shell
    printf("Press Enter to continue...\n");
    // Uses system command to get rid of keyboard inputs
    system("stty -echo");
    // While the most recent pressed key is not the 'Enter' key (also the '\n' character)
    while(getchar() != '\n')
    {
        // Does nothing, just waits
    }
    // Show the keyboard inputs again
    system("stty echo");
}

// Runs Linux commands if no supported command used
int external_command(char **args)
{
    arg = args;
    // Create a child processer to run the external commands
    pid = fork();
    int status;
    // Checks if processer ID is working
    if(pid == 0)
    {
        // Checks and runs child processer as external command
        if(execvp(args[0], args) == -1)
        {
            // Print error message if command is not found
            printf("Command not found.\n");
        } 
    }
    // If processor is not valid
    else
    {
        // Do not wait on the child processer
        waitpid(pid, &status, 0);
    }
    // Return back to terminal
    return 1;
}

// Input/Output redirection. Uses a file as input, uses a file as output, and adds on to an existing file as output
void IO_Redirection(char **args)
{
    // Makes child process
    pid = fork();
    // If process is valud
    if(pid == 0)
    {
        // Iterate through arguments
        for(int i = 1; args[i] != NULL; i++)
        {
            // If the current argument is '<' and the next argument is not nothing, use the next argument as an input file
            if(!strcmp(args[i], "<") && args[i+1] != NULL)
            {
                // Opens the file and read from it
                FILE *input = fopen(args[i+1], "r");
                // If file is empty
                if (input == NULL)
                {
                    // Print an error message
                    fprintf(stderr, "Error: Failed to open input file '%s'\n", args[i+1]);
                    // Leave the process and go back to terminal
                    exit(1);
                }
                // Copies the file descriptor to the standard input
                dup2(fileno(input), STDIN_FILENO);
                // Close the file
                fclose(input);
                // Set the '<' character and file to nothing so it does not get in the way of the command
                args[i] = NULL;
                args[i+1] = NULL;
            }
            // If the current argument is '>' and the next argument is not nothing, use the next argument as an output file
            else if(!strcmp(args[i], ">") && args[i+1] != NULL)
            {
                // Opens the file and write in it
                FILE *output = fopen(args[i+1], "w");
                // If file is empty
                if(output == NULL)
                {
                    // Print an error message
                    fprintf(stderr, "Error: Failed to open output file '%s'\n", args[i+1]);
                    exit(1);
                }
                // Copies the file descriptor to the standard output
                dup2(fileno(output), STDOUT_FILENO);
                // Close the file
                fclose(output);
                // Set the '>' character and file to nothing so it does not get in the way of the command
                args[i] = NULL;
                args[i+1] = NULL;

            }
            // If the current argument is '>>' and the next argument is not nothing, use the next argument as an output file
            else if(!strcmp(args[i], ">>") && args[i+1] != NULL)
            {
                // Opens the file and add onto it
                FILE *output = fopen(args[i+1], "a");
                // If file is empty
                if (output == NULL)
                {
                    // Print an error message
                    fprintf(stderr, "Error: Failed to open output file '%s'\n", args[i+1]);
                    exit(1);
                }
                // Copies the file descriptor to the standard output
                dup2(fileno(output), STDOUT_FILENO);
                // Close the file
                fclose(output);
                // Set the '>>' character and file to nothing so it does not get in the way of the command
                args[i] = NULL;
                args[i+1] = NULL;
            }
        }
        // Run command as external command
        execvp(args[0], args);
        // Return back to terminal
        exit(0);
    }
}

// Displays the manual for 'myshell'
void help(char help_path[MAX_BUFFER])
{
    // Set appropriate size for command
    char command[MAX_BUFFER];
    // Concatenate the command 'more' to the command
    strcpy(command, "cat ");
    // Concatenate the file path to the command
    strcat(command, help_path);
    // Concatenate futher file path to get to the manual directory
    strcat(command, "/../manual/readme.md | more");
    // Run the command through system
    system(command);
    // Clear the screen afterwards
}

// Executes a command as a background process
void background_execution(char ** args)
{
    // Iterate through arguments
    for(int i = 0; args[i] != NULL; i++)
    {
        // Checks if current argument iteration is an ampersand
        if(!strcmp(args[i], "&"))
        {
            // Create child proces
            pid_t pid = fork();

            // If the process is invalid
            if (pid == -1)
            {
                // Print an error
                perror("fork");
                exit(1);
            }
            // If process is valid
            else if(pid == 0)
            {
                // Run arguments as external commands
                execvp(args[0], args);
                // If fail, print error
                perror("execvp");
                exit(1); 
            }
            // After command start running
            else
            {
                // Print a message telling the user that the current process is running in the background
                printf("Background, process with PID %d started.\n", pid);
                
            }
           
        }
    }
}
