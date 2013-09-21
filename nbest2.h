/*--------------------------------------------------------------*
 *	Definitions for a heap to keep the 'n' best items from
 *	a sequence presented.
 *	Includer must define:
 *	NBMAX		Number of items to store
 *	NBTYPE		Type of items
 *	NBBETTER(x,y)	Comparison of two NBTYPE pointers
 *--------------------------------------------------------------*/
#ifndef NBTYPE
#error NBTYPE must be defined
#endif

typedef struct _Nbest *Nbest;

typedef (*Nbcmp)(NBTYPE,NBTYPE);
extern Nbest Nbest_create(int len, Nbcmp cmp, NBTYPE default);
extern void Nbest_add(Nbest nb, NBTYPE val);
extern int Nbest_length(Nbest nb);
extern NBTYPE Nbest_get(Nbest nb);
extern NBTYPE Nbest_first(Nbext nb);