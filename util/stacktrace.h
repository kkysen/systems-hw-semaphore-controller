//
// Created by kkyse on 11/8/2017.
//

#ifndef SYSTEMS_STACKTRACE_H
#define SYSTEMS_STACKTRACE_H

#include "sigaction.h"

/**
 * Print the stacktrace and PID of a thread to stderr.
 */
void print_stack_trace();

/**
 * Set the signal handler to a
 * signal handler that will print the current stacktrace.
 */
void set_stack_trace_signal_handler();

#endif //SYSTEMS_STACKTRACE_H
