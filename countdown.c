/***************************************************************************
 *      Find longest sub-word(s) from set of letters
 ***************************************************************************/
#include "d2.h"
#include <stdlib.h>
#include <stdio.h>                     
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

Dict dict;

/*-------------------------------------------------------------------------*
 *      types used                                                         *
 *-------------------------------------------------------------------------*/
typedef struct {unsigned char count[26];} Pool;
#define Pool_contains(p,c) ((p).count[(c)-'A']>0)
#define Pool_add(p,c) ((p).count[(c)-'A']++)
#define Pool_remove(p,c) ((p).count[(c)-'A']--)
#define Pool_count(p,c) ((p).count[(c)-'A'])
#define Pool_clear(p) {int i; for (i=0; i<26; i++) (p).count[i]=0;}

/*-------------------------------------------------------------------------*
 *      Make a pool out of a word                                          *
 *-------------------------------------------------------------------------*/
int make_pool(const char *word, Pool *p)
{
  int c, n=0;

  Pool_clear(*p);			/* clear the counts */

  while ((c = *word++) != 0) {
    if (isalpha(c)) {
      Pool_add(*p, toupper(c));
      n++;
    }
  }
  return n;
}

/*-------------------------------------------------------------------------*
 *      Process all sub-words of the given length made from the pool       *
 *-------------------------------------------------------------------------*/
void find_subwords(Pool *wordpool, int sublen,
                   void process(const char *sw, Pool *rem))
{
  if (sublen >= 2 && sublen <= MAXLEN) {
    Dscan ds = Dscan_open(dict, sublen);

    /* scan dictionary for each initial pair of letters */
    const char *subword;

    while ((subword = Dscan_read(ds)) != NULL) {
      Pool remd = *wordpool;
      int i, c, ok=TRUE;

      for (i=0; i<sublen; i++) {
      	c = subword[i];
      	if (Pool_contains(remd, c)) {
      	  Pool_remove(remd, c);
      	}else{
      	  ok = FALSE;
      	  Dscan_skip(ds, i);
      	  break;
      	}
      }
      if (ok) {
        process(subword, &remd);
      }
    }
    Dscan_close(ds);
  }
}

static int found;

void print_word_and_remainder(const char *word, Pool *remd)
{
  int c, n;
  printf("%s\t", word);

  for (c='A'; c<='Z'; c++) {
    for (n=Pool_count(*remd,c); n>0; n--) {
      putchar(c);
    }
  }
  putchar('\n');
  found = TRUE;
}

int main(int argc, char **argv)
{
  Pool wordpool;
  int len, i;
  int found1st;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s letters\n", argv[0]);
    exit(2);
  }
  dict = Dict_open(NULL);

  len = make_pool(argv[1], &wordpool);
  found = found1st = FALSE;
  for (i=len; i>=0 && !(found && found1st); i--) {
    if (found) {found1st=TRUE; found=FALSE;}
    find_subwords(&wordpool, i, print_word_and_remainder);
    
  }

  Dict_close(dict);
  return 0;
}
