# Simple Shell

## Description

Simple Shell is a Holberton School team project whose goal is to understand
how a Unix command interpreter works and to build a simple shell in C.

The project is developed progressively. This README documents only behavior
and components that are required or verified during the project. Features are
not presented as implemented before they actually exist.

## Learning objectives

Through this project we will study and apply concepts including:

- Unix shells and command interpreters
- PID and PPID
- process creation with fork
- program execution with execve
- waiting for child processes
- environment variables
- command lookup through PATH
- end-of-file handling
- interactive and non-interactive execution
- error handling
- memory management

## Repository

Repository:

holbertonschool-simple_shell

The project is developed by a two-person Holberton team.

## Compilation

The project is expected to compile on Ubuntu 20.04 LTS with:

    gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## General requirements

The project follows the requirements defined by Holberton School, including:

- Betty coding and documentation style
- no memory leaks
- a maximum of five functions per source file
- header include guards
- use of only authorized functions and system calls
- use of system calls only when necessary and explainable

## Project workflow

We approach each task using the following sequence:

1. Read and understand the requirement.
2. Rewrite the problem in our own words.
3. Identify inputs, outputs, constraints, and expected errors.
4. Design the solution before coding.
5. Simulate the logic step by step.
6. Implement the smallest valid part.
7. Test and debug it.
8. Document only verified behavior.

## Current state

The repository contains the functional version developed for Simple Shell
1.0.

The current implementation can:

- run in interactive and non-interactive mode
- display the `#cisfun$ ` prompt in interactive mode
- read command lines with a custom buffered reader built on `read`
- execute commands from a file with `simple_shell [filename]`
- suppress the prompt while executing commands from a file
- stop at end-of-file without falling back to standard input
- stop processing later file commands when a valid `exit` command is encountered
- preserve unread input between calls with internal `static` buffer state
- split command lines into arguments with custom parsing logic without `strtok`
- build a NULL-terminated argument vector for `execve`
- execute commands provided with their full path
- resolve commands through the `PATH` environment variable with manual `:`-separated traversal without `strtok`
- pass command arguments and options to executed programs
- avoid creating a child process when a command cannot be resolved
- create a child process with `fork` only for a valid executable command
- execute programs with `execve`
- wait for the child process with `waitpid`
- propagate the child process exit status
- handle empty input lines and end-of-file input
- execute a final file command even when the last line has no trailing newline
- handle the built-in `exit` command with or without a numeric status argument
- stop the shell immediately when a valid `exit` command is entered
- preserve the last command status when leaving through `exit` without an argument
- convert a valid numeric `exit` argument to the effective process exit status
- report invalid numeric `exit` arguments and numeric overflow as errors
- reject extra `exit` arguments without terminating the shell immediately
- handle the built-in `env` command
- print the current environment, one variable per line
- handle the built-in `setenv VARIABLE VALUE` command
- create or replace environment variables inside the running shell
- handle the built-in `unsetenv VARIABLE` command
- remove environment variables inside the running shell
- preserve environment changes for subsequent commands in the same shell session
- apply changes to `PATH` immediately to later command resolution
- handle the built-in `cd` command
- change to an explicit directory with `cd DIRECTORY`
- use `HOME` when `cd` is called without an argument
- support `cd -` using `OLDPWD` and print the destination directory
- update `PWD` and `OLDPWD` after successful directory changes
- handle `exit`, `env`, `setenv`, `unsetenv`, and `cd` internally before command lookup through `PATH`
- survive `Ctrl+C` (`SIGINT`) while waiting for interactive input
- interrupt external child commands with `Ctrl+C` without terminating the shell
- temporarily ignore `SIGINT` in the parent while waiting for a child process
- execute multiple commands sequentially when they are separated by `;`
- continue with later `;`-separated commands after a normal command failure, while stopping immediately if `exit` is requested
- support logical command chaining with `&&` and `||`
- short-circuit `&&` after a failed command and `||` after a successful command
- evaluate mixed `&&` and `||` chains from left to right while preserving the status of the last command actually executed
- handle the built-in `alias` command
- handle the built-in `help` command
- handle the built-in `history` command
- load persistent command history from `$HOME/.simple_shell_history`
- record each non-empty input line exactly once in the command history
- display history entries with visible line numbers
- persist commands without line numbers when the shell exits
- display general built-in help with `help`
- display specific built-in help with `help BUILTIN`
- report an error for unknown help topics or extra help arguments
- create aliases with `alias name='value'`
- preserve alias values containing spaces when they are enclosed in single quotes
- query one or more aliases by name
- list all aliases defined during the current shell session
- replace the value of an existing alias
- expand `$?` to the exit status of the last command executed
- expand `$$` to the process ID of the running shell
- ignore comments beginning with `#` until the end of the current input line
- expand multiple `$?` or `$$` occurrences in the same command line
- preserve unsupported `$` forms literally instead of treating them as general environment-variable expansion

