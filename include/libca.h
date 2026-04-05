#ifndef libca_h
#define libca_h

#include <stdlib.h>

#define max(A, B) (((A) > (B)) ? (A) : (B))

void *ca_alloc(size_t n, size_t size);
void *ca_realloc(void *p, size_t n, size_t size);

void ca_warn(const char *message);
_Noreturn void ca_error(const char *message);

// affiche un message d'erreur selon errno
_Noreturn void ca_perror(void);

#endif
