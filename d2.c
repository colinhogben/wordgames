/*======================================================================
 *	Implementation of d2 dictionary
 *======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "mem.h"
#include "d2.h"
#include "d2file.h"			/* definition of file layout */

#define DEFAULT_DIR "/usr/share/dict/d2"

/*--- Definitions of structures half-declared in d2.h */
struct _Dictl {
  int len;
  Dict dict;				/* Optional parent */
  int nhead;				/* How many 2-char heads */
  D2fsect *hindex;			/* Array of head data */
  char *tails;				/* char[][len-2] */
};

struct _Dict {
  char *dir;
  Dictl dictl[MAXLEN+1];		/* Children managed */
  int used[MAXLEN+1];
};

struct _Dscan {
  Dictl dictl;
  int len2;
  char *word;
  int hcount;
  int tindex;
  int tend;
  D2fsect *hptr;
};

struct _Dpool {
  unsigned char nlet[26];
  unsigned char nwild;
};

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
 *	Destroy a Dictl. Called from Dict_close.
 *----------------------------------------------------------------------*/
static void Dictl_delete(Dictl dl)
{
  Mem_free(dl->hindex);
  Mem_free(dl->tails);
  Mem_free(dl);
}

/*----------------------------------------------------------------------
 *	Access a length dictionary from a file
 *----------------------------------------------------------------------*/
Dictl Dictl_fopen(const char *filename)
{
  Dictl dl;
  FILE *f;
  int ntail;
  D2fhdr hdr;

  dl = NEW(struct _Dictl);
  dl->dict = NULL;			/* No parent (yet) */
  f = fopen(filename, "rb");		/* open it for reading */
  if (f==NULL) fail("%s: Unable to open file\n", filename);

  if (fread(&hdr, sizeof(D2fhdr), 1, f) < 1)
    faile(filename);
  if (strncmp(hdr.id, "d2", 2) != 0)
    fail("%s: bad magic number", filename);
  if ((unsigned)hdr.nhead > 26*26)
    fail("%s: bad head count %d", filename, hdr.nhead);
  dl->len = hdr.len;
  dl->nhead = hdr.nhead;

  dl->hindex = NEWARRAY(D2fsect, dl->nhead);
  if (fread(dl->hindex, sizeof(D2fsect), dl->nhead, f) < dl->nhead)
    faile(filename);

  ntail = (dl->nhead == 0) ? 0 : dl->hindex[dl->nhead - 1].tend;
  dl->tails = NEWARRAY(char, ntail*(hdr.len-2));
  if (fread(dl->tails, sizeof(char), ntail*(hdr.len-2), f) < ntail*(hdr.len-2))
    faile(filename);

  fclose(f);

  return dl;
}

/*----------------------------------------------------------------------
 *	Access a length part of a Dict
 *----------------------------------------------------------------------*/
Dictl Dictl_open(Dict d, int len)
{
  Dictl dl;

  if (len < 1 || len > MAXLEN)
    fail("Dictl_open: length %d is out of range [1:%d]", len, MAXLEN);

  if ((dl = d->dictl[len]) == NULL) {	/* not already cached */
    char *fname = Mem_new(strlen(d->dir)+1+2+1);
    sprintf(fname, "%s/%02d", d->dir, len); /* make filename from stub */
    dl = d->dictl[len] = Dictl_fopen(fname);
    if (dl->len != len) fail("%s: bad word length %d", fname, dl->len);
    dl->dict = d;
    Mem_free(fname);
  }
  ++ d->used[len];
  return dl;
}
  
/*----------------------------------------------------------------------
 *	Relinquish use of a Dictl, but hang on to it internally in case
 *	it's needed again (saves re-reading the file).
 *----------------------------------------------------------------------*/
void Dictl_close(Dictl dl)
{
  if (dl->dict != NULL)
    -- dl->dict->used[dl->len];		/* decrement usage count */
}


/*----------------------------------------------------------------------
 *	Return the number of items in a Dictl
 *----------------------------------------------------------------------*/
