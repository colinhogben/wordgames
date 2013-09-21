/*======================================================================
 *	Process a dictionary to find the palindromable parts
 *======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define TRUE 1
#define FALSE 0

char *progname;				/* our own name */

typedef struct {
  int len;
  int map;				/* bit map of breaks, plus used flag */
  char *chars;
} Word;

#define BIT(pos) (1<<(pos))
#define MAPSET(word,pos) (((word)->map & BIT(pos)) != 0)
#define SETMAP(word,pos) ((word)->map |= BIT(pos))
#define CLRMAP(word,pos) ((word)->map &=~ BIT(pos))

#define MAXLEN 30
#define ISBREAK(word,pos) MAPSET(word,pos)
#define SETBREAK(word,pos) SETMAP(word,pos)
#define CLRBREAK(word,pos) CLRMAP(word,pos)
#define ISUSED(word) MAPSET(word,31)
#define SETUSED(word) SETMAP(word,31)
#define CLRUSED(word) CLRMAP(word,31)

static char *chk(char *al)
{
  if (al==NULL) {
    fprintf(stderr, "Out of allocation space\n");
    exit(1);
  }
  return al;
}

#define HEAPCHUNK 4096
static char *xmalloc(size_t size)
{
  static char *heap = "";
  static int hlen = 0;
  char *t;
  size = ((size-1) | 3)+1;             /* align to word */
  if (size >= HEAPCHUNK) {
    if ((t = malloc(size)) == NULL) {
      fprintf(stderr, "Out of allocation space\n");
      exit(1);
    }
  }else{
    if (size > hlen) {
      if ((heap = malloc(hlen=HEAPCHUNK)) == NULL) {
        fprintf(stderr, "Out of allocation space\n");
        exit(1);
      }
    }
    t = heap + (hlen -= size);
  }
  return t;
}

static char *strdup(const char *s)
{
  static char *heap = "";
  static int hlen = 0;
  int slen = strlen(s);
  if (slen+1 > hlen) {
    if ((heap = malloc(hlen=HEAPCHUNK)) == NULL) {
      fprintf(stderr, "Out of allocation space\n");
      exit(1);
    }
  }
  return strcpy(heap + (hlen -= slen+1), s);
}

static void tracef(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
#ifdef TRACE
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
#endif
  va_end(ap);
}

static Word **dict;			/* words in alphabetic order */
static Word **rdict;			/* words, in order if reversed */
static int nword = 0;			/* number of words */

/*----------------------------------------------------------------------
 *	copy in reverse a given length of string, adding a null
 *----------------------------------------------------------------------*/
static void revncpy(char *s, const char *t, int len)
{
  s[len] = '\0';
  for (; len; ) {
    s[--len] = *t++;
  }
}

/*----------------------------------------------------------------------
 *	compare two strings in reverse order
 *	Arguments s and t point to the ends of the strings
 *----------------------------------------------------------------------*/
static int revncmp(const char *s, const char *t, int len)
{
  int cmp = 0;
  while (len > 0 && ((cmp = *--s - *--t) == 0)) {
    len--;
  }
  return cmp;
}

/*----------------------------------------------------------------------
 *	compare two dictionary entries (used by qsort)
 *----------------------------------------------------------------------*/
static int wordcmp(const void *a, const void *b)
{
  return strcmp((*(Word **)a)->chars, (*(Word **)b)->chars);
}

/*----------------------------------------------------------------------
 *	compare two reversed dictionary entries (used by qsort)
 *----------------------------------------------------------------------*/
static int rwordcmp(const void *a, const void *b)
{
  Word *wa = *(Word **)a, *wb = *(Word **)b;
  int la = wa->len, lb = wb->len;
  int cmp = revncmp(wa->chars+la, wb->chars+lb, la<lb? la:lb);
  return cmp? cmp: la-lb;
}

/*----------------------------------------------------------------------
 *	test if a word is in the dictionary. If so, set its "used" flag
 *----------------------------------------------------------------------*/
