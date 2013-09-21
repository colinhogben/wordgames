/*--------------------------------------------------------------*
 *	Merge sort of linked lists - Implementation
 *	14-Jun-93	First version
 *--------------------------------------------------------------*/
#include <stdlib.h>

struct _mslist {
  struct _mslist *next;
};

#define MSLIST struct _mslist
#include "msort.h"

struct _Msort {
  int (*cmp)(const MSLIST *,const MSLIST *);
  MSLIST *list[32];
};

Msort ms_new(int (*cmp)(const MSLIST *,const MSLIST *))
{
  Msort ms = (struct _Msort *)malloc(sizeof(struct _Msort));
  int i;
  ms->cmp = cmp;
  for (i=0; i<32; i++) {
    ms->list[i] = NULL;
  }
  return ms;
}

static MSLIST *merge(MSLIST *a, MSLIST *b,
		     int (*cmp)(const MSLIST *,const MSLIST *))
{
  MSLIST *list=NULL, **tail=&list;

  do {
    if (a==NULL) {
      *tail = b;
      return list;
    }else if (b==NULL) {
      *tail = a;
      return list;
    }else if (cmp(a, b) < 0) {
      *tail = a;
      tail = &a->next;
      a = *tail;
    }else{
      *tail = b;
      tail = &b->next;
      b = *tail;
    }
  } while (1);
}

void ms_add(Msort ms, MSLIST *add)
{
  int i;

  add->next = NULL;
  for (i=0; ms->list[i] != NULL; i++) {
    add = merge(add, ms->list[i], ms->cmp);
    ms->list[i] = NULL;
  }
  ms->list[i] = add;
}

MSLIST *ms_sorted(Msort ms)
{
  MSLIST *result = NULL;
  int i;

  for (i=0; i<32; i++) {
    result = merge(ms->list[i], result, ms->cmp);
    ms->list[i] = NULL;
  }
  return result;
}

int ms_length(Msort ms)
{
  int len=0, i;

  for (i=0; i<32; i++) {
    if (ms->list[i] != NULL) len += (1 << i);
  }
  return len;
}

MSLIST *ms_sort(MSLIST *list, int (*cmp)(const MSLIST *,const MSLIST *))
{
  MSLIST *m, *next;
  Msort ms = ms_new(cmp);

  for (m=list; m; m=next) {
    next = m->next;
    ms_add(ms, m);
  }
  m = ms_sorted(ms);
  free(ms);
  return m;
}
