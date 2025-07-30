# minishell

## Overview
minishell is a simplified Unix shell implemented in C. It reads user input, tokenizes and parses commands, expands environment variables, checks syntax, and executes commands, mimicking basic shell behavior.

## Features
- Read user input with prompt and history
- Tokenize input into commands, arguments, pipes, and redirections
- Syntax checking for valid shell grammar
- Environment variable expansion ($VAR)
- Command parsing and execution
- Memory cleanup after each command

## Workflow
1. **Read Input**: Get a line from the user using `readline()` and add to history
2. **Tokenizer**: Split input into tokens (words, pipes, redirections)
3. **Syntax Checker**: Validate token sequence (no pipes at start/end, redirection rules, etc.)
4. **Variable Expander**: Replace `$VAR` with environment values, respecting quote rules
5. **Parser**: Build command structures for execution
6. **Executor**: Run commands, handle pipes and redirections
7. **Cleanup**: Free all allocated memory

## Example
```sh
minishell$ echo $USER

minishell$ echo "$HOME/file"
/home/file
minishell$ echo '$USER'
$USER
```

## Build & Run
1. Clone the repository
2. Build with `make`
3. Run with `./minishell`

## File Structure
- `src/` - Source code
- `include/` - Header files
- `libft/` - Custom C library
- `shell.drawio` - Project workflow diagram

## License
MIT