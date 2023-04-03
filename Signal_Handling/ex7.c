/*
In general, it is best to use only a small subset of functions inside a signal handler in C,
 because signal handlers can be called asynchronously and can interrupt the normal flow of execution.

The POSIX standard specifies a set of "async-signal-safe" functions that are guaranteed to be safe
to call from within a signal handler. These functions have a restricted set of operations and do not
 use dynamic memory allocation, because memory allocation and deallocation functions are not considered
 to be async-signal-safe.

Here are some examples of commonly used async-signal-safe functions:

write(): This function is used to write data to a file descriptor, such as standard output or a
 log file.

read(): This function is used to read data from a file descriptor.

exit(): This function terminates the calling process.

_exit(): This function is similar to exit(), but it does not perform any standard I/O cleanup.

sigprocmask(): This function is used to examine or change the signal mask of the calling process.

kill(): This function is used to send a signal to a process or group of processes.

getpid(): This function returns the process ID of the calling process.

getppid(): This function returns the parent process ID of the calling process.

fcntl(): This function is used to manipulate file descriptor flags.

sigaction(): This function is used to examine or change the action associated with a specific signal.

*/