static int use_word(const char *chars)
{
  int i,j,k,cmp;
/*  tracef("Word %s ?", chars);*/
  for (i=0,j=nword; j > i; ) {		/* perform binary chop */
    k = (i + j) / 2;			/* look in the middle */
    cmp = strcmp(chars, dict[k]->chars);
    if (cmp == 0) {			/* exact match */
      SETUSED(dict[k]);			/* mark as used */
/*      tracef("  yes");*/
      return TRUE;
    }else if (cmp < 0) {
      j = k;				/* look in first half */
    }else{
      i = k+1;				/* look in second half */
    }
  }
  return FALSE;
}

/*----------------------------------------------------------------------
 *	Check if a sequence of characters is a valid head of a word in
 *	the dictionary.
 *----------------------------------------------------------------------*/
static int use_head(const char *chars, int len)
{
  int i,j,k,cmp;
/*  tracef("Head %s ?", chars);*/
  for (i=0,j=nword; j-i > 1; ) {	/* perform binary chop */
    k = (i+j-1) / 2;			/* look in the middle(ish) */
    cmp = strncmp(dict[k]->chars, chars, len);
    if (cmp == 0) {
      j = k+1;
    }else if (cmp > 0) {
      j = k;				/* look in first half */
    }else{
      i = k+1;				/* look in second half */
    }
  }
  for (; i<nword && strncmp(dict[i]->chars, chars, len)==0; i++) {
    if (ISBREAK(dict[i],len)) {
/*      tracef("  %s", dict[i]->chars); */
      return TRUE;
    }
  }
  return FALSE;
}

/*----------------------------------------------------------------------
 *	Check if a sequence of characters is a valid tail of a word in
 *	the dictionary. Set the "used" flag of any such words.
 *----------------------------------------------------------------------*/
static int use_tail(const char *chars, int len)
{
  int i,j,k,cmp,dlen;
/*  tracef("Tail %s ?", chars-len);*/
  for (i=0,j=nword; j-i > 1; ) {	/* perform binary chop */
    k = (i+j-1) / 2;			/* look in the middle(ish) */
    dlen = rdict[k]->len;
    cmp = revncmp(rdict[k]->chars+dlen, chars, dlen<len? dlen: len);
    if (cmp == 0 && dlen >= len) {
      j = k+1;
    }else if (cmp > 0) {
      j = k;				/* look in first half */
    }else{
      i = k+1;				/* look in second half */
    }
  }
  for (; i<nword && (dlen=rdict[i]->len) >= len &&
         revncmp(rdict[i]->chars+dlen, chars, len)==0; i++) {
    if (ISBREAK(rdict[i],dlen-len)) {
/*      tracef("  %s", rdict[i]->chars);*/
      return TRUE;
    }
  }
  return FALSE;
}

/*----------------------------------------------------------------------
 *	Check the validity of a word break in the forward direction
 *----------------------------------------------------------------------*/
static int check_break(Word *word, int pos)
{
  int ok = FALSE;
  char rtext[MAXLEN+1];
  int tlen = word->len - pos;
  int i;
  revncpy(rtext, word->chars+pos, tlen);
  if (tlen == 1 || use_tail(rtext+tlen, tlen)) {
    ok = TRUE;
  }
  for (i=pos+1; i <= word->len; i++) {
    if (ISBREAK(word,i)) {
      revncpy(rtext, word->chars+pos, i-pos);
      if (use_word(rtext)) {
	ok = TRUE;
      }
    }
  }
  return ok;
}

static int check_word(Word *word)
{
  int len = word->len;
  int ok, i;
  int chg = FALSE;
/*  tracef("Checking %s", word->chars);*/
  for (i=len-1; i>=0; i--) {
    if (ISBREAK(word, i)) {
      ok = check_break(word, i);
      if (!ok) {
	CLRBREAK(word, i);
	chg = TRUE;
      }
    }
  }
  return chg;
}

/*----------------------------------------------------------------------
 *	Check the validity of a word break in the backward direction
 *----------------------------------------------------------------------*/
