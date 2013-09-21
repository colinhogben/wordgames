/*--------------------------------------------------------------*
 *	Find words in a two-dimensional array of letters moving	*
 *	up, down, left, right or diagonally.			*
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "d2.h"

#define MAXPOS 32
#define NONE (-1)

typedef int bits;                       /* must be at least MAXPOS bits */
#define isset(m,b) ((m)&(1<<(b)))
#define adbit(m,b) ((m)|(1<<(b)))
#define stbit(m,b) (m)|=(1<<(b))

int first[26];                          /* first pos containing letter */
int next[MAXPOS];                       /* next pos containing same letter */
bits move[MAXPOS];                      /* isset(move[x],y) if can move x->y */
int xmax, ymax, maxpos;			/* actual sizes */

#define FAIL 0
#define REDO 1
#define EXIT 2

/*--------------------------------------------------------------*
 *	recursively look for a word				*
 *--------------------------------------------------------------*/
int boggle(const char *word,            /* word to look for */
           int pos,                     /* current position in array */
           bits used)                   /* positions visited already */
{
  int c, p;
  if ((c=*word++) == '\0') return EXIT;	/* end of string, ok! */
  p = first[c - 'A'];			/* first square containing letter */
  if (p == NONE) return FAIL;           /* letter not in square */
  for (; p != NONE; p=next[p]) {
    if (pos == NONE || isset(move[pos],p)) {    /* can move to square */
      if (! isset(used,p)) {            /* not already used */
        switch (boggle(word, p, adbit(used,p))) {   /* recurse on remainder */
        case FAIL: return FAIL;         /* later letter impossible */
        case EXIT: return EXIT;         /* succeeded */
	/* else try other direction(s) */
        }
      }
    }
  }
  return REDO;                          /* no luck here */
}

void makemap(char *letters)
{
  int i,j,p,c;
  char *s = letters;
  for (i=0; i<26; i++) {
    first[i] = NONE;
  }
  for (i=0; i<maxpos; i++) {
    next[i] = NONE;
    move[i] = 0;
  }
  p = 0;
  for (j=0; j<ymax; j++) {
    for (i=0; i<xmax; i++,p++) {
      if (j>0) {
        if (i>0) stbit(move[p],p-xmax-1);
        stbit(move[p],p-xmax);
        if (i<xmax-1) stbit(move[p],p-xmax+1);
      }
      if (i>0) stbit(move[p],p-1);
      if (i<xmax-1) stbit(move[p],p+1);
      if (j<ymax-1) {
        if (i>0) stbit(move[p],p+xmax-1);
        stbit(move[p],p+xmax);
        if (i<xmax-1) stbit(move[p],p+xmax+1);
      }
    }
  }
  
  p = 0;
  for (j=0; j<ymax; j++) {
    for (i=0; i<xmax; i++,p++) {
      c = *s++;
      if (c == '\0') {
        fprintf(stderr, "Too few letters\n");
        exit(1);
      }else if (! isalpha(c)) {
        fprintf(stderr, "Non-alphabetic character %c\n", c);
        exit(1);
      }
      c = toupper(c) - 'A';
      
      next[p] = first[c];
      first[c] = p;
    }
  }
  if (*s != '\0') {
    fprintf(stderr, "Too many letters\n");
    exit(1);
  }
}	

#ifdef TEST
#define MAXTEST 6
char *test[6] = {
"the","cat","sat","on","a","mat"};
#endif

/*--------------------------------------------------------------*
 *	Main program						*
 *--------------------------------------------------------------*/
int main(int argc, char **argv)
{
  char letters[MAXPOS+1];
  int i,l;
  const char *word;
  Dict d = Dict_open(NULL);

  ymax = argc - 1;		/* Number of rows */
  if (argc < 3) {
    fprintf(stderr, "usage: %s row...\n", argv[0]);
    exit(1);
  }
  xmax = strlen(argv[1]);	/* Number of columns */
  if ((maxpos = xmax*ymax) > MAXPOS) {
    fprintf(stderr, "%s: total size > %d\n", argv[0], MAXPOS);
    exit(1);
  }
  strcpy(letters, "");
  for (i=1; i<=ymax; i++) {
    if (strlen(argv[i]) != xmax) {
      fprintf(stderr, "Wrong length of line %d\n", i);
      exit(1);
    }
    strcat(letters, argv[i]);
  }
  makemap(letters);
  
  for (l=3; l<=MAXLEN && l<=maxpos; l++) {
    Dscan ds = Dscan_open(d, l);
    while ((word = Dscan_read(ds)) != NULL) {
      if (boggle(word,NONE,0)==EXIT) {
        printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);

  return 0;
} 
