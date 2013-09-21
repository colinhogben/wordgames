/*======================================================================
 *	Implementation of memory allocator
 *======================================================================*/
#include "mem.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Freer {
  MemFreer freer;
  void *closure;
  struct Freer *next;
} Freer;

static Freer *freers;

#if DEBUG
#define TRACEF(args) (printf args, printf("\n"))
#else
#define TRACEF(args)
#endif

void *Mem_new(size_t size)
{
  void *mem;
  Freer *f;

  TRACEF(("Mem_new(%#x)", size));
  if ((mem = malloc(size)) != NULL) {	/* Initial attempt */
    TRACEF(("Allocated %#x..%#x", mem, (char*)mem+size));
    return mem;				/* Success */
  }

  for (f=freers; f; f=f->next) {
    TRACEF(("trying freer %#x(%#x)", f->freer, f->closure));
    if (f->freer(f->closure, size)) {	/* Freed some memory? */
      TRACEF(("freer freed some memory"));
      if ((mem = malloc(size)) != NULL)	{ /* Yes: retry malloc */
	TRACEF(("Allocated %#x..%#x", mem, (char*)mem+size));
	return mem;
      }
    }
  }
  fprintf(stderr, "Out of memory\n");
  exit(1);
  return NULL;
}

void Mem_free(void *addr)
{
  TRACEF(("Freeing %#x", addr));
  free(addr);
}

void Mem_add_freer(MemFreer freer, void *closure)
{
  Freer *f = NEW(Freer);
  f->freer = freer;
  f->closure = closure;
  f->next = freers;
  freers = f;
}

void Mem_remove_freer(MemFreer freer, void *closure)
{
  Freer **pf, *f;
  for (pf = &freers; (f=*pf) != NULL; pf=&f->next) {
    if (f->freer == freer && f->closure == closure) {
      *pf = f->next;			/* remove from chain */
      Mem_free(f);
      return;
    }
  }
/*  fprintf(stderr, "Mem_remove_freer: entry not found\n"); */
}
