
# Minishell

Minishell is a minimal Unix shell written in C for the 42/1337 school project. It aims to replicate basic shell behavior, including command parsing, execution, environment management, pipes, redirections, and built-in commands.

---

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Redirections & Pipes](#redirections--pipes)
- [Environment Variables](#environment-variables)
- [Project Structure](#project-structure)
- [Development Guide](#development-guide)
- [Troubleshooting](#troubleshooting)
- [Authors](#authors)
- [License](#license)

---

## Features
- Command parsing and tokenization
- Execution of external programs
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Environment variable management
- Pipes (`|`) and redirections (`>`, `<`, `>>`, heredoc)
- Signal handling (SIGINT, SIGQUIT)
- Custom garbage collector for memory management

---

## Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/M-elhouary/rep.git
   cd rep/MINISHELL
   ```
2. **Build the project:**
   ```sh
   make
   ```
3. **Run Minishell:**
   ```sh
   ./minishell
   ```

---

## Usage
- Type commands as you would in a regular shell:
  ```sh
  minishell$ ls -l | grep minishell > out.txt
  minishell$ echo $HOME
  minishell$ cd ..
  minishell$ export VAR=value
  minishell$ unset VAR
  minishell$ exit
  ```
- Supports chaining commands with pipes and redirections.
- Environment variables can be expanded using `$VAR` syntax.

---

## Built-in Commands
- `echo [args...]` : Print arguments to stdout
- `cd [dir]` : Change current directory
- `pwd` : Print working directory
- `export VAR=value` : Set environment variable
- `unset VAR` : Remove environment variable
- `env` : List environment variables
- `exit [status]` : Exit the shell

---

## Redirections & Pipes
- `>` : Redirect output to file (overwrite)
- `>>` : Redirect output to file (append)
- `<` : Redirect input from file
- `|` : Pipe output of one command to another
- `<<` : Heredoc (multi-line input until delimiter)

**Examples:**
```sh
minishell$ echo "Hello" > hello.txt
minishell$ cat < hello.txt | grep H
minishell$ cat << EOF
Line1
Line2
EOF
```

---

## Environment Variables
- Set: `export NAME=Spider`
- Use: `echo $NAME`
- Unset: `unset NAME`

---

## Project Structure
- `src/exec/` : Execution logic, built-ins, environment, pipes, redirections
- `src/parser/` : Tokenization, parsing, syntax checking, expansion, heredoc
- `src/garbage_c/` : Garbage collector implementation
- `src/shell_main/` : Main shell loop and helpers
- `libft/` : Custom standard library functions
- `include/` : Header files

---

## Development Guide
- **Adding a Built-in:**
  - Implement the function in `src/exec/` (e.g., `b_echo.c` for echo)
  - Register it in `builin.c`
- **Adding a Parser Feature:**
  - Update tokenization logic in `src/parser/tokens/`
  - Update syntax checks in `src/parser/syntax_checker.c`
- **Memory Management:**
  - Use the custom garbage collector in `src/garbage_c/`
- **Testing:**
  - Test with various shell commands, edge cases, and invalid syntax

---

## Troubleshooting
- **Build errors:** Ensure you have a C compiler (e.g., gcc) and GNU Make installed.
- **Segmentation faults:** Check for memory leaks or invalid pointer usage, especially in parser and exec modules.
- **Command not found:** Ensure your PATH is set correctly and external commands exist.

---

## Authors
- mel-houa
- houardi

---

## License
This project is for educational purposes at 42/1337 school.
