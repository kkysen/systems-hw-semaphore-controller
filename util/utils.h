//
// Created by kkyse on 11/9/2017.
//

#ifndef SYSTEMS_UTIL_H
#define SYSTEMS_UTIL_H

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int uint;

#define arraylen(array) (sizeof((array)) / sizeof(*(array)))

#define PTR_MAX_STRLEN (((sizeof(void *) * 8) / 4) + 1)

#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif

#define extreme(cmp, a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a cmp _b ? _a : _b; })

#define min(a,b) extreme(<, a, b)

#define max(a, b) extreme(>, a, b)

#define memcpy_add(dst, src, size) \
    memcpy(dst, src, size); \
    (dst) += (size);

extern const char *HOME;

/**
 * Initialize the HOME global the environment variable HOME.
 */
void init_home();

#define p(s) printf(s"\n")
#define ps(var) printf(#var": '%s' (%zu)\n", (char *) (var), strlen(var))
#define pz(var) printf(#var": %zu\n", (size_t) (var))
#define pd(var) printf(#var": %zd\n", (ssize_t) (var))
#define pp(var) printf(#var": %p\n", var)
#define pb(var) printf(#var": %s\n", (var) ? "true" : "false")
#define pc(var) printf(#var": %c\n", (char) (var))

#define perror(s) fprintf(stderr, "[%d] %s: %s (%s at %s:%d)\n", getpid(), s, strerror(errno), __func__, __FILE__, __LINE__)

#define debug() printf("[%d] %s at %s:%d\n", getpid(), __func__, __FILE__, __LINE__)

#endif //SYSTEMS_UTIL_H