static int check_rbreak(Word *word, int pos)
{
  int ok = FALSE;
  char rtext[MAXLEN+1];
  int i;
  revncpy(rtext, word->chars, pos);
  if (pos == 1 || use_head(rtext, pos)) {
    ok = TRUE;
  }
  for (i=pos-1; i >= 0; i--) {
    if (ISBREAK(word,i)) {
      revncpy(rtext, word->chars+i, pos-i);
      if (use_word(rtext)) {
	ok = TRUE;
      }
    }
  }
  return ok;
}

static int check_rword(Word *word)
{
  int len = word->len;
  int ok, i;
  int chg = FALSE;
/*  tracef("rChecking %s", word->chars);*/
  for (i=1; i<=len; i++) {
    if (ISBREAK(word, i)) {
      ok = check_rbreak(word, i);
      if (!ok) {
	CLRBREAK(word, i);
	chg = TRUE;
      }
    }
  }
  return chg;
}

static void readdict(const char *fname)
{
  char line[256];
  int len;
  int dsize = 60000;
  Word *word;
  FILE *f = fopen(fname, "r");
  if (f==0) {
    perror(fname);
    exit(1);
  }
  dict = (Word **)xmalloc(dsize*sizeof(Word *));
  for (nword=0; fgets(line, 256, f); ) {
    len = strlen(line);
    if (line[len-1]=='\n') line[--len] = '\0';
    if (nword == dsize) {
      dsize *= 2;
/*      dict = (Word **)chk(realloc(dict, dsize*sizeof(Word *)));*/
    }
    word = (Word *)xmalloc(sizeof(Word));
    word->len = len;
    word->map = 0;
    word->chars = strdup(line);
    dict[nword++] = word;
  }
/*  dict = (Word **)chk(realloc(dict, nword*sizeof(Word *)));*/
}

/*----------------------------------------------------------------------
 *	Print out one of the dictionaries
 *----------------------------------------------------------------------*/
static void printword(const Word *word)
{
  int i;
  if (ISBREAK(word,0)) printf("-");
  for (i=0; i<word->len; i++) {
    printf("%c", word->chars[i]);
    if (ISBREAK(word,i+1)) printf("-");
  }
  printf("\n");
}

static void printdict(Word **d)
{
  int i;
  for (i=0; i<nword; i++) {
    printword(d[i]);
  }
}

/*----------------------------------------------------------------------
 *	Remove all words which neither contain a valid work break nor
 *	are used as a subword of a longer word
 *----------------------------------------------------------------------*/
static int purge(void)
{
  int i, j, oldn=nword;
  for (i=0, j=0; i<nword; i++) {
    if (rdict[i]->map != 0) {
      rdict[j++] = rdict[i];
    }
  }
  for (i=0, j=0; i<nword; i++) {
    if (dict[i]->map != 0) {
      CLRUSED(dict[i]);
      dict[j++] = dict[i];
    }else{
/*      tracef("Removed %s", dict[i]->chars);*/
    }
  }
  return oldn - (nword=j);
}

int main(int argc, char *argv[])
{
  int i, chg, rev, np;
  progname = argv[0];
  if (argc != 2) {
    fprintf(stderr, "Usage: %s dict\n", progname);
    exit(1);
  }
  tracef("Started");
  readdict(argv[1]);
  tracef("Dictionary read");
  rdict = (Word **)xmalloc(nword*sizeof(Word *));
  memcpy(rdict, dict, nword*sizeof(Word *));
  qsort(dict, nword, sizeof(Word *), wordcmp); /* sort forwards */
  tracef("Sorted fwd");
  qsort(rdict, nword, sizeof(Word *), rwordcmp); /* sort backwards */
  tracef("Sorted bwd");
  for (i=0; i<nword; i++) {
    dict[i]->map = (1<<(dict[i]->len+1))-1;
  }
  tracef("Set map");
  rev = FALSE;
  do {
    chg = FALSE;
    for (i=0; i<nword; i++) {
      if ((rev? check_rword: check_word)(dict[i])) chg = TRUE;
    }
    if ((np = purge()) != 0) chg = TRUE;
    tracef("%d words purged", np);
    rev = !rev;
  } while (chg);
  printdict(dict);
  return 0;
}
