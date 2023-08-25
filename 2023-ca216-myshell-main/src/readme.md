USER MANUAL - 'myshell' [Press 'Q' to quit the manual]

INTRODUCTION[1]
        'myshell' is a command line interpreter that executes commands. 'myshell' runs both internal commands and external commands

SET UP
        To install the 'myshell' you have to download the '/myshell' directory.
        After that you have to go into the directory in terminal.
        Then you have to run the command 'make' to make sure everything is compiled.
        Finally, to run 'myshell' you have go to the '/bin' directory first and then run the command './myshell'
        You can enter batchfile mode by running the command './myshell [batchfile] (See below at BATCHFILE for futher documentation).

DESCRIPTION
        This shell is made to replace most, but not all functionalities that the normal Linux Terminal has. It is not mean to fully replace the normal Linux Terminal
        but it has most of the functionalities. There are various amounts of internal commands that are supported by the shell while also having support for I/O
        redirection, background execution and external commands (see the different sections below for more documentation.

BATCHFILE
        'myshell' supports command line arguments. It takes in a batchfile by running './myshell [batchfile]'. If the batchfile contains a set of commands, they will be read
        and executed in the 'myshell' instance and then quit.

    ./myshell [batchfile]

EXECUTION
        'myshell' can be executed by running the command './myshell' in the Linux terminal. When there is no batchfile or any other argument, an interactive shell 
        session begins. The user can then type command lines into the shell and run them using the 'Enter' key. The output of the command will be displayed in the
        terminal. The session ends when the user types in and runs 'quit'.

These are a set of the internal commands (supported commands by the shell). 

Internal Commands (Supported Commands) cd [directory]. Change th current working directory to [directory] and change the PWD environment variable (see environ).
If the argument [directory] is not present, report the current directory. If the directory does not exist, an error will be reported.


    clr
            Clears the screen.
            No arguments needed.

    dir [directory]
            Lists the contents of [directory].
            If no [directory] is present, reports the current directory

    echo [comment]
            Displays [comment] and also prints a newline.
            If all spaces or tabspaces are reduced to one white space.

    quit
            Quits the shell.

    env
            Prints a list of environment strings. Changing the current working diectory changes the 'PWD=' to current working directory.

    cd [directory]
            Changes the current working directory to [directory]. (See 'env' command to check changes to environment)
            If no [directory] argument is present, it does nothing.


    pause
            Pause the shell operation until the 'Enter' key is pressed.
            Typing is also paused until the 'Enter' key is pressed.

    help
            Display the user manual using the 'more' filter[2]. (Run 'man more' to see the Linux manual for the 'more' command)
            Press 'Enter' to display next line.i
            Press 'Space' bar to display the next page.
            Press 'Q' key to quit.
            User can scroll up or down through the page.

    

EXTERNAL COMMANDS
        'myshell' supports execution of Linux commands. 'myshell' will return the error "Command not found." if the command is not found.

ENVIRONMENT[3]
        Global varibales are visible from the shell session and all child subshells.
        'myshell' also contians elements from the Linux shell environment variables.
        USER: The current user logged in.
        HOME: The home directory, set as current directory too.
        PWD: The current working directory.
        SHELL: The absolute path of the shell. The path to the directory where 'myshell' and all other dependant files are in.

        Local varaibles are only visible to the local process in which they are defined.

I/O REDIRECTION
        'myshell' supports the use of I/O Redirection[3]. Before any commands are executed, first the command line checks for characters such as '>', '<' or '>>'.
        These characters mean 'stdout' (Standard Output), 'stdin' (Standard Input) and '>>' appending 'stdout' (Appending Standard Input).

                [command] [arg1] [arg2] > [output file]
                        Causes any output created from commands to be redirected to [output file] instead of terminal.

                [command] < [input file]
                        Runs the command [command] using arguments from the input file [input file].
                        Can be used as a replacement for terminal input for coding languages such as python.

                [command] [arg1] [arg2] >> [output file]
                        Does the same as '>' except if there is an existing file it adds the output at the end of the file.
                        If there is no outfile file [output file] it creates it instead.

        The shell uses 'dup2()' to redirect the contents of any file to the shell or redirect the contents of a shell to a file. The funciton 'dup2()' works by
        copying the shell's file descriptor and associate it with another file. This allows the shell to report the output to said file instead of the terminl
        as the share a link now.

EXTERNAL COMMANDS
        If a command [command] is entered that is not part of the internal commands, 'myshell' creates a child process by duplicating the parent process.
        This is done using forking. The child process runs seperately to the parent process. This allows Linux commands to run in 'myshell'.
        'myshell' waits until the child process ends before going back to the parent process.

CONCEPT OF PROCESSES[4]
	'myshell' can run processes in the foreground and also in the background. (Indepth of background processes explained in BACKGROUND EXECUTION part).
	Foreground processes are those which a user can actively interact with that are also running. Usually they need user inputs and displays the output on
	the screen. Foreground processes have high priority which means they recieve more resources.

	Bakcground processes run silently in the background usually without any user inputs. Background processes are usually processes such as file content
	transferring and printing. These processes also run when a user is not logged in. Background processes are lower down in priority than foreground
	processes. This means that they get less resources which can contribute to slower run times.


BACKGROUND EXECUTION[5]
        'myshell' has the function to put processes in the background using the '&' character at the end of a command or else they run in the foreground.
        Commands will keep running in the background but 'myshell' will immediately return to the command line (This is more visible with large commands).
        This has a similar effect to Linux's 'CTRL-Z' command which puts the current proces in the background. Normally, foreground processes (normal command
        executions) have to finish before being able to execute any further command. Backgorund processes changes this so you can execute commands while the other
        bavkground process runs. This is done by making a child process that runs independantly to the shell (parent process). This is how they can both run
        independantly.

REFERENCES
    [1] C. Ramey, "Bash Reference Manual," Network Theory Limited, 1998. [Online].
	Available: https://fossies.org/linux/misc/bash-5.1.tar.xz/bash-5.1/doc/bashref.pdf. [Accessed: March 15, 2023].
    [2] The Open Group, "more - display files on a page by page basis", The Open Group Base Specifications Issue 7, 2018 edition, [Online].
        Available: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/more.html. [Accessed: Mar. 15, 2023].
    [3] B. Richardson, "Using AWK for Data Management", Linux Journal, Nov. 2000. [Online].
        Available: https://www.linuxjournal.com/article/3645. [Accessed: Mar. 15, 2023].
    [4] S. Androutsellis-Theotokis and D. Spinellis, "A survey of peer-to-peer content distribution technologies,"
	ACM Computing Surveys, Dec. 2008, [Online]. Available: https://dl.acm.org/doi/pdf/10.1145/1357010.1352596. [Accessed: Mar. 18, 2023].
    [5] Mendel Cooper, "Advanced Bash-Scripting Guide - Chapter 16. Manipulating Variables", The Linux Documentation Project, 2012. [Online].
        Available: https://tldp.org/LDP/abs/html/x9644.html. [Accessed: Mar. 15, 2023].


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