At this stage the implemented built-ins are `exit`, `env`, `setenv`,
`unsetenv`, `cd`, `alias`, `help`, and `history`. Environment changes made with `setenv` and
`unsetenv` persist inside the running shell and are used by later command
execution and `PATH` resolution. Successful `cd` operations update `PWD` and
`OLDPWD`, while `cd` without an argument uses `HOME` and `cd -` uses `OLDPWD`.
Aliases created with `alias` remain available during the current shell session
and can be queried, listed, or redefined. The `help` built-in displays general
help without arguments and specific help for `exit`, `env`, `setenv`,
`unsetenv`, `cd`, `alias`, and `help`. Unknown topics and extra arguments are
reported as errors. The `history` built-in displays the in-memory command
history with visible line numbers. The shell loads persistent history from
`$HOME/.simple_shell_history` at startup and writes commands back to that
file without line numbers when it exits. Each non-empty input line is
recorded once before command sequencing is evaluated. The shell also expands
`$?` to the
status of the last command that actually executed and `$$` to the process ID
of the running shell. Multiple occurrences of these special variables are
expanded consistently within the same command line. General `$VAR` environment
variable expansion is not currently implemented. Comment text beginning with
`#` is ignored before command sequencing and execution.

The shell can also execute commands from a file with `simple_shell [filename]`.
In file-input mode, the file is the only command source: no prompt is displayed,
commands are processed line by line through the same parsing, built-in, history,
sequence, variable, comment, and execution pipeline, and reaching end-of-file
does not fall back to standard input. A valid `exit` command stops processing
any later commands in the file.

## Documentation

This repository currently contains:

- README.md - project overview and evolving technical documentation
- man_1_simple_shell - manual page for the Simple Shell project
- AUTHORS - project contributors
- shell.c - Simple Shell parsing, built-in dispatch, execution loop, process handling, and history integration
- builtins.c - built-in command handling for `exit` and `help`
- input.c - custom buffered line input based on `read`, with persistent `static` buffer state and an explicit input file descriptor
- input_source.c - input-source selection, file opening, prompt control, and descriptor cleanup for standard-input and file-input modes
- path.c - command resolution through PATH
- env.c - `setenv` and `unsetenv` handling plus shell-owned environment memory management
- cd.c - `cd` handling, directory changes, and `PWD`/`OLDPWD` synchronization
- signals.c - interactive `SIGINT` handling and parent signal-state control
- sequence.c - sequential and logical execution of commands using `;`, `&&`, and `||`
- alias.c - alias storage, lookup, printing, replacement, and cleanup
- alias_parse.c - parsing and handling of raw `alias` command input
- variables.c - expansion of `$?` and `$$`, plus preparation and execution of expanded arguments
- comments.c - removal of shell comments beginning with `#`
- history.c - persistent command history loading, storage, display, saving, and cleanup
- shell_cleanup.c - shared final resource cleanup, including history persistence
- shell.h - shared declarations, alias and history structures, and required headers

## Authors

- Raider Del Castillo Abalos
- Theo Golik
