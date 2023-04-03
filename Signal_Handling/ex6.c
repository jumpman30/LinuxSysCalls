//
// Created by Signals Pereira on 28/03/2023.
//

/*
 It is generally not safe to use global or static variables inside a signal handler in C,
 because signal handlers can be called asynchronously and preempt the normal flow of
 execution.

When a signal is raised and the signal handler is invoked, the program's execution is
 interrupted and the handler function is executed in a separate execution context.
 If the handler function modifies global or static variables that are also accessed
 by the program's main execution context, this can lead to race conditions and other
 types of undefined behavior.

To avoid these issues, it is best to use only local variables inside signal handlers.
 If the handler needs to modify state that is also accessed by the main program, it should
 use some form of inter-process communication (IPC) to safely communicate with the main
 program. One way to achieve this is by using pipes or signals to communicate between the
 main program and the signal handler.
 */





