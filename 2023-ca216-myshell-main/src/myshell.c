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

#include"myshell.h"

int main (int argc, char **argv)
{
    //Defining variabkes
    char buf[MAX_BUFFER];                     // line buffer
    char *args[MAX_ARGS];                     // pointers to arg strings
    char **arg;                               // working pointer thru args
    char cwd[MAX_BUFFER];                     // current working directory
    char *prompt = "$ ";                      // shell prompt 
    int counter = 0;                          // counter used for I/O redirection
    int batchchecker = 0;                     // checks if there is a batchfile
    char help_path[MAX_BUFFER];               // tracks main path for manual file

    // Checks if myshell is executed alongside a file to enter batch mode
    if(argv[1] != NULL)
    {
        // Open and read commands from batchfile
        freopen(argv[1], "r", stdin);
        // Sets batch checker to 1 to tell other commands that a batchfile is used
        batchchecker = 1;
    }

    // Gets current path and set it as the environment SHELL and to display current path in the prompt
    getcwd(cwd, sizeof(cwd));
    strcpy(help_path, cwd);
    strcat(cwd, "/myshell");
    setenv("SHELL", cwd, 1);
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, " $ ");
    prompt = cwd;

    // Checks if batch mode is on to remove prompt
    if(batchchecker == 1)
    {
        prompt = "";
    }

    // Makes sure that the file is contstantly on
    while(!feof(stdin))
    {
        // Puts the prompt in the output           
        fputs(prompt, stdout);
                
        if(fgets(buf, MAX_BUFFER, stdin))
        {
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);

            while((*arg++ = strtok(NULL,SEPARATORS)));

            // Checks what to execute with command given
            if (args[0])
            {
                // Checks for characters associated with I/O redirection by iterating all arguments
                for(int i = 0; args[i] != NULL; i++)
                {
                    // Checks if current iteration is such character
                    if(!strcmp(args[i], ">") || !strcmp(args[i], "<") || !strcmp(args[i], ">>"))
                    {
                        // Counter exists to check if I/O redirection should be used
                        counter += 1;
                    }
                } 
                
                // If character for I/O redirection is found
                if(counter >= 1)
                {
                    // Run redirection function
                    IO_Redirection(args);
                    // Resets coubter for further commands
                    counter = 0;
                }
                // If no characters for I/O redirection is found
                else if(counter == 0)
                {
                    // Checks for clear argument
                    if(!strcmp(args[0], "clr"))
                    {
                        // Run clear function
                        clr();
                        continue;
                    }

                    // Checks for directroy argument
                    if(!strcmp(args[0], "dir"))
                    {
                        // Run directory function
                        dir(args);
                        continue;
                    }

                    // Checks for echo argument
                    if(!strcmp(args[0], "echo"))
                    {
                        // Run echo funtion
                        echo(argc, args);
                        continue;
                    }

                    // Checks for quit arugment
                    if(!strcmp(args[0], "quit"))
                    {
                        // Run quit function
                        quit();
                    }

                    // Checks for environment argument
                    if(!strcmp(args[0], "environ"))
                    {
                        // Run environment function
                        env();
                        continue;
                    }

                    // Check for change directory argument
                    if(!strcmp(args[0], "cd"))
                    {
                        // Run change argument function
                        cd(args);
                        // Get current directory as new current directory
                        getcwd(cwd, sizeof(cwd));
                        // Add '$' to the directory to make it the new prompt
                        strcat(cwd, " $ ");
                        // Set the prompt as the new directory, concatenated with '$'
                        prompt = cwd;
                        continue;
                    }

                    // Checks for pause argument
                    if(!strcmp(args[0], "pause"))
                    {
                        // Run pause command
                        pause_command();
                        continue;
                    }

                    // Check for help argument
                    if(!strcmp(args[0], "help"))
                    {
                        // Run help command
                        help(help_path); 
                        continue;
                    }

                    // If argument is not part of internal commands
                    else
                    {
                        // Checker for ampersand for background execution
                        int and_checker = 0;
                        // Iterate through arguments for the ampersand
                        for(int i = 0; args[i] != NULL; i++)
                        {
                            // Compares current argument iteration for ampersand
                            if(!strcmp(args[i], "&"))
                            {
                                // Increases checker to make sure background execution happens instead of external command
                                and_checker += 1;
                            }
                        }
                        // If no ampersand is found
                        if(and_checker == 0)
                        {
                            // Run the external command functiom
                            external_command(args);
                            continue;
                        }

                        // If no ampersand is found
                        if(and_checker > 0)
                        {
                            // Run background execution function
                            background_execution(args);
                            continue;
                        }
                    }
                }
            }
        }
    }
    // Free memory
    free(arg);
    free(prompt);

    // Exit the shell (is necessary in C to add)
    return 0; 
}
