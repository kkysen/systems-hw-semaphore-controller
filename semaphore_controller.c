//
// Created by kkyse on 12/6/2017.
//

#include <semaphore.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "util/stacktrace.h"
#include "util/utils.h"

/*
 * I used the newer and simpler POSIX semaphores
 * instead of the older, more complicated System V semaphores,
 * whose API we learned in class.
 */

typedef sem_t Semaphore;

int create_semaphore(const char *const semaphore_name, const int initial_value) {
    Semaphore *semaphore = sem_open(semaphore_name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, initial_value);
    if (semaphore == SEM_FAILED) {
        if (errno != EEXIST) {
            perror("sem_open");
            return -1;
        }
        errno = 0;
        printf("The semaphore named \"%s\" already exists.\n", semaphore_name);
        return 1;
    }
    
    if (sem_close(semaphore) == -1) {
        perror("sem_close");
        return -1;
    }
    return 0;
}

int read_semaphore(const char *const semaphore_name, int *const semaphore_value) {
    Semaphore *semaphore = sem_open(semaphore_name, 0);
    if (semaphore == SEM_FAILED) {
        if (errno != ENOENT) {
            perror("sem_open");
            return -1;
        }
        errno = 0;
        printf("The semaphore named \"%s\" doesnt exist yet.\n"
                       "You must create it first "
                       "by running \"%s -c N\", "
                       "where N = the initial value of the semaphore.\n",
               semaphore_name, semaphore_name);
        return 1;
    }
    
    if (sem_getvalue(semaphore, semaphore_value) == -1) {
        perror("sem_getvalue");
        return -1;
    }
    
    if (sem_close(semaphore) == -1) {
        perror("sem_close");
        return -1;
    }
    return 0;
}

int destroy_semaphore(const char *const semaphore_name) {
    if (sem_unlink(semaphore_name) == -1) {
        if (errno == ENOENT) {
            errno = 0;
            return 1;
        }
        perror("sem_unlink");
        return -1;
    }
    return 0;
}

void usage(const char *const program_name) {
    printf("Usage: %s -flag:\n"
                   "    where -flag = \n"
                   "        -c N: create a semaphore if it doesn't exist with an initial value of N\n"
                   "        -v: view the current value of the semaphore\n"
                   "        -r: remove the semaphore\n", program_name);
}

#define einval(assertion, message) \
    if (assertion) { \
        fprintf(stderr, "Invalid Argument: %s\n", message); \
        return EXIT_FAILURE; \
    }

int main(const int argc, const char *const *const argv) {
    set_stack_trace_signal_handler();
    
    if (argc <= 1) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }
    
    const char *semaphore_name = argv[0];
    if (semaphore_name[0] == '.' && semaphore_name[1] == '/') {
        semaphore_name += 2;
    }
    
    const char *const flag_arg = argv[1];
    einval(strlen(flag_arg) != 2, "flag should be 2 chars");
    einval(flag_arg[0] != '-', "flag should begin with '-'");
    const char flag = flag_arg[1];
    switch (flag) {
        case 'c': {
            einval(argc != 3, "-c must be followed only by one other argument, N, the initial value");
            const char *const initial_value_string = argv[2];
            const long initial_value_long = strtol(initial_value_string, NULL, 10);
            einval(errno == ERANGE, "N is out of range");
            einval(initial_value_long < 0, "N must be a positive");
            einval(initial_value_long > INT_MAX, "N must be a positive, signed 32-bit integer");
            const int initial_value = (int) initial_value_long;
            assert(create_semaphore(semaphore_name, initial_value) != -1);
            break;
        }
        case 'v': {
            einval(argc != 2, "no arguments may follow -v");
            int semaphore_value;
            const int ret_val = read_semaphore(semaphore_name, &semaphore_value);
            assert(ret_val != -1);
            if (ret_val != 0) {
                break;
            }
            printf("The value of the semaphore named \"%s\" is %d.\n",
                   semaphore_name, semaphore_value);
            break;
        }
        case 'r': {
            einval(argc != 2, "no arguments may follow -r");
            const int ret_val = destroy_semaphore(semaphore_name);
            assert(ret_val != -1);
            if (ret_val != 0) {
                break;
            }
            printf("Removed the semaphore named \"%s\".\n", semaphore_name);
            break;
        }
        default:
            einval(true, "flag must be either -c, -v, or -r");
    }
    
    return EXIT_SUCCESS;
}