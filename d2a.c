/*======================================================================
 *	Implementation of d2 dictionary
 *======================================================================*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
extern int vfprintf(FILE *f, const char *fmt, va_list ap);
extern char *strdup(const char *s);

#define DEFAULT_STUB "Words:Words"

/* new(type) creates a new type, where type == struct _type*  */
#define new(t) ((t)xmalloc(sizeof(struct _##t)))
/* newarray(type,num) creates an array of num type elements */
#define newarray(t,n) ((t *)xmalloc((n)*sizeof(t)))
#define strdup(s) strcpy(xmalloc(strlen(s)+1), s)

/*----------------------------------------------------------------------
 *	report an error and die
 *----------------------------------------------------------------------*/
#define faile(str) (perror(str), exit(1))
static void fail(const char *fmt, ...)
{
  va_list ap;
  va_start(ap,fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);
  exit(1);
}

static void *xmalloc(int size)
{
  void *mem = malloc(size);
  if (mem == NULL) fail("Out of memory");
  return mem;
}
  
/*======================================================================
 *	class Dict
 *	Controls a set of dictionary files (one for each word length).
 *	The Dictl objects are cached so they can be re-used when needed.
 *	If memory is restricted, it may be better (a) to delete a Dictl
 *	when its `used' count has fallen to zero; or (b) delete unused
 *	Dictls selectively if we run out of space making a new one.
 *======================================================================*/

/*----------------------------------------------------------------------
 *	Create a dictionary from a given file stub (NULL => default)
 *----------------------------------------------------------------------*/
Dict Dict_open(const char *stub)
{
  int i;
  Dict d = new(Dict);
  d->filestub = strdup(stub? stub: (const char *)DEFAULT_STUB); /* copy the file stub */
  for (i=0; i<=MAXLEN; i++) {
    d->dictl[i] = NULL;			/* no Dictl objects yet */
    d->used[i] = NULL;			/* so not used */
  }
  return d;
}

/*----------------------------------------------------------------------
 *	Finish using a Dict
 *----------------------------------------------------------------------*/
void Dict_close(Dict d)
{
  int i;
  static void Dictl_delete(Dictl dl);

  for (i=0; i<=MAXLEN; i++) {		/* remove all Dictls */
    if (d->dictl[i] != NULL) Dictl_delete(d->dictl[i]);
  }
  free(d->filestub);
  free(d);
}

/*----------------------------------------------------------------------
 *	Test if a given word is in the dictionary
 *----------------------------------------------------------------------*/
int Dict_isword(Dict d, const char *word, int len)
{
  int result;
  Dictl dl = Dictl_open(d, len);	/* open section for this length */
  result = Dictl_isword(dl, word);	/* check the word */
  Dictl_close(dl);
  return result;
}

/*======================================================================
 *	class Dictl
 *	Controls part of a Dict dealing with words of a specific length.
 *	No per-object data, so just use the reference counts in Dict.
 *	Reads the data out of the file, which has the format:
 *		char[2] = "d2"		magic number
 *		short len		length of words
 *		int nhead		number of head pairs
 *		( char head[2]		first two letters of word(s)
 *	nhead *	(   2 bytes padding
 *		( int tend		cumulative number of tails
 *		char tails[ntail][len-2]	tails of words
 *======================================================================*/

/*----------------------------------------------------------------------
 *	Access a length part of a Dict
 *----------------------------------------------------------------------*/
Dictl Dictl_open(Dict d, int len)
{
  Dictl dl;

  if (len < 2 || len > MAXLEN)
    fail("Dictl_open: length %d is out of range [2:%d]", len, MAXLEN);
  if ((dl = d->dictl[len]) == NULL) {	/* not already cached */
    char *fname;
    FILE *f;
    char id[2];
    short flen;
    int ntail;

    dl = new(Dictl);
    dl->dict = d;
    dl->len = len;
    fname = malloc(strlen(d->filestub)+2+1);
    sprintf(fname, "%s%02d", d->filestub, len); /* make filename from stub */
    f = fopen(fname, "rb");		/* open it for reading */
    if (f==NULL) fail("%s: Unable to open file\n", fname);

    if (fread(id, sizeof(char), 2, f) < 2) faile(fname);
    if (strncmp(id, "d2", 2) != 0) fail("%s: bad magic number", fname);

    if (fread(&flen, sizeof(short), 1, f) < 1) faile(fname);
    if (flen != len) fail("%s: bad word length %d", fname, flen);

    if (fread(&dl->nhead, sizeof(int), 1, f) < 1) faile(fname);
    if ((unsigned)dl->nhead > 26*26)
      fail("%s: bad head count %d", fname, dl->nhead);

    dl->hindex = newarray(Dhead, dl->nhead);
    if (fread(dl->hindex, sizeof(Dhead), dl->nhead, f) < dl->nhead)
      faile(fname);

    ntail = dl->nhead == 0 ? 0 : dl->hindex[dl->nhead - 1].tend;
    dl->tails = newarray(char, ntail*(len-2));
    if (fread(dl->tails, sizeof(char), ntail*(len-2), f) < ntail*(len-2))
      faile(fname);

    fclose(f);
    free(fname);

    d->dictl[len] = dl;
  }
  d->used[len]++;			/* increment usage count */

  return dl;
}
  