int Dictl_length(Dictl dl)
{
  return dl->hindex[dl->nhead - 1].tend;
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

int Dictl_contains(Dictl dl, const char *word)
{
  int i,j,k, cmp, len2;
  len2 = dl->len-2;
  if (len2 < 0) {			/* Length 1 - special case */
    for (i=0; i<dl->nhead; i++) {
      if (dl->hindex[i].head[0] == *word) return 1;
    }
    return 0;
  }
  for (i=0, j=dl->nhead; i < j; ) {
    k = (i+j)/2;			/* binary chop */
    cmp = c2cmp(word, dl->hindex[k].head);
    if (cmp < 0) j = k;			/* try in lower half */
    else if (cmp > 0) i = k+1;		/* try in upper half */
    else break;				/* exact match */
  }
  if (i == j) return 0;			/* head not found */

  if (len2 == 0) return 1;		/* len=2: no tail */

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
 *	class Dict
 *	Controls a set of dictionary files (one for each word length).
 *	The Dictl objects are cached so they can be re-used when needed.
 *	If memory is restricted, it may be better (a) to delete a Dictl
 *	when its `used' count has fallen to zero; or (b) delete unused
 *	Dictls selectively if we run out of space making a new one.
 *======================================================================*/

/*--------------------------------------------------------------*
 *	If we've run out of free memory, de-allocate any Dictl
 *	entries that are unused at present
 *--------------------------------------------------------------*/
static int Dict_freer(void *vdict, size_t size)
{
  Dict d = vdict;
  int i;

  for (i=0; i<=MAXLEN; i++) {
    if (d->dictl[i] != NULL && d->used[i] == 0) {
/*      fprintf(stderr, "Dict_freer(%d): freeing dictl[%d]\n", size, i);*/
      Dictl_delete(d->dictl[i]);
      d->dictl[i] = NULL;
      return 1;
    }
  }
  return 0;
}

/*----------------------------------------------------------------------
 *	Create a dictionary from a given file stub (NULL => default)
 *----------------------------------------------------------------------*/
Dict Dict_open(const char *dir)
{
  int i;
  const char *envdir;
  Dict d = NEW(struct _Dict);
  d->dir = Mem_strdup(dir ? dir:
		      (envdir = getenv("D2DICT")) != NULL ? envdir:
		      (const char *)DEFAULT_DIR); /* copy file stub */
  for (i=0; i<=MAXLEN; i++) {
    d->dictl[i] = NULL;			/* no Dictl objects yet */
    d->used[i] = 0;			/* so not used */
  }
  Mem_add_freer(Dict_freer, d);
  return d;
}

/*----------------------------------------------------------------------
 *	Finish using a Dict
 *----------------------------------------------------------------------*/
void Dict_close(Dict d)
{
  int i;
  static void Dictl_delete(Dictl dl);

  Mem_remove_freer(Dict_freer, d);
  for (i=0; i<=MAXLEN; i++) {		/* remove all Dictls */
    if (d->dictl[i] != NULL) Dictl_delete(d->dictl[i]);
  }
  Mem_free(d->dir);
  Mem_free(d);
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
 *	class Dscan
 *	Allows a serial scan through a Dictl.
 *======================================================================*/
Dscan Dscan_open(Dict d, int len)
{
  Dscan ds = NEW(struct _Dscan);
  ds->dictl = Dictl_open(d, len);
  ds->len2 = len - 2;			/* length of tails */
  ds->word = NEWARRAY(char,len + 1);	/* space for words returned */
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
  if (ds->len2 > 0)
    strncpy(ds->word+2, ds->dictl->tails+(ds->tindex * ds->len2),
	    ds->len2);			/* copy the tail data */
  ds->tindex ++;

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
  Mem_free(ds->word);
  Mem_free(ds);
}

/*=======================================================================
 *	Pool of letters
 *=======================================================================*/
void
(dpool_clear)(Dpool *pPool)
{
  memset(pPool,0,sizeof(Dpool));
}

void
dpool_add(Dpool *pPool, const Dpool *pOther)
{
  int i;
  for (i=0; i<26; i++) {
    pPool->nlet[i] += pOther->nlet[i];
  }
  pPool->nwild += pOther->nwild;
}

void
(dpool_add_char)(Dpool *pPool, int c)
{
  if (isalpha(c)) {
    pPool->nlet[toupper(c)-'A']++;
  } else {
    pPool->nwild++;
  }
}

void
dpool_add_word(Dpool *pPool, const char *sWord)
{
  int c;
  while ((c=*sWord++) != '\0') {
    if (isalpha(c)) {
      pPool->nlet[toupper(c)-'A']++;
    } else {
      pPool->nwild++;
    }
  }
}

int
dpool_sub(Dpool *pPool, const Dpool *pOther)
{
  int i;
  for (i=0; i<26; i++) {
    if (pPool->nlet[i] < pOther->nlet[i]) {
      if (pPool->nlet[i] + pPool->nwild < pOther->nlet[i]) return -1;
      pPool->nwild -= pOther->nlet[i] - pPool->nlet[i];
      pPool->nlet[i] = 0;
    } else {
      pPool->nlet[i] -= pOther->nlet[i];
    }
  }
  if (pPool->nwild < pOther->nwild) return -1;
  pPool->nwild -= pOther->nwild;
  return 0;
}

int
dpool_sub_char(Dpool *pPool, int c)
{
  if (isalpha(c)) {
    int i = toupper(c) - 'A';
    if (pPool->nlet[i] > 0) {
      --pPool->nlet[i];
      return 0;
    }
  }
  if (pPool->nwild > 0) {
    --pPool->nwild;
    return 0;
  } else {
    return -1;
  }
}

int
dpool_sub_word(Dpool *pPool, const char *sWord)
{
  int c, i;
  while ((c=*sWord++) != '\0') {
    if (isalpha(c) &&
	(i = toupper(c) - 'A', pPool->nlet[i] > 0)) {
      --pPool->nlet[i];
    } else if (pPool->nwild > 0) {
      --pPool->nwild;
    } else {
      return -1;
    }
  }
  return 0;
}

void
dpool_string(char *sBuffer, const Dpool *pPool, int cWild)
{
  int i, j;
  for (i=0; i<26; i++) {
    for (j=0; j<pPool->nlet[i]; j++) {
      *sBuffer++ = 'A'+i;
    }
  }
  for (j=0; j<pPool->nwild; j++) {
    *sBuffer++ = cWild;
  }
  *sBuffer = '\0';
}

