# retval

A small C program that runs a given shell command and prints its return value. Reads the command from stdin or arguments, runs commands with `/bin/sh`, installs to `/usr/local/bin` with `make install`, and is run with `retval`.<br>
Feel free to change the code as you'd like. MIT License applies.

## Requirements

- x86_64 Linux
- `gcc` and `make`

## Build & run
```shell
make build
make run   # test run with "echo hi"
```

## Install & uninstall

Installs to `/usr/local/bin`. Needs su permissions.
```shell
make install
make uninstall
```

# Usage

```shell
echo "exit 69" | retval
# or
retval # then type in your command
# or
retval "exit 69" # double quotes, shell will parse, therefore can use variables
retval 'exit 69' # double quotes, directly given to retval without bash parsing
retval exit 69
```

# Output format

If the given command exits itself, `retval` prints out the return value directly. However if not (killed by signal), it prints an error message and the signal number in line 2.

```shell 
$ echo "nonexistent-cmd" | retval
127
$ retval "echo working"
0
$ retval 'kill -INT $$'
Process was killed by signal. Signal number:
2
```