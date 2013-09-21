/*--------------------------------------------------------------*
 *	Merge sort of linked lists.
 *	User must define MSLIST to be a structure whose first
 *	field is the 'next' pointer. E.g:
 *
 *	typedef struct foo {struct foo *next; int myfield;} Foo;
 *	#define MSLIST Foo
 *	#include <msort.h>
 *
 *	int before(const Foo *a, const Foo *b) {...}
 *	  Msort ms = ms_new(before);
 *	  ms_add(ms, foo); ...
 *	  list = ms_sorted(ms);
 *
 *	14-Jun-93	First version
 *--------------------------------------------------------------*/
typedef struct _Msort *Msort;		/* opaque handle */

extern Msort ms_new(int (*cmp)(const MSLIST *,const MSLIST *));
extern void ms_add(Msort ms, MSLIST *add);
extern MSLIST *ms_sorted(Msort ms);
extern int ms_length(Msort ms);
extern MSLIST *ms_sort(MSLIST *list, int (*cmp)(const MSLIST *,const MSLIST *));
