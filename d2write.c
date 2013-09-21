/*======================================================================
 *	Implementation of writeable d2 dictionary
 *======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "mem.h"
#include "d2write.h"
#include "d2file.h"			/* definition of file layout */

#define DEFAULT_STUB "Words:Words"

/*--- Definitions of structure half-declared in d2write.h */
typedef struct _Dtail {
  struct _Dtail *next;
  char tail[0];
} Dtail;

#define MSLIST Dtail
#include "msort.h"

struct _Dictw {
  int len;
  int nhead;				/* How many 2-char heads */
  Dtail *sect[26*26];			/* Tail lists */
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
 *	class Dictw
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
 *	Create a Dictw.
 *----------------------------------------------------------------------*/
Dictw Dictw_create(int len)
{
  Dictw dw;
  int i;

  dw = NEW(struct _Dictw);
  dw->len = len;
  dw->nhead = 0;
  for (i=0; i<26*26; i++)
    dw->sect[i] = NULL;
  return dw;
}

/*----------------------------------------------------------------------
 *	Destroy a Dictw
 *----------------------------------------------------------------------*/
void Dictw_delete(Dictw dw)
{
  int i;
  Dtail *t, *next;

  for (i=0; i<26*26; i++)
    for (t=dw->sect[i]; t; t=next) {
      next = t->next;
      free(t);
    }
  free(dw);
}

/*----------------------------------------------------------------------
 *	Add a word to the dictionary
 *----------------------------------------------------------------------*/
static Dtail dummy;

void Dictw_add(Dictw dw, const char *word)
{
  int head = 0;
  Dtail *t;

  switch (dw->len) {
  case 1: case 2:
    head = 26*(word[0]-'A');
    if (dw->len == 2) head += (word[1]-'A');
    if (dw->sect[head] != NULL) return;	/* Already seen */
    dw->sect[head] = &dummy;
    ++ dw->nhead;
    break;
  default:
    head = 26*(word[0]-'A') + (word[1]-'A');
    for (t=dw->sect[head]; t; t=t->next) {
      if (strncmp(word+2, t->tail, dw->len-2)==0) return;
    }
    if (dw->sect[head] == NULL) ++ dw->nhead;
    t = (Dtail *)Mem_new(sizeof(Dtail) + dw->len - 2);
    t->next = dw->sect[head];
    strncpy(t->tail, word+2, dw->len-2);
    dw->sect[head] = t;
  }
}

/*----------------------------------------------------------------------
 *	Write the dictionary to file in d2 format
 *----------------------------------------------------------------------*/
static int tclen;
static int tailcmp(const Dtail *a, const Dtail *b)
{ return strncmp(a->tail, b->tail, tclen); }

void Dictw_write(Dictw dw, const char *filename)
{
  FILE *f;
  int i, ntail;
  D2fhdr hdr;
  D2fsect sect;
  Dtail *t;

  f = fopen(filename, "wb");
  if (f == NULL) fail("%s: Unable to open file for writing\n", filename);

  hdr.id[0] = 'd';  hdr.id[1] = '2';	/* Set id */
  hdr.len = dw->len;
  hdr.nhead = dw->nhead;
  if (fwrite(&hdr, sizeof(D2fhdr), 1, f) < 1)
    faile(filename);

  sect.UNUSED[0] = sect.UNUSED[1] = 0;	/* Clear unused bytes */
  ntail = 0;
  for (i=0; i<26*26; i++) {
    if (dw->sect[i] != NULL) {
      for (t=dw->sect[i]; t; t=t->next) {
	++ ntail;			/* Count total tails */
      }
      sect.head[0] = (i/26)+'A';
      sect.head[1] = dw->len==1 ? 0 : (i%26)+'A';
      sect.tend = ntail;
      if (fwrite(&sect, sizeof(D2fsect), 1, f) < 1)
	faile(filename);
    }
  }
  if (dw->len > 2) {
    tclen = dw->len - 2;		/* Length for tailcmp() */
    for (i=0; i<26*26; i++) {
      if (dw->sect[i] != NULL) {
	dw->sect[i] = ms_sort(dw->sect[i], tailcmp);
        for (t=dw->sect[i]; t; t=t->next) {
	  if (fwrite(t->tail, 1, tclen, f) < tclen)
	    faile(filename);
	}
      }
    }
  }
  if (fclose(f) < 0) faile(filename);
}
