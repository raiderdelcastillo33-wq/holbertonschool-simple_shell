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
- handle the built-in `exit` command without arguments
- stop the shell immediately when `exit` is entered
- preserve the last command status when leaving through `exit`
- handle the built-in `env` command
- print the current environment, one variable per line
- handle `exit` and `env` internally before command lookup through `PATH`

At this stage the built-ins implemented for Simple Shell 1.0 are `exit` and
`env`. Other built-in commands belong to later project requirements and are
not documented as implemented yet.

## Documentation

This repository currently contains:

- README.md - project overview and evolving technical documentation
- man_1_simple_shell - manual page for the Simple Shell project
- AUTHORS - project contributors
- shell.c - Simple Shell parsing, built-in exit and env handling, execution loop, and process handling
- input.c - custom buffered line input based on `read`, with persistent `static` buffer state
- path.c - command resolution through PATH
- shell.h - shared declarations and required headers

## Authors

- Raider Del Castillo Abalos
- Theo Golik
