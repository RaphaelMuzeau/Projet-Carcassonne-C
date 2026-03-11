#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void *ca_alloc(size_t n, size_t size)
{
    void *p = calloc(n, size);
    if (p == NULL) {
        perror("carcassonne");
        exit(EXIT_FAILURE);
    }
    return p;
}

void *ca_realloc(void *p, size_t n, size_t size)
{
    // copie le comportement du calloc de glibc
    // pour verifier l'overflow
    size_t octets;
    if (__builtin_mul_overflow(n, size, &octets)) {
        errno = ENOMEM;
        goto ca_realloc_fail;
    }

    void *np = realloc(p, n * size);
    if (np == NULL)
        goto ca_realloc_fail;

    return np;

ca_realloc_fail:
    perror("carcassonne");
    exit(EXIT_FAILURE);
}
