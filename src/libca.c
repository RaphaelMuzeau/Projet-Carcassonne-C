#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "libca.h"

void *ca_alloc(size_t n, size_t size)
{
    void *p = calloc(n, size);
    if (p == NULL)
        ca_perror();

    return p;
}

void *ca_realloc(void *p, size_t n, size_t size)
{
    // copie le comportement du calloc de glibc
    // pour verifier l'overflow
    size_t octets;
    if (__builtin_mul_overflow(n, size, &octets)) {
        errno = ENOMEM;
        ca_perror();
    }

    void *np = realloc(p, n * size);
    if (np == NULL)
        ca_perror();

    return np;
}

void ca_warn(const char *message)
{
    fprintf(stderr, "carcassonne: (warning) %s\n", message);
}

_Noreturn void ca_error(const char *message)
{
    fprintf(stderr, "carcassonne: %s\n", message);
    exit(EXIT_FAILURE);
}

// affiche un message d'erreur selon errno
_Noreturn void ca_perror(void)
{
    perror("carcassonne");
    exit(EXIT_FAILURE);
}
