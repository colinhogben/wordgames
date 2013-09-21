/*======================================================================
 *	Definitions for a memory allocator
 *======================================================================*/
#include <string.h>

/*--- Routine to free space when memory is tight */
typedef int (*MemFreer)(void *, size_t);

extern void *Mem_new(size_t size);
extern void Mem_free(void *addr);
extern void Mem_add_freer(MemFreer free, void *handle);
extern void Mem_remove_freer(MemFreer free, void *handle);

#define NEW(type) ((type *)Mem_new(sizeof(type)))
#define NEWARRAY(type,n) ((type *)Mem_new((n)*sizeof(type)))
#define Mem_strdup(s) strcpy(Mem_new(strlen(s)+1), s)
