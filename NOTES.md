# What I learned while writing this

## getline()

getline() is a safe way to get user input from stdin, no matter the amount of input the user gives. It allocates the right amount of memory needed to the buffer, for all user input. The second argument, the length, tells it how big the currently allocated block of memory is. If it's 0, it allocates memory for your buffer. If not, it reallocates it. This way the user can enter a command as long as they like, both through piping it into my program and entering it the normal way

## fork()

`fork()` returns:
- `0` for the child process
- PID of  the child process

## syscalls and perror()

you can use perror to almost always display extra info on what exactly happened when a syscall fails. Uses errno to get that info

## execl()

execl() takes over the current process entirely, executes the given command and then exits with the return value of the command that was ran.

## dup2()
dup2() redirects a file descriptor to a different file descriptor (allows for stdin/out/err silencing, so a fork won't print to the terminal and wont read from it)

