#ifndef libca_h
#define libca_h

#include <stdlib.h>

void *ca_alloc(size_t n, size_t size);
void *ca_realloc(void *p, size_t n, size_t size);

#endif
