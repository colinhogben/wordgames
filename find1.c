/***************************************************************************
 *      Find sub-words from dictionary                                     *
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

int Pool_size(Pool *p)
{
  int num=0, i;
  for (i=0; i<26; i++) {
    num += p->count[i];
  }
  return num;
}

/*-------------------------------------------------------------------------*
 *      Make a pool out of a word                                          *
 *-------------------------------------------------------------------------*/
void Pool_addword(Pool *p, const char *word)
{
  int c;

  while ((c = *word++) != 0) {
    if (isalpha(c)) {
      Pool_add(*p, toupper(c));
    }
  }
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
}

int main(int argc, char **argv)
{
  char word[100];
  Pool wordpool;
  int len, i;

  dict = Dict_open(NULL);
  Pool_clear(wordpool);

  if (argc == 1) {
    while (printf("word> "), fflush(stdout), 
	   fgets(word, sizeof(word), stdin)) {
      Pool_addword(&wordpool, word);
      len = Pool_size(&wordpool);
      if (len == 0) break;
      for (i=len; i>=0; i--) {
	find_subwords(&wordpool, i, print_word_and_remainder);
      }
    }
  } else {
    for (i=1; i<argc; i++) {
      Pool_addword(&wordpool, argv[i]);
    }
    len = Pool_size(&wordpool);
    for (i=len; i>=0; i--) {
      find_subwords(&wordpool, i, print_word_and_remainder);
    }
  }

  Dict_close(dict);
  return 0;
}
