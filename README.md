# retval

A small C program that runs a shell command and prints its return value. Reads the command from stdin, installs to `/usr/local/bin` with `make install`, and is run with `retval`.<br>
Feel free to change the code as you'd like. MIT License applies.
s
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

The command is read through stdin.
```shell
echo "exit 69" | retval  # 69
# or
retval # then type in your command
```

# Output format

If it returns normally, `retval` prints out the return value directly. However if not (killed by signal), it prints an error message and prints the signal number in line 2.

```shell 
$ echo "nonexistent-cmd" | retval
127
$ echo "echo working" | retval
0
$ echo 'kill -INT $$' | retval
Process was killed by signal. Signal number:
2
```