/*----------------------------------------------------------------------
 *	Relinquish use of a Dictl, but hang on to it internally in case
 *	it's needed again (saves re-reading the file).
 *----------------------------------------------------------------------*/
void Dictl_close(Dictl dl)
{
  dl->dict->used[dl->len]--;		/* decrement usage count */
}

/*----------------------------------------------------------------------
 *	Destroy a Dictl. Called from Dict_close.
 *----------------------------------------------------------------------*/
static void Dictl_delete(Dictl dl)
{
  free(dl->hindex);
  free(dl->tails);
  free(dl);
}

/*----------------------------------------------------------------------
 *	Check if a word is in the given dictionary part
 *----------------------------------------------------------------------*/
/*#define c2cmp(s,t) strncmp(s,t,2)*/
#ifdef _GNUC_
#define c2cmp(s,t) (((int)(s)[0]-(t)[0]) ?: ((int)(s)[1]-(int)(t)[1]))
#else
#define c2cmp(s,t) (((int)(s)[0]-(t)[0]) ? ((int)(s)[0]-(int)(t)[0]) : ((int)(s)[1]-(int)(t)[1]))
#endif

int Dictl_isword(Dictl dl, const char *word)
{
  int i,j,k, cmp, len2;
  for (i=0, j=dl->nhead; i < j; ) {
    k = (i+j)/2;			/* binary chop */
    cmp = c2cmp(word, dl->hindex[k].head);
    if (cmp < 0) j = k;			/* try in lower half */
    else if (cmp > 0) i = k+1;		/* try in upper half */
    else break;				/* exact match */
  }
  if (i == j) return 0;			/* head not found */

  len2 = dl->len-2;
  for (i=(k==0)?0: dl->hindex[k-1].tend, j=dl->hindex[k].tend; i<j; ) {
    k = (i+j)/2;			/* binary chop */
    cmp = strncmp(word+2, dl->tails+k*len2, len2);
    if (cmp < 0) j = k;			/* try in lower half */
    else if (cmp > 0) i = k+1;		/* try in upper half */
    else break;				/* exact match */
  }
  return i < j;				/* true if tail found */
}
    
/*======================================================================
 *	class Dscan
 *	Allows a serial scan through a Dictl.
 *======================================================================*/
Dscan Dscan_open(Dict d, int len)
{
  Dscan ds = new(Dscan);
  ds->dictl = Dictl_open(d, len);
  ds->len2 = len - 2;			/* length of tails */
  ds->word = newarray(char,len + 1);	/* space for words returned */
  ds->word[len] = '\0';			/* drop in a terminator */
  ds->hcount = ds->dictl->nhead;	/* number of heads untried */
  ds->hptr = ds->dictl->hindex;		/* point to start of `file' */
  ds->tindex = 0;
  ds->tend = 0;				/* need to access next `block' */

  return ds;
}

/*----------------------------------------------------------------------
 *	Return the next word in the scan.
 *----------------------------------------------------------------------*/
const char *Dscan_read(Dscan ds)
{
  if (ds->tindex == ds->tend) {		/* at end of current head */
    if (ds->hcount == 0) return NULL;	/* no more to read */
    ds->word[0] = ds->hptr->head[0];	/* fetch next head */
    ds->word[1] = ds->hptr->head[1];
    ds->tend = ds->hptr->tend;		/* mark end of next block */
    ds->hptr++;
    ds->hcount--;
  }
  strncpy(ds->word+2, ds->dictl->tails+(ds->tindex++ * ds->len2),
	  ds->len2);			/* copy the tail data */

  return ds->word;
}

/*----------------------------------------------------------------------
 *	Hint to Dscan_read that we are not interested in words which
 *	begin with the same <skip> characters as the last word returned.
 *----------------------------------------------------------------------*/
void Dscan_skip(Dscan ds, int skip)
{
  if (skip < 2) ds->tindex = ds->tend;	/* ignore rest of current `block' */
}

/*----------------------------------------------------------------------
 *	Terminate a scan
 *----------------------------------------------------------------------*/
void Dscan_close(Dscan ds)
{
  Dictl_close(ds->dictl);
  free(ds->word);
  free(ds);
}
